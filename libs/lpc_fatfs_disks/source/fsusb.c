/*
 * @brief LPCUSBLib API for MassStorage Disk and ChaN FAT FS drive functions.
 *
 * @note
 * Copyright(C) Santiago Germino, 2018
 * Copyright(C) NXP Semiconductors, 2012
 * Copyright(C) Dean Camera, 2011, 2012
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
 * Refinement of the abstraction layer made by Santiago Germino.
 * Please look at this section on fsusb.h for details.
 */


#include "fsusb.h"


/** LPCUSBlib Mass Storage Class driver interface configuration and state information. This structure is
 *  passed to all Mass Storage Class driver functions, so that multiple instances of the same class
 *  within a device can be differentiated from one another.
 */
static USB_ClassInfo_MS_Host_t USB0MS_Interface = 
{
	.Config = 
    {
		.DataINPipeNumber       = 1,
		.DataINPipeDoubleBank   = false,

		.DataOUTPipeNumber      = 2,
		.DataOUTPipeDoubleBank  = false,
         // This and corenum (USB0 or USB1) will be always zero (USB0) on Edu-CIAA.
		.PortNumber = 0,
	},
};


static SCSI_Capacity_t                  g_diskCapacity;
static FSUSB_StatusUpdateCallback_Func  g_statusUpdateCallback = NULL;
static enum FSUSB_HostStatus            g_hostStatus = FSUSB_HostStatus_DeviceDetached;


static DISK_HANDLE_T    *hDisk      = NULL;
static volatile DSTATUS g_diskStats = STA_NOINIT;


static void newHostStatus (enum FSUSB_HostStatus hostStatus,
                           uint32_t coreNum, 
                           uint32_t errorCode,
                           uint32_t subErrorCode)
{
    g_hostStatus = hostStatus;
    if (g_statusUpdateCallback)
    {
        g_statusUpdateCallback (g_hostStatus, coreNum, errorCode, subErrorCode);
    }
}


void FSUSB_InitHost ()
{    
    DISK_HANDLE_T *hDisk = FSUSB_DiskInit ();   
    
    // ASSERT (hDisk->Config.PortNumber == 0) 
    Chip_USB0_Init();      
    
    g_hostStatus = FSUSB_HostStatus_DeviceDetached;
    USB_Init (hDisk->Config.PortNumber, USB_MODE_Host);
}


enum FSUSB_HostStatus FSUSB_HostStatus ()
{
    return g_hostStatus;
}


void FSUSB_UnconfigureDevice (DISK_HANDLE_T *hDisk)
{
    USB_Host_SetDeviceConfiguration (hDisk->Config.PortNumber, 0);
}


void FSUSB_SetStatusUpdateCallback (FSUSB_StatusUpdateCallback_Func func)
{
    g_statusUpdateCallback = func;
}


static DRESULT updateUnconfDiskHostState (DISK_HANDLE_T *hDisk)
{
    if (USB_HostState[hDisk->Config.PortNumber] != HOST_STATE_Configured)
    {
        MS_Host_USBTask (hDisk);
        USB_USBTask     (hDisk->Config.PortNumber, USB_MODE_Host);
    }
    
    if (USB_HostState[hDisk->Config.PortNumber] == HOST_STATE_Configured)
    {
        newHostStatus (FSUSB_HostStatus_DeviceConfigured, hDisk->Config.PortNumber, 0, 0);
        return RES_OK;
    }
    
    return RES_NOTRDY;
}


