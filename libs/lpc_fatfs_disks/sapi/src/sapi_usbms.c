/*
    FSUSB API wrapper for use with FatFs
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
*/

#include "sapi_usbms.h"
#include <string.h>


static const char *g_usbmsDriveName = "USB:";
static usbms_t *g_usbms = NULL;


static void statusUpdateCallback( enum FSUSB_HostStatus hostStatus, uint32_t coreNum, 
                                  uint32_t errorCode, uint32_t subErrorCode )
{
    switch( hostStatus )
    {
        case FSUSB_HostStatus_DeviceDetached:
            g_usbms->status = USBMS_Status_StorageDetached;
            usbmsMount( false );
            break;
            
        case FSUSB_HostStatus_DeviceAttached:
            g_usbms->status = USBMS_Status_StorageAttached;
            break;
            
        case FSUSB_HostStatus_DeviceConfigured:
            g_usbms->status = USBMS_Status_StorageConfigured;
            break;
            
        case FSUSB_HostStatus_DeviceEnumerationComplete:
            g_usbms->status = USBMS_Status_StorageEnumerated;
            break;
            
        case FSUSB_HostStatus_DeviceReady:
            g_usbms->status = USBMS_Status_StorageReadyUnmounted;
            // Automount
            if (!usbmsMount( true ))
            {
                Board_UARTPutSTR ("sapi_usbms: Automount failed!\r\n");
            }
            break;

        case FSUSB_HostStatus_DeviceEnumerationFailed:
        case FSUSB_HostStatus_DeviceTestUnitReadyError:
        case FSUSB_HostStatus_DeviceCapacityError: 
        case FSUSB_HostStatus_Error:    
            g_usbms->status = USBMS_Status_StorageError;
            break;
    }     
}


bool usbmsInit( usbms_t *u )
{
    if( !u || g_usbms )
    {
        return false;
    }
    
    memset( u, 0, sizeof( usbms_t ) );
    g_usbms = u;
    
    FSUSB_SetStatusUpdateCallback( statusUpdateCallback );
    FSUSB_InitHost( );
    return true;
}


void usbmsUpdate( void )
{
    if( !g_usbms )
    {
        return;
    }
    
    FSUSB_Update(  );
}


usbmsStatus_t usbmsStatus( void )
{
    if( !g_usbms )
    {
        return USBMS_Status_StorageError;
    }
    
    return g_usbms->status;
}


const char *usbmsDriveName( void )
{
    return g_usbmsDriveName;
}


bool usbmsMount( bool mount )
{
    if( !g_usbms )
    {
        return false;
    }
    
    if( mount && g_usbms->status != USBMS_Status_StorageReadyUnmounted )
    {
        return false;
    }
    
    if(( g_usbms->result = f_mount( mount? &g_usbms->fatFs : NULL,
                                    g_usbmsDriveName, 1 )) != FR_OK )
    {
        return false;
    }
    
    if( mount )
    {
        g_usbms->status = USBMS_Status_StorageReadyMounted;
    }
    else if( g_usbms->status == USBMS_Status_StorageReadyMounted )
    {
        g_usbms->status = USBMS_Status_StorageReadyUnmounted;
    }
    
    return true;
}

