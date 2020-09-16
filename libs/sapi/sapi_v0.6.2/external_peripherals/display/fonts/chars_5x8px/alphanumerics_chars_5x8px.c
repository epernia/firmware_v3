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

// Simbolo de grados
#define DEGREE_BITMAP      \
{                          \
   ____XXX_               ,\
   ____X_X_               ,\
   ____XXX_               ,\
   ________               ,\
   ________               ,\
   ________               ,\
   ________               ,\
   ________               ,\
}

// E mayuscula personalizada
#define CUSTOM_UC_E        \
{                          \
   ____XXX_               ,\
   ___X____               ,\
   ___X____               ,\
   ____XX__               ,\
   ____X___               ,\
   ___X____               ,\
   ___X___X               ,\
   ____XXX_               ,\
}

// r minuscula personalizada
#define CUSTOM_LC_R        \
{                          \
   ________               ,\
   ________               ,\
   ________               ,\
   ____XXX_               ,\
   ____X___               ,\
   ____X___               ,\
   ____X___               ,\
   ____X___               ,\
}

// i minuscula personalizada
#define CUSTOM_LC_I        \
{                          \
   ________               ,\
   _____X__               ,\
   ________               ,\
   _____X__               ,\
   _____X__               ,\
   _____X__               ,\
   _____X__               ,\
   _____XX_               ,\
}

// c minuscula personalizada
#define CUSTOM_LC_C        \
{                          \
   ________               ,\
   ________               ,\
   ________               ,\
   _____XX_               ,\
   ____X___               ,\
   ____X___               ,\
   ____X__X               ,\
   _____XX_               ,\
}

// Custom char variables ------------------------------------------------------

// Simbolo de grados

const uint8_t degreeBitmap[8] = DEGREE_BITMAP;

lcdCustomChar_t degreeChar = {
   .address = 0,
   .bitmap = DEGREE_BITMAP
};

// Custom upercase E

const uint8_t customUCEBitmap[8] = CUSTOM_UC_E;

lcdCustomChar_t customUCEChar = {
   .address = 1,
   .bitmap = CUSTOM_UC_E
};

// Custom lowercase r 

const uint8_t customLCRBitmap[8] = CUSTOM_LC_R;

lcdCustomChar_t customLCRChar = {
   .address = 2,
   .bitmap = CUSTOM_LC_R
};

// Custom lowercase i

const uint8_t customLCIBitmap[8] = CUSTOM_LC_I;

lcdCustomChar_t customLCIChar = {
   .address = 3,
   .bitmap = CUSTOM_LC_I
};

// Custom lowercase c

const uint8_t customLCCBitmap[8] = CUSTOM_LC_C;

lcdCustomChar_t customLCCChar = {
   .address = 4,
   .bitmap = CUSTOM_LC_C
};

// Custom char array ----------------------------------------------------------

lcdCustomChar_t* customAlphanumerics[5] = {
   &degreeChar,
   &customUCEChar,
   &customLCRChar,
   &customLCIChar,
   &customLCCChar,
};