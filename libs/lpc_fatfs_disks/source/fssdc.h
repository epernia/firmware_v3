/*-----------------------------------------------------------------------*/
/* MMCv3/SDv1/SDv2 (in SPI mode) control module  (C)ChaN, 2007           */
/*-----------------------------------------------------------------------*/
/* Only rcvr_spi(), xmit_spi(), disk_timerproc() and some macros         */
/* are platform dependent.                                               */
/*-----------------------------------------------------------------------*/
/* New Edu-CIAA firmware FatFS multidisk layout, optimizations and       */
/* driver upgrade to FatFs 013b by Santiago Germino, 2018                */
/* (royconejo@gmail.com).                                                */
/*-----------------------------------------------------------------------*/

#ifndef __FSSDC_H_
#define __FSSDC_H_


#include "board.h"      // LPCOpen board support
#include "diskio.h"		// FatFs lower layer API


/*==================[cplusplus]==============================================*/

#ifdef __cplusplus
extern "C" {
#endif

enum FSSDC_CardStatus
{
    FSSDC_CardStatus_Error = 0,
    FSSDC_CardStatus_Removed,
    FSSDC_CardStatus_Inserted,          // SPI @ Slow speed. 74 or more pulses.
    FSSDC_CardStatus_NativeMode,        // Begin procedures to switch to SPI.
    FSSDC_CardStatus_Initializing,      // Switched to SPI mode and initializing...
    FSSDC_CardStatus_Ready,             // Card initialized and ready, SPI @ Fast speed.
};


typedef void (* FSSDC_StatusUpdateCallback_Func) (enum FSSDC_CardStatus cardStatus);


void        FSSDC_InitSPI                   ();
uint32_t    FSSDC_GetSlowClock              ();
uint32_t    FSSDC_GetFastClock              ();
void        FSSDC_SetSlowClock              (uint32_t hz);
void        FSSDC_SetFastClock              (uint32_t hz);
enum FSSDC_CardStatus
            FSSDC_CardStatus                ();
void        FSSDC_SetStatusUpdateCallback   (FSSDC_StatusUpdateCallback_Func func);
void        FSSDC_Update                    ();

DSTATUS     FSSDC_FatFs_DiskStatus          ();
DSTATUS     FSSDC_FatFs_DiskInitialize      ();
DRESULT     FSSDC_FatFs_DiskRead            (BYTE *buff, DWORD sector, UINT count);
#if !FF_FS_READONLY
DRESULT     FSSDC_FatFs_DiskWrite           (const BYTE *buff, DWORD sector, UINT count);
#endif
DRESULT     FSSDC_FatFs_DiskIoCtl           (BYTE cmd, void *buff);



/*==================[cplusplus]==============================================*/

#ifdef __cplusplus
}
#endif


#endif /* __FSSDC_H_ */
