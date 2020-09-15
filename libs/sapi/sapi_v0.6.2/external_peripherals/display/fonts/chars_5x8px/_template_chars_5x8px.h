/*=============================================================================
 * Copyright (c) 2019, Eric Pernia <ericpernia@gmail.com>
 * All rights reserved.
 * License: bsd-3-clause (see LICENSE.txt)
 * Date: 2019/08/26
 *===========================================================================*/

#ifndef _TEMPLATE_CHARS_5X8_PX_H_
#define _TEMPLATE_CHARS_5X8_PX_H_

/*==================[inclusiones]============================================*/

#include "sapi.h"

/*==================[c++]====================================================*/

#ifdef __cplusplus
extern "C" {
#endif
   
/*===========================================================================*/

// Custom char variables ------------------------------------------------------

extern const uint8_t customCharNameBitmap[8;
extern lcdCustomChar_t customCharNameChar;

// Custom char array ----------------------------------------------------------

estern lcdCustomChar_t* customChars[1]; // Remember update this 1 for real size

/*==================[c++]====================================================*/
#ifdef __cplusplus
}
#endif

/*==================[end of file]============================================*/
#endif