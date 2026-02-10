/*-----------------------------------------------------------------------*/
/* Low level disk I/O module for FatFs                                   */
/*-----------------------------------------------------------------------*/

#include <diskio.h>
#include <fatfs_sd.h>

/*-----------------------------------------------------------------------*/
/* Get Drive Status                                                      */
/*-----------------------------------------------------------------------*/

DSTATUS disk_status(BYTE pdrv) {
    return SD_disk_status(pdrv);
}

/*-----------------------------------------------------------------------*/
/* Initialize a Drive                                                    */
/*-----------------------------------------------------------------------*/

DSTATUS disk_initialize(BYTE pdrv) {
    return SD_disk_initialize(pdrv);
}

/*-----------------------------------------------------------------------*/
/* Read Sector(s)                                                        */
/*-----------------------------------------------------------------------*/

DRESULT disk_read(BYTE pdrv, BYTE *buff, DWORD sector, UINT count) {
    return SD_disk_read(pdrv, buff, sector, count);
}

/*-----------------------------------------------------------------------*/
/* Write Sector(s)                                                       */
/*-----------------------------------------------------------------------*/

DRESULT disk_write(BYTE pdrv, const BYTE *buff, DWORD sector, UINT count) {
    return SD_disk_write(pdrv, buff, sector, count);
}

/*-----------------------------------------------------------------------*/
/* Miscellaneous Functions                                               */
/*-----------------------------------------------------------------------*/

DRESULT disk_ioctl(BYTE pdrv, BYTE cmd, void *buff) {
    return SD_disk_ioctl(pdrv, cmd, buff);
}

/*-----------------------------------------------------------------------*/
/* Get current time for FatFs timestamps                                 */
/*-----------------------------------------------------------------------*/

DWORD get_fattime(void) {
    /* Return a fixed timestamp if RTC is not available */
    /* Format: bit31:25=Year(0-127 from 1980), bit24:21=Month(1-12),
       bit20:16=Day(1-31), bit15:11=Hour(0-23), bit10:5=Min(0-59),
       bit4:0=Sec/2(0-29) */
    return ((DWORD)(2024 - 1980) << 25)  /* Year 2024 */
         | ((DWORD)1 << 21)              /* January */
         | ((DWORD)1 << 16)              /* Day 1 */
         | ((DWORD)0 << 11)              /* Hour 0 */
         | ((DWORD)0 << 5)               /* Min 0 */
         | ((DWORD)0 >> 1);              /* Sec 0 */
}
