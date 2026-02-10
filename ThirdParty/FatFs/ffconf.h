/*---------------------------------------------------------------------------/
/  FatFs Configuration - Optimized for STM32F103
/---------------------------------------------------------------------------*/

#ifndef _FFCONF
#define _FFCONF 32020

/*---------------------------------------------------------------------------/
/ Function Configurations
/---------------------------------------------------------------------------*/

#define _FS_READONLY    0   /* 0: Read/Write, 1: Read only */
#define _FS_MINIMIZE    0   /* 0: Full function, 1-3: Reduced functions */
#define _USE_STRFUNC    1   /* 0: Disable, 1-2: Enable string functions */
#define _USE_FIND       0   /* 0: Disable, 1: Enable f_findfirst/f_findnext */
#define _USE_MKFS       0   /* 0: Disable, 1: Enable f_mkfs */
#define _USE_FASTSEEK   0   /* 0: Disable, 1: Enable fast seek */
#define _USE_LABEL      0   /* 0: Disable, 1: Enable volume label functions */
#define _USE_FORWARD    0   /* 0: Disable, 1: Enable f_forward */

/*---------------------------------------------------------------------------/
/ Locale and Namespace Configurations
/---------------------------------------------------------------------------*/

#define _CODE_PAGE      437  /* US ASCII */

#define _USE_LFN        0    /* 0: Disable LFN (saves RAM) */
#define _MAX_LFN        255  /* Max LFN length (12-255) */

#define _LFN_UNICODE    0    /* 0: ANSI/OEM, 1: Unicode */
#define _STRF_ENCODE    3    /* 0: ANSI, 1: UTF-16LE, 2: UTF-16BE, 3: UTF-8 */
#define _FS_RPATH       0    /* 0: Disable relative path */

/*---------------------------------------------------------------------------/
/ Drive/Volume Configurations
/---------------------------------------------------------------------------*/

#define _VOLUMES        1    /* Number of volumes (logical drives) */
#define _STR_VOLUME_ID  0    /* 0: Use only 0-9 for drive ID */
#define _VOLUME_STRS    "SD"

#define _MULTI_PARTITION 0   /* 0: Single partition per drive */

#define _MIN_SS         512  /* Minimum sector size */
#define _MAX_SS         512  /* Maximum sector size (512 for SD cards) */

#define _USE_TRIM       0    /* 0: Disable TRIM */
#define _FS_NOFSINFO    0    /* 0: Use FSINFO */

/*---------------------------------------------------------------------------/
/ System Configurations
/---------------------------------------------------------------------------*/

#define _FS_TINY        0    /* 0: Normal, 1: Tiny (reduced buffer) */
#define _FS_NORTC       1    /* 1: No RTC (use fixed timestamp) */
#define _NORTC_MON      1
#define _NORTC_MDAY     1
#define _NORTC_YEAR     2024

#define _FS_LOCK        0    /* 0: Disable file lock */
#define _FS_REENTRANT   0    /* 0: Disable re-entrancy (not thread-safe) */
#define _FS_TIMEOUT     1000
#define _SYNC_t         void*

#define _WORD_ACCESS    0    /* 0: Byte-by-byte access (ARM compatible) */

#endif /* _FFCONF */
