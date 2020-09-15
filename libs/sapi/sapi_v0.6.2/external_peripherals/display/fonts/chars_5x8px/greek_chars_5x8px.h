/*=============================================================================
 * Copyright (c) 2019, Eric Pernia <ericpernia@gmail.com>
 * All rights reserved.
 * License: bsd-3-clause (see LICENSE.txt)
 * Date: 2019/08/26
 *===========================================================================*/

#ifndef _LCD_GREEK_CHARS_5X8_PX_H_
#define _LCD_GREEK_CHARS_5X8_PX_H_

/*==================[inclusiones]============================================*/

#include "sapi.h"

/*==================[c++]====================================================*/

#ifdef __cplusplus
extern "C" {
#endif
   
/*===========================================================================*/

extern const uint8_t GamaBitmap[8];
extern const uint8_t DeltaBitmap[8];
extern const uint8_t ThitaBitmap[8];
extern const uint8_t LamdaBitmap[8];
extern const uint8_t KsiBitmap[8];
extern const uint8_t PiBitmap[8];
extern const uint8_t SigmaBitmap[8];
extern const uint8_t FiBitmap[8];
extern const uint8_t PsiBitmap[8];
extern const uint8_t OmegaBitmap[8];
   
extern const uint8_t NuBitmap[8];

extern lcdCustomChar_t gamaChar;
extern lcdCustomChar_t deltaChar;
extern lcdCustomChar_t thitaChar;
extern lcdCustomChar_t lamdaChar;
extern lcdCustomChar_t ksiChar;
extern lcdCustomChar_t piChar;
extern lcdCustomChar_t sigmaChar;
extern lcdCustomChar_t fiChar;
extern lcdCustomChar_t psiChar;
extern lcdCustomChar_t omegaChar;

extern lcdCustomChar_t nuChar;

extern lcdCustomChar_t* greekChars[11];

/*==================[c++]====================================================*/
#ifdef __cplusplus
}
#endif

/*==================[end of file]============================================*/
#endif