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

#ifndef SAPI_SDCARD_H_
#define SAPI_SDCARD_H_

#include "fssdc.h"
#include "ff.h"

#ifdef __cplusplus
extern "C" {
#endif


typedef enum {
    SDCARD_Status_Removed = 0,
    SDCARD_Status_Inserted,
    SDCARD_Status_NativeMode,
    SDCARD_Status_Initializing,
    SDCARD_Status_ReadyUnmounted,
    SDCARD_Status_ReadyMounted,    
    SDCARD_Status_Error,
} sdcardStatus_t;


typedef struct {
    FATFS fatFs;
    FRESULT result;
    sdcardStatus_t status;
} sdcard_t;


bool sdcardInit( sdcard_t *u );
void sdcardUpdate( void );
sdcardStatus_t sdcardStatus( void );
const char * sdcardDriveName( void );
bool sdcardMount( bool mount );


#ifdef __cplusplus
}
#endif

#endif /* SAPI_SDCARD_H_ */
