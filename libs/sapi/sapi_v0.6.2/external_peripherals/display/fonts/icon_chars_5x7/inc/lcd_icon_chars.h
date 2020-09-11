/*=============================================================================
 * Copyright (c) 2019, Eric Pernia <ericpernia@gmail.com>
 * All rights reserved.
 * License: bsd-3-clause (see LICENSE.txt)
 * Date: 2019/08/26
 *===========================================================================*/

#ifndef _LCD_ICONS_CHARS_H_
#define _LCD_ICONS_CHARS_H_

/*==================[inclusiones]============================================*/

#include "sapi.h"

/*==================[c++]====================================================*/

#ifdef __cplusplus
extern "C" {
#endif
   
/*===========================================================================*/

extern const uint8_t SpeakerOnBitmap[8];
extern const uint8_t SpeakerOffBitmap[8];

extern lcdCustomChar_t SpeakerOnChar;
extern lcdCustomChar_t SpeakerOffChar;

extern lcdCustomChar_t* iconsChars[2];

/*==================[c++]====================================================*/
#ifdef __cplusplus
}
#endif

/*==================[end of file]============================================*/
#endif /* _LCD_GREEK_CHARS_H_ */
