/*-----------------------------------------------------------------------*/
/* Low level disk I/O module for FatFs - SD card over SPI (STM32H5)     */
/*                                                                       */
/* Uses direct SPI register access with TSIZE=0 (unlimited mode) to     */
/* keep the clock running continuously. The H5 HAL starts/stops SPI     */
/* per transfer call, creating clock gaps that SD cards reject.         */
/*-----------------------------------------------------------------------*/

#include "ff.h"
#include "diskio.h"
#include "spi.h"
#include "main.h"
#include <string.h>

// CS pin at PC4, SPI instance: SPI1

/* SD card CS pin */
#define SD_CS_PORT  SD_CS_GPIO_Port
#define SD_CS_PIN   SD_CS_Pin

#define SD_CS_LOW()   HAL_GPIO_WritePin(SD_CS_PORT, SD_CS_PIN, GPIO_PIN_RESET)
#define SD_CS_HIGH()  HAL_GPIO_WritePin(SD_CS_PORT, SD_CS_PIN, GPIO_PIN_SET)

/* SD command definitions */
#define CMD0    0   /* GO_IDLE_STATE */
#define CMD1    1   /* SEND_OP_COND (MMC) */
#define CMD8    8   /* SEND_IF_COND */
#define CMD9    9   /* SEND_CSD */
#define CMD10   10  /* SEND_CID */
#define CMD12   12  /* STOP_TRANSMISSION */
#define CMD16   16  /* SET_BLOCKLEN */
#define CMD17   17  /* READ_SINGLE_BLOCK */
#define CMD18   18  /* READ_MULTIPLE_BLOCK */
#define CMD24   24  /* WRITE_BLOCK */
#define CMD25   25  /* WRITE_MULTIPLE_BLOCK */
#define CMD55   55  /* APP_CMD */
#define CMD58   58  /* READ_OCR */
#define ACMD41  (0x80 | 41)  /* SD_SEND_OP_COND (SDC) - bit7 flags app cmd */

/* Card type flags */
#define CT_MMC   0x01  /* MMC ver 3 */
#define CT_SD1   0x02  /* SD ver 1 */
#define CT_SD2   0x04  /* SD ver 2 */
#define CT_BLOCK 0x08  /* Block addressing */

static volatile DSTATUS Stat = STA_NOINIT;
static BYTE CardType;

/* Debug: read these from main to see where init fails */
volatile uint8_t sd_dbg_spi_ok;
volatile uint8_t sd_dbg_hal_err;
volatile uint8_t sd_dbg_cmd0;
volatile uint8_t sd_dbg_cmd8;
volatile uint8_t sd_dbg_acmd41;
volatile uint8_t sd_dbg_cardtype;

extern SPI_HandleTypeDef hspi1;

/*-----------------------------------------------------------------------*/
/* SPI low-level: register-based continuous transfer for STM32H5         */
/*                                                                       */
/* The H5 SPI HAL enables/disables SPE for every transfer, creating     */
/* gaps in the clock. SD cards need continuous clocking within a CS      */
/* assertion. We use TSIZE=0 (unlimited) mode and directly access       */
/* TXDR/RXDR to keep the clock running.                                 */
/*-----------------------------------------------------------------------*/

/* Start continuous SPI session (call before CS_LOW or after CS changes) */
static void spi_begin(void)
{
	SPI_TypeDef *spi = hspi1.Instance;

	/* If SPI is already running, nothing to do */
	if (spi->CR1 & SPI_CR1_SPE) return;

	/* Clear all status flags */
	spi->IFCR = 0xFFFFFFFFU;

	/* TSIZE=0: unlimited transfer - clock runs until we stop it */
	spi->CR2 = 0;

	/* Enable SPI and start master transfer */
	SET_BIT(spi->CR1, SPI_CR1_SPE);
	SET_BIT(spi->CR1, SPI_CR1_CSTART);
}

/* Stop continuous SPI session (call before CS_HIGH) */
static void spi_end(void)
{
	SPI_TypeDef *spi = hspi1.Instance;

	/* If SPI is not running, nothing to do */
	if (!(spi->CR1 & SPI_CR1_SPE)) return;

	/* Request suspend and wait */
	SET_BIT(spi->CR1, SPI_CR1_CSUSP);
	while (!(spi->SR & SPI_SR_SUSP) && (spi->CR1 & SPI_CR1_CSTART));

	/* Disable SPI */
	CLEAR_BIT(spi->CR1, SPI_CR1_SPE);
	spi->IFCR = 0xFFFFFFFFU;

	/* Keep HAL state in sync so HAL calls still work for init etc. */
	hspi1.State = HAL_SPI_STATE_READY;
}

