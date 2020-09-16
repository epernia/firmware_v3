/*=============================================================================
 * Copyright (c) 2019, Eric Pernia <ericpernia@gmail.com>
 * All rights reserved.
 * License: bsd-3-clause (see LICENSE.txt)
 * Date: 2019/08/26
 *===========================================================================*/

// Chars 5x8 (width x height)

#include "sapi.h"
#include "font_bytes.h"

// Custom char bitmaps --------------------------------------------------------

// Carita sonriendo
#define SMILE_BITMAP       \
{                          \
   ________               ,\
   ____X_X_               ,\
   ____X_X_               ,\
   ____X_X_               ,\
   ________               ,\
   ___X___X               ,\
   ____XXX_               ,\
   ________               ,\
}

// Parlante
#define SPEAKER_ON_BITMAP  \
{                          \
   ______X_               ,\
   _____XX_               ,\
   ___XXXX_               ,\
   ___XXXX_               ,\
   ___XXXX_               ,\
   _____XX_               ,\
   ______X_               ,\
   ________               ,\
}

// Parlante tachado
#define SPEAKER_OFF_BITMAP \
{                          \
   ______X_              ,\
   _____XX_              ,\
   ___XX_X_              ,\
   ___X__X_              ,\
   ___XX_X_              ,\
   _____XX_              ,\
   ______X_              ,\
   ________              ,\
}

// Termometro o tubo de ensayo
#define THERMOMETER_BITMAP \
{                          \
   ____XXX_               ,\
   ____X_X_               ,\
   ____X_X_               ,\
   ____XXX_               ,\
   ____XXX_               ,\
   ___X_XXX               ,\
   ___XXXXX               ,\
   ____XXX_               ,\
}

// Gota de agua
#define DROP_BITMAP        \
{                          \
   _____X__               ,\
   _____X__               ,\
   ____XXX_               ,\
   ___X_XXX               ,\
   ___X_XXX               ,\
   ___X__XX               ,\
   ____XXX_               ,\
   ________               ,\
}

// Viento
#define WIND_BITMAP        \
{                          \
   _____XXX               ,\
   ___XXX__               ,\
   ________               ,\
   _____XXX               ,\
   ___XXX__               ,\
   ________               ,\
   _____XXX               ,\
   ___XXX__               ,\
}

// Custom char variables ------------------------------------------------------

// Carita sonriendo

const uint8_t smileBitmap[8] = SMILE_BITMAP;

lcdCustomChar_t smileChar = {
   .address = 0,           // Custom character address
   .bitmap = SMILE_BITMAP  // Custom character bitmap
};

// Parlante encendido

const uint8_t speakerOnBitmap[8]  = SPEAKER_ON_BITMAP;

lcdCustomChar_t speakerOnChar = {
   .address = 1,
   .bitmap = SPEAKER_ON_BITMAP
};

// Parlante apagado

const uint8_t speakerOffBitmap[8] = SPEAKER_OFF_BITMAP;

lcdCustomChar_t speakerOffChar = {
   .address = 2,
   .bitmap = SPEAKER_OFF_BITMAP
};

// Termometro o tubo de ensayo

const uint8_t thermometerBitmap[8] = THERMOMETER_BITMAP;

lcdCustomChar_t thermometerChar = {
   .address = 3,
   .bitmap = THERMOMETER_BITMAP
};

// Gota de agua

const uint8_t dropBitmap[8] = DROP_BITMAP;

lcdCustomChar_t dropChar = {
   .address = 4,
   .bitmap = DROP_BITMAP
};

// Viento

const uint8_t windBitmap[8] = WIND_BITMAP;

lcdCustomChar_t windChar = {
   .address = 5,
   .bitmap = WIND_BITMAP
};

// Custom char array ----------------------------------------------------------

lcdCustomChar_t* iconChars[] = {
   &speakerOnChar,
   &speakerOffChar,
   &smileChar,
   &thermometerChar,
   &dropChar,
   &windChar,
};