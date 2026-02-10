#include <hc04bt.h>
#include <application.h>
#include <ILI9341.h>
#include <string.h>
#include <stdio.h>

void BT_config(UART_HandleTypeDef *huart)
{
    // check existence
    HAL_Delay(100);
    uint8_t cmd[] = "AT";
    HAL_UART_Transmit(huart, cmd, strlen((char *)cmd), 10);
    HAL_StatusTypeDef status = HAL_UART_Receive(huart, rx_buffer, 4, 1000); // this command will wait buffer filled or timeout
    if (status == HAL_OK && strncmp((char *)rx_buffer, "OK", 2) == 0)
    {
        LCD_draw_string(&lcd_config, 0, 8, "HC04 Detected!", COLOR_BLACK, COLOR_WHITE);
    }
    else
    {
        LCD_draw_string(&lcd_config, 0, 8, "HC04 cannot find!", COLOR_BLACK, COLOR_WHITE);
    }

    HAL_Delay(500);

    memset(rx_buffer, 0, sizeof(rx_buffer));
    uint8_t cmd2[] = "AT+BAUD=921600";
    HAL_UART_Transmit(huart, cmd2, strlen((char *)cmd2), 300);
    status = HAL_UART_Receive(huart, rx_buffer, 6, 1000); // this command will wait buffer filled or timeout
    LCD_draw_string(&lcd_config, 5, 10, (char *)rx_buffer, COLOR_BLACK, COLOR_WHITE);
    // DO NOT write any code below, they will not work (HAL Time out & cannot receive), change command above and re-flash to config HC-04
    // HC-04 auto restart on such command (NAME, PIN, BAUDRATE)
    // The setting's baud rate is same as normal use baud rate, restart will change the setting's baud rate as well.
}

void BT_send_info(UART_HandleTypeDef *huart, uint8_t *info, uint16_t info_size)
{
    HAL_UART_StateTypeDef state = HAL_UART_GetState(huart);
    if (state == HAL_UART_STATE_READY || state == HAL_UART_STATE_BUSY_RX)
    {
        HAL_UART_Transmit_IT(huart, info, info_size);
    }
}

/*
 * Bypass the HAL state machine (gState/Lock can block HAL_UART_Transmit
 * when DMA RX is active on the same handle).  Write directly to the
 * USART1 hardware registers instead.
 */
void HC04_SendBytes(uint8_t *data, uint16_t len)
{
    for (uint16_t i = 0; i < len; i++)
    {
        /* Wait for TXE (TX data register empty) */
        uint32_t t0 = HAL_GetTick();
        while (!(USART1->ISR & USART_ISR_TXE_TXFNF))
        {
            if (HAL_GetTick() - t0 > 50)
                return; /* safety timeout */
        }
        USART1->TDR = data[i];
    }
    /* Wait for TC (Transmission Complete) so all bits are on the wire */
    uint32_t t0 = HAL_GetTick();
    while (!(USART1->ISR & USART_ISR_TC))
    {
        if (HAL_GetTick() - t0 > 50)
            return;
    }
}

/* Send a null-terminated string over Bluetooth (blocking, register-level) */
void HC04_SendString(const char *str)
{
    HC04_SendBytes((uint8_t *)str, strlen(str));
}