/* Exchange one byte - must be called between spi_begin() and spi_end() */
static BYTE spi_txrx(BYTE data)
{
	SPI_TypeDef *spi = hspi1.Instance;

	while (!(spi->SR & SPI_SR_TXP));
	*(volatile uint8_t *)&spi->TXDR = data;
	while (!(spi->SR & SPI_SR_RXP));
	return *(volatile uint8_t *)&spi->RXDR;
}

/* Transmit multiple bytes (discard received data) */
static void spi_tx_multi(const BYTE *buf, UINT cnt)
{
	SPI_TypeDef *spi = hspi1.Instance;
	while (cnt--) {
		while (!(spi->SR & SPI_SR_TXP));
		*(volatile uint8_t *)&spi->TXDR = *buf++;
		while (!(spi->SR & SPI_SR_RXP));
		(void)*(volatile uint8_t *)&spi->RXDR;
	}
}

/* Receive multiple bytes (transmit 0xFF) */
static void spi_rx_multi(BYTE *buf, UINT cnt)
{
	SPI_TypeDef *spi = hspi1.Instance;
	while (cnt--) {
		while (!(spi->SR & SPI_SR_TXP));
		*(volatile uint8_t *)&spi->TXDR = 0xFF;
		while (!(spi->SR & SPI_SR_RXP));
		*buf++ = *(volatile uint8_t *)&spi->RXDR;
	}
}

/*-----------------------------------------------------------------------*/
/* Wait for card ready                                                   */
/*-----------------------------------------------------------------------*/

static int wait_ready(UINT timeout_ms)
{
	BYTE d;
	uint32_t start = HAL_GetTick();

	do {
		d = spi_txrx(0xFF);
		if (d == 0xFF) return 1;
	} while ((HAL_GetTick() - start) < timeout_ms);

	return 0;
}

/*-----------------------------------------------------------------------*/
/* Deselect card and release SPI                                         */
/*-----------------------------------------------------------------------*/

static void deselect(void)
{
	spi_end();
	SD_CS_HIGH();
	/* Send release byte with CS high */
	spi_begin();
	spi_txrx(0xFF);
	spi_end();
}

/*-----------------------------------------------------------------------*/
/* Select card and wait for ready                                        */
/*-----------------------------------------------------------------------*/

static int select(void)
{
	SD_CS_LOW();
	spi_begin();
	spi_txrx(0xFF);
	if (wait_ready(500)) return 1;  /* SPI stays running */
	/* Timed out - release */
	spi_end();
	SD_CS_HIGH();
	spi_begin();
	spi_txrx(0xFF);
	spi_end();
	return 0;
}

/*-----------------------------------------------------------------------*/
/* Receive a data packet from the card                                   */
/*-----------------------------------------------------------------------*/

static int rcvr_datablock(BYTE *buff, UINT btr)
{
	BYTE token;
	uint32_t start = HAL_GetTick();

	do {
		token = spi_txrx(0xFF);
	} while (token == 0xFF && (HAL_GetTick() - start) < 200);

	if (token != 0xFE) return 0;

	spi_rx_multi(buff, btr);
	spi_txrx(0xFF);  /* Discard CRC */
	spi_txrx(0xFF);

	return 1;
}

/*-----------------------------------------------------------------------*/
/* Send a data packet to the card                                        */
/*-----------------------------------------------------------------------*/

#if FF_FS_READONLY == 0
static int xmit_datablock(const BYTE *buff, BYTE token)
{
	BYTE resp;

	if (!wait_ready(500)) return 0;

	spi_txrx(token);
	if (token != 0xFD) {
		spi_tx_multi(buff, 512);
		spi_txrx(0xFF);  /* Dummy CRC */
		spi_txrx(0xFF);
		resp = spi_txrx(0xFF);
		if ((resp & 0x1F) != 0x05) return 0;
	}

	return 1;
}
#endif

/*-----------------------------------------------------------------------*/
/* Send a command packet to the card                                     */
/*-----------------------------------------------------------------------*/

