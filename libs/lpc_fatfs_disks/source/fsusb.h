/*
 * @brief LPCUSBLib API for MassStorage Disk and ChaN FAT FS drive functions.
 *
 * @note
 * Copyright(C) Santiago Germino, 2018
 * Copyright(C) NXP Semiconductors, 2012
 * All rights reserved.
 *
 * @par
 * Software that is described herein is for illustrative purposes only
 * which provides customers with programming information regarding the
 * LPC products.  This software is supplied "AS IS" without any warranties of
 * any kind, and NXP Semiconductors and its licensor disclaim any and
 * all warranties, express or implied, including all implied warranties of
 * merchantability, fitness for a particular purpose and non-infringement of
 * intellectual property rights.  NXP Semiconductors assumes no responsibility
 * or liability for the use of the software, conveys no license or rights under any
 * patent, copyright, mask work right, or any other intellectual property rights in
 * or to any products. NXP Semiconductors reserves the right to make changes
 * in the software without notification. NXP Semiconductors also makes no
 * representation or warranty that such application will be suitable for the
 * specified use without further testing or modification.
 *
 * @par
 * Permission to use, copy, modify, and distribute this software and its
 * documentation is hereby granted, under NXP Semiconductors' and its
 * licensor's relevant copyrights in the software, without fee, provided that it
 * is used in conjunction with NXP Semiconductors microcontrollers.  This
 * copyright, permission, and disclaimer notice must appear in all copies of
 * this code.
 * 
 * @par
 * Changelog
 * 2018/08 sgermino (royconejo@gmail.com): 
 *          Refinement of the example abstraction layer as a stand-alone library
 *          to be used on real applications. Host status callback added. Busy
 *          loops while performing USB device configuration has been removed.
 *          Note that you must call FSUSB_Update() once in a while in your main
 *          loop or recurring task. Define FSUSB_BUSY_WAITING_API to use the 
 *          old implementation instead.
 * 
 *          Also note that in the original code there are still decisions to
 *          take and work to be done to support multiple device instances. That
 *          is, cleaning the current nonsense with DISK_HANDLE_T *hDisk 
 *          everywhere. Some issues to solve: which instance to process on the 
 *          EVENT_* handlers (now hardcoded to the single global instance) and 
 *          the fact that there is only a single, global SCSI_Capacity_t.
 */

#ifndef __FSUSB_H_
#define __FSUSB_H_


#include "board.h"      // LPCOpen board support
#include "diskio.h"		// FatFs lower layer API
#include "USB.h"        // LPCUSBLib host mode required


/**
 * @ingroup Mass_Storage_Host
 * @{
 */


enum FSUSB_HostStatus
{
    FSUSB_HostStatus_Error = 0,
    FSUSB_HostStatus_DeviceDetached,
    FSUSB_HostStatus_DeviceAttached,
    FSUSB_HostStatus_DeviceConfigured,
    FSUSB_HostStatus_DeviceEnumerationComplete,
    FSUSB_HostStatus_DeviceEnumerationFailed,
    FSUSB_HostStatus_DeviceTestUnitReadyError,
    FSUSB_HostStatus_DeviceCapacityError,
    FSUSB_HostStatus_DeviceReady
};


enum FSUSB_DeviceEnumerationError
{
    FSUSB_HostDeviceEnumError_None = 0,
    FSUSB_HostDeviceEnumError_ConfigDescriptor,
    FSUSB_HostDeviceEnumError_ConfigurePipes,
    FSUSB_HostDeviceEnumError_SetDeviceConfiguration,
    FSUSB_HostDeviceEnumError_GetMaxLUN,
    FSUSB_HostDeviceEnumError_ResetMSInterface,
    FSUSB_HostDeviceEnumError_RequestSense,
    FSUSB_HostDeviceEnumError_GetInquiryData  
};