static DRESULT checkDiskReady (DISK_HANDLE_T *hDisk)
{
    Board_UARTPutSTR ("FSUSB: Checkig for disk ready...\r\n");
    
    if (!hDisk)
    {
        return RES_PARERR;
    }
    
    uint8_t errorCode;
    if ((errorCode = MS_Host_TestUnitReady (hDisk, 0)))
    {
        /* Check if an error other than a logical command error (device busy) received */        
        if (errorCode == MS_ERROR_LOGICAL_CMD_FAILED)
        {
            return RES_NOTRDY;
        }
        
        Board_UARTPutSTR ("FSUSB: Device error.\r\n");
        FSUSB_UnconfigureDevice (hDisk);
        
        newHostStatus (FSUSB_HostStatus_DeviceTestUnitReadyError,
                       hDisk->Config.PortNumber, errorCode, 0);
        return RES_ERROR;
    }
    
    if ((errorCode = MS_Host_ReadDeviceCapacity (hDisk, 0, &g_diskCapacity)))
    {
		Board_UARTPutSTR ("FSUSB: Error retrieving device capacity.\r\n");
        FSUSB_UnconfigureDevice (hDisk);
        
        newHostStatus (FSUSB_HostStatus_DeviceCapacityError,
                       hDisk->Config.PortNumber, errorCode, 0);
        return RES_ERROR;
 	}

    Board_UARTPutSTR ("FSUSB: Disk ready!\r\n");    
    
    char debugBuf[32];
    sprintf (debugBuf, "FSUSB: %u", (unsigned int) g_diskCapacity.Blocks);
    Board_UARTPutSTR (debugBuf);
    Board_UARTPutSTR (" blocks of ");
    sprintf (debugBuf, "%u", (unsigned int) g_diskCapacity.BlockSize);
    Board_UARTPutSTR (debugBuf);
    Board_UARTPutSTR (" bytes.\r\n");
    
    newHostStatus (FSUSB_HostStatus_DeviceReady, 
                   hDisk->Config.PortNumber, 0, 0);
    return RES_OK;
}


DRESULT FSUSB_Update ()
{
    DISK_HANDLE_T   *hDisk = FSUSB_DiskInit ();
    DRESULT         dr;
    
    if (g_hostStatus == FSUSB_HostStatus_DeviceAttached)
    {
        dr = updateUnconfDiskHostState (hDisk);
    }
    
    if (g_hostStatus == FSUSB_HostStatus_DeviceConfigured)
    {
        dr = checkDiskReady (hDisk);
    }
    
    return dr;
}


// For the return value to have any sense al all, this function should take the
// "pdrv" of the accessed device and return the corresponding interface pointer.
DISK_HANDLE_T *FSUSB_DiskInit (void)
{
	return &USB0MS_Interface;
}


#ifdef FSUSB_BUSY_WAITING_API
/* Wait for disk to be inserted */
int FSUSB_DiskInsertWait (DISK_HANDLE_T *hDisk)
{
	while (USB_HostState[hDisk->Config.PortNumber] != HOST_STATE_Configured) {
		MS_Host_USBTask(hDisk);
		USB_USBTask(hDisk->Config.PortNumber, USB_MODE_Host);
	}
	return 1;
}


/* Disk acquire function that waits for disk to be ready */
int FSUSB_DiskAcquire (DISK_HANDLE_T *hDisk)
{
    char debugBuf[64];
        
	Board_UARTPutSTR ("FSUSB: Waiting for ready...\r\n");
	for (;; ) {
		uint8_t ErrorCode = MS_Host_TestUnitReady(hDisk, 0);

		if (!(ErrorCode)) {
			break;
		}

		/* Check if an error other than a logical command error (device busy) received */
		if (ErrorCode != MS_ERROR_LOGICAL_CMD_FAILED) {
			Board_UARTPutSTR ("FSUSB: Failed\r\n");
			USB_Host_SetDeviceConfiguration(hDisk->Config.PortNumber, 0);
			return 0;
		}
	}
	Board_UARTPutSTR ("FSUSB: Done.\r\n");

	if (MS_Host_ReadDeviceCapacity(hDisk, 0, &g_diskCapacity)) {
		Board_UARTPutSTR ("FSUSB: Error retrieving device capacity.\r\n");
		USB_Host_SetDeviceConfiguration(hDisk->Config.PortNumber, 0);
		return 0;
	}

    sprintf (debugBuf, "FSUSB: %u", (unsigned int) g_diskCapacity.Blocks);
    Board_UARTPutSTR (debugBuf);
	Board_UARTPutSTR (" blocks of ");
    sprintf (debugBuf, "%u", (unsigned int) g_diskCapacity.BlockSize);
    Board_UARTPutSTR (debugBuf);
    Board_UARTPutSTR (" bytes.\r\n");
	return 1;
}
#endif


uint32_t FSUSB_DiskGetSectorCnt (DISK_HANDLE_T *hDisk)
{
	return g_diskCapacity.Blocks;
}