static BYTE send_cmd(BYTE cmd, DWORD arg)
{
	BYTE n, res;

	/* ACMD<n> is CMD55 + CMD<n> */
	if (cmd & 0x80) {
		cmd &= 0x7F;
		res = send_cmd(CMD55, 0);
		if (res > 1) return res;
	}

	/* Select the card and wait for ready (except CMD12) */
	if (cmd != CMD12) {
		deselect();
		if (!select()) return 0xFF;
	}

	/* Send command packet (SPI is running from select()) */
	spi_txrx(0x40 | cmd);
	spi_txrx((BYTE)(arg >> 24));
	spi_txrx((BYTE)(arg >> 16));
	spi_txrx((BYTE)(arg >> 8));
	spi_txrx((BYTE)arg);

	n = 0x01;  /* Dummy CRC + Stop */
	if (cmd == CMD0) n = 0x95;
	if (cmd == CMD8) n = 0x87;
	spi_txrx(n);

	/* Receive command response */
	if (cmd == CMD12) spi_txrx(0xFF);

	n = 10;
	do {
		res = spi_txrx(0xFF);
	} while ((res & 0x80) && --n);

	return res;
}

/*-----------------------------------------------------------------------*/
/* Get Drive Status                                                      */
/*-----------------------------------------------------------------------*/

DSTATUS disk_status(BYTE pdrv)
{
	if (pdrv != 0) return STA_NOINIT;
	return Stat;
}

/*-----------------------------------------------------------------------*/
/* Initialize a Drive                                                    */
/*-----------------------------------------------------------------------*/

DSTATUS disk_initialize(BYTE pdrv)
{
	BYTE n, cmd, ty, ocr[4], resp;
	uint32_t start;

	if (pdrv != 0) return STA_NOINIT;

	/* Disable NSSP mode, ensure 8-bit, and re-init SPI (one-time setup) */
	HAL_SPI_DeInit(&hspi1);
	hspi1.Init.NSSPMode = SPI_NSS_PULSE_DISABLE;
	hspi1.Init.DataSize = SPI_DATASIZE_8BIT;
	HAL_SPI_Init(&hspi1);

	/* Power-up delay */
	HAL_Delay(50);

	/* Send 80+ clock pulses with CS high to put card in SPI mode */
	SD_CS_HIGH();
	spi_begin();
	for (n = 0; n < 20; n++) spi_txrx(0xFF);
	spi_end();

	/* SPI sanity check */
	{
		BYTE rx;
		spi_begin();
		rx = spi_txrx(0xFF);
		spi_end();
		sd_dbg_spi_ok = (rx == 0xFF) ? 1 : 0;
		sd_dbg_hal_err = 0;
	}

	/* CMD0: GO_IDLE_STATE */
	resp = 0xFF;
	for (n = 0; n < 10 && resp != 1; n++) {
		SD_CS_LOW();
		spi_begin();
		spi_txrx(0xFF);

		spi_txrx(0x40 | CMD0);
		spi_txrx(0x00);
		spi_txrx(0x00);
		spi_txrx(0x00);
		spi_txrx(0x00);
		spi_txrx(0x95);

		{
			BYTE i;
			for (i = 0; i < 10; i++) {
				resp = spi_txrx(0xFF);
				if (!(resp & 0x80)) break;
			}
		}

		spi_end();
		SD_CS_HIGH();
		spi_begin();
		spi_txrx(0xFF);
		spi_end();
	}
	sd_dbg_cmd0 = resp;

	ty = 0;
	if (resp == 1) {  /* Card is in idle state */
		start = HAL_GetTick();

		resp = send_cmd(CMD8, 0x1AA);
		sd_dbg_cmd8 = resp;

		if (resp == 1) {  /* SDv2 */
			for (n = 0; n < 4; n++) ocr[n] = spi_txrx(0xFF);

			if (ocr[2] == 0x01 && ocr[3] == 0xAA) {
				/* Wait for leaving idle state (ACMD41 with HCS bit) */
				do {
					resp = send_cmd(ACMD41, 1UL << 30);
					sd_dbg_acmd41 = resp;
				} while (resp && (HAL_GetTick() - start) < 2000);

				if (resp == 0 && send_cmd(CMD58, 0) == 0) {
					for (n = 0; n < 4; n++) ocr[n] = spi_txrx(0xFF);
					ty = (ocr[0] & 0x40) ? CT_SD2 | CT_BLOCK : CT_SD2;
				}
			}
		} else {  /* SDv1 or MMCv3 */
			if (send_cmd(ACMD41, 0) <= 1) {
				ty = CT_SD1;
				cmd = ACMD41;
			} else {
				ty = CT_MMC;
				cmd = CMD1;
			}
			/* Wait for leaving idle state */
			do {
				resp = send_cmd(cmd, 0);
				sd_dbg_acmd41 = resp;
			} while (resp && (HAL_GetTick() - start) < 2000);

			/* Set block length to 512 */
			if (resp != 0 || send_cmd(CMD16, 512) != 0)
				ty = 0;
		}
	}

	CardType = ty;
	sd_dbg_cardtype = ty;
	deselect();

	if (ty) {
		Stat &= ~STA_NOINIT;
	} else {
		Stat = STA_NOINIT;
	}

	return Stat;
}

