/*=============================================================================
 * Copyright (c) 2019, Eric Pernia <ericpernia@gmail.com>
 * All rights reserved.
 * License: bsd-3-clause (see LICENSE.txt)
 * Date: 2019/08/26
 *===========================================================================*/

#include "sapi.h"
#include "font_8bit.h"

// Generar caracteres personalizados:
// https://maxpromer.github.io/LCD-Character-Creator/

// Para investigar como hacer mas de 8 caracteres personalizados:
// https://www.ccsinfo.com/forum/viewtopic.php?t=48459

// Chars 5x7 (width x height)

// Speaker --------------------------------------------------------------------

#define SPEAKER_ON_BITMAP \
{                         \
   ______X_              ,\
   _____XX_              ,\
   ___XX_X_              ,\
   ___X__X_              ,\
   ___XX_X_              ,\
   _____XX_              ,\
   ______X_              ,\
}

#define SPEAKER_OFF_BITMAP \
{                          \
   ______XX               ,\
   _____XX_               ,\
   ___XX_X_               ,\
   ___X_XX_               ,\
   ___XX_X_               ,\
   ____XXX_               ,\
   ___X__X_               ,\
}

/*
   ______XX               ,\
   _____XX_               ,\
   ___XX_X_               ,\
   ___X_XX_               ,\
   ___XX_X_               ,\
   ____XXX_               ,\
   ___X__X_               ,\
*/

const uint8_t SpeakerOnBitmap[8]  = SPEAKER_ON_BITMAP;
const uint8_t SpeakerOffBitmap[8] = SPEAKER_OFF_BITMAP;

lcdCustomChar_t SpeakerOnChar = {
   .address = 2,                // Custom character address
   .bitmap = SPEAKER_ON_BITMAP  // Custom character bitmap
};

lcdCustomChar_t SpeakerOffChar = {
   .address = 3,                // Custom character address
   .bitmap = SPEAKER_OFF_BITMAP // Custom character bitmap
};

// Speaker --------------------------------------------------------------------

lcdCustomChar_t* iconChars[2] = {
   &SpeakerOnChar,
   &SpeakerOffChar,
};
