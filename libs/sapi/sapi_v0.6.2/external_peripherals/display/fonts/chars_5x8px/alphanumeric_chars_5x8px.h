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

// Simbolo de grados
extern const uint8_t degreeBitmap[8];
extern lcdCustomChar_t degreeChar;

// E mayuscula personalizada
extern const uint8_t customUCEBitmap[8];
extern lcdCustomChar_t customUCEChar;

// r minuscula personalizada
extern const uint8_t customLCRBitmap[8];
extern lcdCustomChar_t speakerOffChar;

// i minuscula personalizada
extern const uint8_t customLCIBitmap[8];
extern lcdCustomChar_t customLCIChar;

// c minuscula personalizada
extern const uint8_t customLCCBitmap[8];
extern lcdCustomChar_t customLCCChar;

// Custom char array ----------------------------------------------------------

extern lcdCustomChar_t* customAlphanumerics[5];

/*==================[c++]====================================================*/
#ifdef __cplusplus
}
#endif

/*==================[end of file]============================================*/
#endif