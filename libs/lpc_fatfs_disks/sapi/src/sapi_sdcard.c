/*
    FSSDC API wrapper for use with FatFs
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

#include "sapi_sdcard.h"
#include <string.h>


static const char *g_sdcardDriveName = "SDC:";
static sdcard_t *g_sdcard = NULL;


static void statusUpdateCallback( enum FSSDC_CardStatus cardStatus )
{    
    switch( cardStatus )
    {
        case FSSDC_CardStatus_Removed:
            g_sdcard->status = SDCARD_Status_Removed;
            sdcardMount( false );
            break;
            
        case FSSDC_CardStatus_Inserted:
            g_sdcard->status = SDCARD_Status_Inserted;
            break;
            
        case FSSDC_CardStatus_NativeMode:
            g_sdcard->status = SDCARD_Status_NativeMode;
            break;
            
        case FSSDC_CardStatus_Initializing:
            g_sdcard->status = SDCARD_Status_Initializing;
            break;
            
        case FSSDC_CardStatus_Ready:
            g_sdcard->status = SDCARD_Status_ReadyUnmounted;
            // Automount
            if (!sdcardMount( true ))
            {
                Board_UARTPutSTR ("sapi_sdcard: Automount failed!\r\n");
            }
            break;

        case FSSDC_CardStatus_Error:
            g_sdcard->status = SDCARD_Status_Error;
            break;
    }     
}


bool sdcardInit( sdcard_t *u )
{
    if( !u || g_sdcard )
    {
        return false;
    }
    
    memset( u, 0, sizeof( sdcard_t ));
    g_sdcard = u;
    
    FSSDC_SetStatusUpdateCallback( statusUpdateCallback );
    FSSDC_InitSPI( );
    return true;
}


void sdcardUpdate( void )
{
    if( !g_sdcard )
    {
        return;
    }
    
    FSSDC_Update(  );
}


sdcardStatus_t sdcardStatus( void )
{
    if( !g_sdcard )
    {
        return SDCARD_Status_Error;
    }
    
    return g_sdcard->status;
}


const char *sdcardDriveName( void )
{
    return g_sdcardDriveName;
}


bool sdcardMount( bool mount )
{
    if( !g_sdcard )
    {
        return false;
    }
    
    if( mount && g_sdcard->status != SDCARD_Status_ReadyUnmounted )
    {
        return false;
    }
    
    if(( g_sdcard->result = f_mount( mount? &g_sdcard->fatFs : NULL, 
                                    g_sdcardDriveName, 1 )) != FR_OK )
    {
        return false;
    }
    
    if( mount )
    {
        g_sdcard->status = SDCARD_Status_ReadyMounted;
    }
    else if( g_sdcard->status == SDCARD_Status_ReadyMounted )
    {
        g_sdcard->status = SDCARD_Status_ReadyUnmounted;
    }
    
    return true;
}