uint32_t FSUSB_DiskGetSectorSz (DISK_HANDLE_T *hDisk)
{
	return g_diskCapacity.BlockSize;
}


DRESULT FSUSB_DiskReadSectors (DISK_HANDLE_T *hDisk, void *buff, uint32_t secStart, uint32_t numSec)
{
	if (MS_Host_ReadDeviceBlocks (hDisk, 0, secStart, numSec, g_diskCapacity.BlockSize, buff))
    {
		Board_UARTPutSTR ("FSUSB: Error reading device block.\r\n");
		return RES_ERROR;
	}
    
	return RES_OK;
}


DRESULT FSUSB_DiskWriteSectors (DISK_HANDLE_T *hDisk, void *buff, uint32_t secStart, uint32_t numSec)
{
	if (MS_Host_WriteDeviceBlocks(hDisk, 0, secStart, numSec, g_diskCapacity.BlockSize, buff))
    {
		Board_UARTPutSTR ("FSUSB: Error writing device block.\r\n");
		return RES_ERROR;
	}
    
	return RES_OK;
}


// FIXME: EN SERIO????? :(
DRESULT FSUSB_DiskReadyWait(DISK_HANDLE_T *hDisk, int tout)
{
	volatile int i = tout * 100;
	while (i--) {	/* Just delay */
	}
	return RES_OK;
}


/** Event handler for the USB_DeviceAttached event. This indicates that a device has been attached to the host, and
 *  starts the library USB task to begin the enumeration and USB management process.
 */
void EVENT_USB_Host_DeviceAttached (const uint8_t corenum)
{   
	Board_UARTPutSTR ("FSUSB: Device Attached on port\r\n");
    newHostStatus (FSUSB_HostStatus_DeviceAttached, corenum, 0, 0);
}

/** Event handler for the USB_DeviceUnattached event. This indicates that a device has been removed from the host, and
 *  stops the library USB task management process.
 */
void EVENT_USB_Host_DeviceUnattached (const uint8_t corenum)
{
	Board_UARTPutSTR ("FSUSB: Device Unattached on port\r\n");
    newHostStatus (FSUSB_HostStatus_DeviceDetached, corenum, 0, 0);
}

/** Event handler for the USB_DeviceEnumerationComplete event. This indicates that a device has been successfully
 *  enumerated by the host and is now ready to be used by the application.
 */
