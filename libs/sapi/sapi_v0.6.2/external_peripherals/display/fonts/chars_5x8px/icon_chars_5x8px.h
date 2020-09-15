/*=============================================================================
 * Copyright (c) 2019, Eric Pernia <ericpernia@gmail.com>
 * All rights reserved.
 * License: bsd-3-clause (see LICENSE.txt)
 * Date: 2019/08/26
 *===========================================================================*/

#ifndef _DISPLAY_ICONS_CHARS_5X8_PX_H_
#define _DISPLAY_ICONS_CHARS_5X8_PX_H_

/*==================[inclusiones]============================================*/

#include "sapi.h"

/*==================[c++]====================================================*/

#ifdef __cplusplus
extern "C" {
#endif
   
/*===========================================================================*/

// Custom char variables ------------------------------------------------------

// Carita sonriendo
extern const uint8_t smileBitmap[8];
extern lcdCustomChar_t smileChar;

// Parlante encendido
extern const uint8_t speakerOnBitmap[8];
extern lcdCustomChar_t speakerOnChar;

// Parlante apagado
extern const uint8_t speakerOffBitmap[8];
extern lcdCustomChar_t speakerOffChar;

// Termometro o tubo de ensayo
extern const uint8_t thermometerBitmap[8];
extern lcdCustomChar_t thermometerChar;

// Gota de agua
extern const uint8_t dropBitmap[8];
extern lcdCustomChar_t dropChar;

// Viento
extern const uint8_t windBitmap[8];
extern lcdCustomChar_t windChar;

// Custom char array ----------------------------------------------------------

extern lcdCustomChar_t* iconChars[6];

/*==================[c++]====================================================*/
#ifdef __cplusplus
}
#endif

/*==================[end of file]============================================*/
#endif