typedef USB_ClassInfo_MS_Host_t DISK_HANDLE_T;
typedef void (* FSUSB_StatusUpdateCallback_Func) (enum FSUSB_HostStatus hostStatus,
                                                  uint32_t coreNum, 
                                                  uint32_t errorCode,
                                                  uint32_t subErrorCode);

// New API functions
void    FSUSB_InitHost ();
enum FSUSB_HostStatus
        FSUSB_HostStatus ();
DRESULT FSUSB_Update ();
void    FSUSB_UnconfigureDevice (DISK_HANDLE_T *hDisk);
void    FSUSB_SetStatusUpdateCallback (FSUSB_StatusUpdateCallback_Func func);

// Interface to FatFs DiskIO
DSTATUS FSUSB_FatFs_DiskStatus ();
DSTATUS FSUSB_FatFs_DiskInitialize ();
DRESULT FSUSB_FatFs_DiskRead (BYTE *buff, DWORD sector, UINT count);
DRESULT FSUSB_FatFs_DiskWrite (const BYTE *buff, DWORD sector, UINT count);
DRESULT FSUSB_FatFs_DiskIoCtl (BYTE cmd, void *buff);

/**
 * @brief	Initialize the disk data structure
 * @return	Pointer to the disk data strucuture
 */
DISK_HANDLE_T *FSUSB_DiskInit(void);


#ifdef FSUSB_BUSY_WAITING_API
/**
 * @brief	Enumerate and get the disk connected
 * @param	hDisk	: Handle to the USB Disk data structure
 * @return	1 on success, 0 on failure
 */
int FSUSB_DiskAcquire(DISK_HANDLE_T *hDisk);

/**
 * @brief	Wait for the USB device to be inserted and ready
 * @param	hDisk	: Handle to USB Disk
 * @return	1 on success and 0 on failure
 */
int FSUSB_DiskInsertWait(DISK_HANDLE_T *hDisk);
#endif


/**
 * @brief	Get the number of sectors in the disk
 * @param	hDisk	: Handle to USB disk
 * @return	Number of sectors in the USB disk
 */
uint32_t FSUSB_DiskGetSectorCnt(DISK_HANDLE_T *hDisk);


/**
 * @brief	Get number of bytes in a sector of the disk
 * @param	hDisk	: Handle to the USB Disk
 * @return	Number of bytes in a single sector of the disk
 */
uint32_t FSUSB_DiskGetSectorSz(DISK_HANDLE_T *hDisk);


/**
 * @brief	Reads sectors from USB mass storage disk
 * @param	hDisk		: Handle to the USB disk
 * @param	buff		: Pointer to memory where the data read will be stored
 * @param	secStart	: Starting sector from which data be read
 * @param	numSec		: Number of sectors to be read
 * @return	RES_OK on success and RES_ERROR on failure
 */
DRESULT FSUSB_DiskReadSectors(DISK_HANDLE_T *hDisk, void *buff, uint32_t secStart, uint32_t numSec);


/**
 * @brief	Write data to USB device sectors
 * @param	hDisk		: Handle to the USB disk
 * @param	buff		: Pointer to memory where the data to be written, be stored
 * @param	secStart	: Starting sector from which data be written
 * @param	numSec		: Number of sectors to write
 * @return	RES_OK on success and RES_ERROR on failure
 */
DRESULT FSUSB_DiskWriteSectors(DISK_HANDLE_T *hDisk, void *buff, uint32_t secStart, uint32_t numSec);


/**
 * @brief	Wait for the USB Disk to be ready
 * @param	hDisk	: Pointer to Disk Handle
 * @param	tout	: Time to wait, in milliseconds
 * @return	RES_ERROR when operation failed RES_OK when successfully completed
 */
DRESULT FSUSB_DiskReadyWait(DISK_HANDLE_T *hDisk, int tout);


/* Erase block size fixed to 4K */
#define FSUSB_DiskGetBlockSz(hDisk)         (4 * 1024)

/**
 * @}
 */

#endif /* __FSUSB_H_ */