void EVENT_USB_Host_DeviceEnumerationComplete (const uint8_t corenum)
{
	uint16_t ConfigDescriptorSize;
	uint8_t  ConfigDescriptorData[512];
    char debugBuf[32];
    
    enum FSUSB_DeviceEnumerationError
                errorCode           = FSUSB_HostDeviceEnumError_None;
    uint8_t     subErrorCode        = 0;
    bool        deviceConfigured    = false;

    do 
    {
        if ((subErrorCode = USB_Host_GetDeviceConfigDescriptor (corenum, 1, 
                                                                &ConfigDescriptorSize, 
                                                                ConfigDescriptorData,
                                                                sizeof(ConfigDescriptorData))) 
            != HOST_GETCONFIG_Successful)
        {
            errorCode = FSUSB_HostDeviceEnumError_ConfigDescriptor;
            Board_UARTPutSTR ("FSUSB: Error Retrieving Configuration Descriptor.\r\n");
            break;
        }
    
        // FIXME: que carajo hacen aca?? si ya habian decidido port 0?
        // USB0MS_Interface.Config.PortNumber = corenum;
        if ((subErrorCode = MS_Host_ConfigurePipes (&USB0MS_Interface, 
                                                    ConfigDescriptorSize, 
                                                    ConfigDescriptorData))
            != MS_ENUMERROR_NoError)
        {
            errorCode = FSUSB_HostDeviceEnumError_ConfigurePipes;
            Board_UARTPutSTR ("FSUSB: Attached Device Not a Valid Mass Storage Device.\r\n");
            break;
        }
    
        if ((subErrorCode = USB_Host_SetDeviceConfiguration (USB0MS_Interface.Config.PortNumber,
                                                             1))
            != HOST_SENDCONTROL_Successful)
        {
            errorCode = FSUSB_HostDeviceEnumError_SetDeviceConfiguration;
            Board_UARTPutSTR ("FSUSB: Error Setting Device Configuration.\r\n");
            break;
        }
        deviceConfigured = true;
    
        uint8_t MaxLUNIndex;
        if ((subErrorCode = MS_Host_GetMaxLUN (&USB0MS_Interface, 
                                               &MaxLUNIndex)))
        {
            errorCode = FSUSB_HostDeviceEnumError_GetMaxLUN;
            Board_UARTPutSTR ("FSUSB: Error retrieving max LUN index.\r\n");
            break;
        }
    
        Board_UARTPutSTR ("FSUSB: Total LUNs: ");
        sprintf (debugBuf, "%d", (MaxLUNIndex + 1));
        Board_UARTPutSTR (debugBuf);
        Board_UARTPutSTR (" - Using first LUN in device.\r\n");
    
        if ((subErrorCode = MS_Host_ResetMSInterface (&USB0MS_Interface)))
        {
            errorCode = FSUSB_HostDeviceEnumError_ResetMSInterface;
            Board_UARTPutSTR ("FSUSB: Error resetting Mass Storage interface.\r\n");
            break;
        }
    
        SCSI_Request_Sense_Response_t SenseData;
        if ((subErrorCode = MS_Host_RequestSense (&USB0MS_Interface, 
                                                  0, &SenseData))
            != 0)
        {
            errorCode = FSUSB_HostDeviceEnumError_RequestSense;
            Board_UARTPutSTR ("FSUSB: Error retrieving device sense.\r\n");
            break;
        }
    
    // 	if (MS_Host_PreventAllowMediumRemoval(&USB0MS_Interface, 0, true)) {
    //      errorCode = agregar error
    // 		DEBUGOUT("Error setting Prevent Device Removal bit.\r\n");
    // 		break;
    // 	}
    
        SCSI_Inquiry_Response_t InquiryData;
        if ((subErrorCode = MS_Host_GetInquiryData(&USB0MS_Interface, 
                                                   0, &InquiryData)))
        {
            errorCode = FSUSB_HostDeviceEnumError_GetInquiryData;
            Board_UARTPutSTR ("FSUSB: Error retrieving device Inquiry data.\r\n");
            break;
        }

        /* DEBUGOUT("Vendor \"%.8s\", Product \"%.16s\"\r\n", InquiryData.VendorID, InquiryData.ProductID); */
        Board_UARTPutSTR ("FSUSB: Mass Storage Device Enumerated.\r\n");
    }
    while (0);
    
    if (errorCode)
    {
        if (deviceConfigured)
        {
            FSUSB_UnconfigureDevice (&USB0MS_Interface);
        }
        
        newHostStatus (FSUSB_HostStatus_DeviceEnumerationFailed, 
                       corenum, (uint8_t)errorCode, subErrorCode);
        return;   
    }
    
    newHostStatus (FSUSB_HostStatus_DeviceEnumerationComplete, corenum, 0, 0);
}


/** Event handler for the USB_DeviceEnumerationFailed event. This indicates that a problem occurred while
 *  enumerating an attached USB device.
 */
void EVENT_USB_Host_DeviceEnumerationFailed (const uint8_t corenum,
											 const uint8_t ErrorCode,
											 const uint8_t SubErrorCode)
{
    char debugBuf[32];
    
	Board_UARTPutSTR ("FSUSB: Dev Enum Error\r\nFSUSB:  -- Error port );");
    sprintf (debugBuf, "%u", corenum);
    Board_UARTPutSTR (debugBuf);                  
    Board_UARTPutSTR ("\r\nFSUSB:  -- Error Code ");
    sprintf (debugBuf, "%u", ErrorCode);
    Board_UARTPutSTR (debugBuf);   
    Board_UARTPutSTR ("\r\nFSUSB:  -- Sub Error Code ");
    sprintf (debugBuf, "%u", SubErrorCode);
    Board_UARTPutSTR (debugBuf); 
    Board_UARTPutSTR ("\r\nFSUSB:  -- In State ");
    sprintf (debugBuf, "%u", USB_HostState[corenum]);
    Board_UARTPutSTR (debugBuf);
    Board_UARTPutSTR ("\r\n");
    
    newHostStatus (FSUSB_HostStatus_DeviceEnumerationFailed, 
                   corenum, ErrorCode, SubErrorCode);
}


