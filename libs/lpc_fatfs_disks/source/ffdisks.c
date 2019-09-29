/*
    FatFS LPCOpen SD & USB Disk support
    Copyright 2018 Santiago Germino (royconejo@gmail.com)

    Redistribution and use in source and binary forms, with or without
    modification, are permitted provided that the following conditions are met:

    1.  Redistributions of source code must retain the above copyright notice,
        this list of conditions and the following disclaimer.

    2.  Redistributions in binary form must reproduce the above copyright
        notice, this list of conditions and the following disclaimer in the
        documentation and/or other materials provided with the distribution.

    3.  Neither the name of the copyright holder nor the names of its
        contributors may be used to endorse or promote products derived from
        this software without specific prior written permission.

    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
    AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
    IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
    ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
    LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
    CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
    SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
    INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
    CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
    ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
    POSSIBILITY OF SUCH DAMAGE.

    ----------------------------------------------------------------------------
    This DiskIO supports two devices tailored specifically to the Edu-CIAA 
    board:
    
    1) SD/MMC card on SPI mode.
    2) USB Mass Storage on core 0.

    You'll ALWAYS need to mount and open files using the corresponding drive 
    letters "SDC:" or "USB:".
    for example: 
        f_mount ("USB:", 1);
        f_open (&FIL, "USB:/dir/subdir/afile.ext", FA_READ);
        
    Remember to successfully mount the device before accessing its contents!
    ----------------------------------------------------------------------------
*/

#include "ff.h"
#include "fssdc.h"
#ifdef LPCUSBLIB_HOST_MASS_STORAGE
#include "fsusb.h"      // FSUSB_* abstraction layer
#endif


#define FATFS_DISK_DRIVE_FSSDC    0
#define FATFS_DISK_DRIVE_FSUSB    1


// Definitions required by FatFs according to ffconf.h
#if !FF_FS_READONLY && !FF_FS_NORTC
DWORD get_fattime (void)
{
	/* Get local time */
    RTC_TIME_T rtcTime;
	Chip_RTC_GetFullTime (LPC_RTC, &rtcTime);

	/* Pack date and time into a DWORD variable */
	return (  (DWORD) (rtcTime.time[RTC_TIMETYPE_YEAR] - 1980)  << 25)
		   | ((DWORD)  rtcTime.time[RTC_TIMETYPE_MONTH]         << 21)
		   | ((DWORD)  rtcTime.time[RTC_TIMETYPE_DAYOFMONTH]    << 16)
		   | ((DWORD)  rtcTime.time[RTC_TIMETYPE_HOUR]          << 11)
		   | ((DWORD)  rtcTime.time[RTC_TIMETYPE_MINUTE]        << 5)
		   | ((DWORD)  rtcTime.time[RTC_TIMETYPE_SECOND]        >> 1);
}
#endif

#if FF_MULTI_PARTITION
PARTITION VolToPart[FF_VOLUMES] = 
{
    {0, 1},             // "0:" ==> Physical drive 0, 1st partition (MMC:)
    {1, 1},             // "1:" ==> Physical drive 1, 1st partition (USB:)
};
#endif

// Must be called once every 10 ms ideally from PendSV, not SysTickHandler!
void disk_timerproc (void)
{
    FSSDC_Update ();
    #ifdef LPCUSBLIB_HOST_MASS_STORAGE
    FSUSB_Update ();
    #endif
}


DSTATUS disk_status (
	BYTE pdrv           /* Physical drive nmuber to identify the drive */
)
{    
    switch (pdrv)
    {
        case FATFS_DISK_DRIVE_FSSDC:
            return FSSDC_FatFs_DiskStatus ();

        case FATFS_DISK_DRIVE_FSUSB:
            #ifdef LPCUSBLIB_HOST_MASS_STORAGE
            return FSUSB_FatFs_DiskStatus ();
            #else
            break;
            #endif
    }

    return STA_NOINIT;
}


DSTATUS disk_initialize (
	BYTE pdrv           /* Physical drive nmuber to identify the drive */
)
{
    switch (pdrv)
    {
        case FATFS_DISK_DRIVE_FSSDC:
            return FSSDC_FatFs_DiskInitialize ();

        case FATFS_DISK_DRIVE_FSUSB:
            #ifdef LPCUSBLIB_HOST_MASS_STORAGE
            return FSUSB_FatFs_DiskInitialize ();
            #else
            break;
            #endif
    }
    
    return STA_NOINIT;
}


DRESULT disk_read (
	BYTE pdrv,          /* Physical drive nmuber to identify the drive */
	BYTE *buff,         /* Data buffer to store read data */
	DWORD sector,       /* Start sector in LBA */
	UINT count          /* Number of sectors to read */
)
{
	if (!buff || !count) 
    {
		return RES_PARERR;
	}
    
    switch (pdrv)
    {
        case FATFS_DISK_DRIVE_FSSDC:
            return FSSDC_FatFs_DiskRead (buff, sector, count);

        case FATFS_DISK_DRIVE_FSUSB:
            #ifdef LPCUSBLIB_HOST_MASS_STORAGE
            return FSUSB_FatFs_DiskRead (buff, sector, count);
            #else
            break;
            #endif
    }
    
    return RES_PARERR;
}


DRESULT disk_write (
	BYTE pdrv,			/* Physical drive nmuber to identify the drive */
	const BYTE *buff,	/* Data to be written */
	DWORD sector,		/* Start sector in LBA */
	UINT count			/* Number of sectors to write */
)
{
	if (!buff || !count)
    {
		return RES_PARERR;
	}
    
    switch (pdrv)
    {
        case FATFS_DISK_DRIVE_FSSDC:
            return FSSDC_FatFs_DiskWrite (buff, sector, count);

        case FATFS_DISK_DRIVE_FSUSB:
            #ifdef LPCUSBLIB_HOST_MASS_STORAGE
            return FSUSB_FatFs_DiskWrite (buff, sector, count);
            #else
            break;
            #endif
    }
    
	return RES_PARERR;
}


DRESULT disk_ioctl (
	BYTE pdrv,          /* Physical drive nmuber (0..) */
	BYTE cmd,           /* Control code */
	void *buff          /* Buffer to send/receive control data */
)
{   
    switch (pdrv)
    {
        case FATFS_DISK_DRIVE_FSSDC:
            return FSSDC_FatFs_DiskIoCtl (cmd, buff);

        case FATFS_DISK_DRIVE_FSUSB:
            #ifdef LPCUSBLIB_HOST_MASS_STORAGE
            return FSUSB_FatFs_DiskIoCtl (cmd, buff);
            #else
            break;
            #endif
    }

	return RES_PARERR;
}