/*-----------------------------------------------------------------------*/
/* Read Sector(s)                                                        */
/*-----------------------------------------------------------------------*/

DRESULT disk_read(BYTE pdrv, BYTE *buff, LBA_t sector, UINT count)
{
	BYTE cmd;

	if (pdrv != 0 || count == 0) return RES_PARERR;
	if (Stat & STA_NOINIT) return RES_NOTRDY;

	if (!(CardType & CT_BLOCK)) sector *= 512;

	cmd = (count > 1) ? CMD18 : CMD17;
	if (send_cmd(cmd, sector) == 0) {
		do {
			if (!rcvr_datablock(buff, 512)) break;
			buff += 512;
		} while (--count);
		if (cmd == CMD18) send_cmd(CMD12, 0);
	}
	deselect();

	return count ? RES_ERROR : RES_OK;
}

/*-----------------------------------------------------------------------*/
/* Write Sector(s)                                                       */
/*-----------------------------------------------------------------------*/

#if FF_FS_READONLY == 0
DRESULT disk_write(BYTE pdrv, const BYTE *buff, LBA_t sector, UINT count)
{
	if (pdrv != 0 || count == 0) return RES_PARERR;
	if (Stat & STA_NOINIT) return RES_NOTRDY;

	if (!(CardType & CT_BLOCK)) sector *= 512;

	if (count == 1) {
		if (send_cmd(CMD24, sector) == 0 && xmit_datablock(buff, 0xFE))
			count = 0;
	} else {
		if (send_cmd(CMD25, sector) == 0) {
			do {
				if (!xmit_datablock(buff, 0xFC)) break;
				buff += 512;
			} while (--count);
			if (!xmit_datablock(0, 0xFD))
				count = 1;
		}
	}
	deselect();

	return count ? RES_ERROR : RES_OK;
}
#endif

/*-----------------------------------------------------------------------*/
/* Miscellaneous Functions                                               */
/*-----------------------------------------------------------------------*/

DRESULT disk_ioctl(BYTE pdrv, BYTE cmd, void *buff)
{
	DRESULT res;
	BYTE n, csd[16];
	DWORD csize;

	if (pdrv != 0) return RES_PARERR;
	if (Stat & STA_NOINIT) return RES_NOTRDY;

	res = RES_ERROR;

	switch (cmd) {
	case CTRL_SYNC:
		if (select()) res = RES_OK;
		break;

	case GET_SECTOR_COUNT:
		if (send_cmd(CMD9, 0) == 0 && rcvr_datablock(csd, 16)) {
			if ((csd[0] >> 6) == 1) {  /* SDC ver 2.00 */
				csize = csd[9] + ((WORD)csd[8] << 8) + ((DWORD)(csd[7] & 63) << 16) + 1;
				*(LBA_t *)buff = csize << 10;
			} else {  /* SDC ver 1.XX or MMC ver 3 */
				n = (csd[5] & 15) + ((csd[10] & 128) >> 7) + ((csd[9] & 3) << 1) + 2;
				csize = (csd[8] >> 6) + ((WORD)csd[7] << 2) + ((WORD)(csd[6] & 3) << 10) + 1;
				*(LBA_t *)buff = csize << (n - 9);
			}
			res = RES_OK;
		}
		break;

	case GET_BLOCK_SIZE:
		*(DWORD *)buff = 128;
		res = RES_OK;
		break;

	default:
		res = RES_PARERR;
	}

	deselect();
	return res;
}

DWORD get_fattime(void)
{
	return ((DWORD)(2026 - 1980) << 25)
	     | ((DWORD)1 << 21)
	     | ((DWORD)1 << 16);
}