/** Event handler for the USB_HostError event. This indicates that a hardware error occurred while in host mode. */
void EVENT_USB_Host_HostError (const uint8_t corenum, const uint8_t ErrorCode)
{
    char debugBuf[32];

    USB_Disable(corenum, USB_MODE_Host);

	Board_UARTPutSTR ("FSUSB: Host Mode Error\r\nFSUSB:  -- Error port ");
    sprintf (debugBuf, "%u", corenum);
    Board_UARTPutSTR (debugBuf);
    Board_UARTPutSTR ("\r\nFSUSB:  -- Error Code ");    
    sprintf (debugBuf, "%u", ErrorCode);
    Board_UARTPutSTR (debugBuf);
    Board_UARTPutSTR ("\r\n");
    
//    Board_UARTPutSTR ("******************* DIE fsusb\r\n"); 
//	for (;; ) {}
    
    newHostStatus (FSUSB_HostStatus_Error, corenum, ErrorCode, 0);
}


DSTATUS FSUSB_FatFs_DiskStatus ()
{
    return g_diskStats;
}


DSTATUS FSUSB_FatFs_DiskInitialize ()
{   
	if (!(g_diskStats & STA_NOINIT)) 
    {
        /* card is already enumerated */
		return g_diskStats;
	}

	/* Initialize the Card Data Strucutre */
	hDisk       = FSUSB_DiskInit ();
	g_diskStats = STA_NOINIT;

#ifdef FSUSB_BUSY_WAITING_API
    /* Wait for card to be inserted */    
	FSUSB_DiskInsertWait (hDisk);
    
	/* Enumerate the card once detected. Note this function may block for a little while. */
	if (!FSUSB_DiskAcquire (hDisk)) {
		Board_UARTPutSTR ("FSUSB: Disk Enumeration failed...\r\n");
		return Stat;
	}
#else 
    if (FSUSB_HostStatus() != FSUSB_HostStatus_DeviceReady)
    {
        Board_UARTPutSTR ("FSUSB: Disk still not ready.\r\n");        
        return g_diskStats;
    }
#endif
    
	g_diskStats &= ~STA_NOINIT;
	return g_diskStats;
}


DRESULT FSUSB_FatFs_DiskRead (BYTE *buff, DWORD sector, UINT count)
{
    if (g_diskStats & STA_NOINIT) 
    {
		return RES_NOTRDY;
	}

    DRESULT dr;
	if ((dr = FSUSB_DiskReadSectors (hDisk, buff, sector, count)) != RES_OK)
    {
        FSUSB_UnconfigureDevice (hDisk);
        g_diskStats = STA_NOINIT;
    }
    
    return dr;
}


DRESULT FSUSB_FatFs_DiskWrite (const BYTE *buff, DWORD sector, UINT count)
{
    if (g_diskStats & STA_NOINIT)
    {
        return RES_NOTRDY;
    }
    
    DRESULT dr;
    if ((dr = FSUSB_DiskWriteSectors (hDisk, (void *) buff, sector, count)) != RES_OK)
    {
        FSUSB_UnconfigureDevice (hDisk);
        g_diskStats = STA_NOINIT;
    }
    
    return dr;
}


DRESULT FSUSB_FatFs_DiskIoCtl (BYTE cmd, void *buff)
{
    if (g_diskStats & STA_NOINIT)
    {
		return RES_NOTRDY;
	}

	switch (cmd) 
    {
        case CTRL_SYNC:	
            /* Make sure that no pending write process */
            return FSUSB_DiskReadyWait (hDisk, 50);
    
        case GET_SECTOR_COUNT:
            /* Get number of sectors on the disk (DWORD) */
            *(DWORD *) buff = FSUSB_DiskGetSectorCnt (hDisk);
            return RES_OK;
   
        case GET_SECTOR_SIZE:
            /* Get R/W sector size (WORD) */
            *(WORD *) buff = FSUSB_DiskGetSectorSz (hDisk);
            return RES_OK;
    
        case GET_BLOCK_SIZE:
            /* Get erase block size in unit of sector (DWORD) */
            *(DWORD *) buff = FSUSB_DiskGetBlockSz (hDisk);
            return RES_OK;
	}

    return RES_PARERR;
}
