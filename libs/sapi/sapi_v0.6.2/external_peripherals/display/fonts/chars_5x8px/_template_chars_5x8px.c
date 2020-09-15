/*=============================================================================
 * Copyright (c) 2019, Eric Pernia <ericpernia@gmail.com>
 * All rights reserved.
 * License: bsd-3-clause (see LICENSE.txt)
 * Date: 2019/08/26
 *===========================================================================*/

// Generar caracteres personalizados:
// https://maxpromer.github.io/LCD-Character-Creator/

// Para investigar como hacer mas de 8 caracteres personalizados:
// https://www.ccsinfo.com/forum/viewtopic.php?t=48459

// Chars 5x8 (width x heigh

#include "sapi.h"
#include "font_bytes.h"

// Custom char bitmaps --------------------------------------------------------

// Custom char bitmap name
#define CUSTOM_CHAR_NAME_BITMAP \
{                               \
   ________                    ,\
   ________                    ,\
   ________                    ,\
   ________                    ,\
   ________                    ,\
   ________                    ,\
   ________                    ,\
   ________                    ,\
}

// Put an X where yo need pixel on. 
// You have Only 5 pixels wide, so first 3 chars always be ___
//
// Example:
//
// #define UPERCASE_E_BITMAP  \
// {                          \
//    ___XXXXX               ,\
//    ___X____               ,\
//    ___X____               ,\
//    ___XXX__               ,\
//    ___X____               ,\
//    ___X____               ,\
//    ___XXXXX               ,\
//    ________               ,\
// }

// Custom char variables ------------------------------------------------------

// Custom char variable names (2 for each char)

const uint8_t customCharNameBitmap[8] = CUSTOM_CHAR_NAME_BITMAP;

lcdCustomChar_t customCharNameChar = {
   .address = 0,
   .bitmap = CUSTOM_CHAR_NAME_BITMAP
};

// Custom char array ----------------------------------------------------------

lcdCustomChar_t* customChars[1] = { // Remember update this 1 for real size
   &customCharNameChar,
   // ... the rest of lcdCustomChar_t vars
};