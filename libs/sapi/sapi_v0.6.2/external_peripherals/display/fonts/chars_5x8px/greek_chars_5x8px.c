/*=============================================================================
 * Copyright (c) 2019, Eric Pernia <ericpernia@gmail.com>
 * All rights reserved.
 * License: bsd-3-clause (see LICENSE.txt)
 * Date: 2019/08/26
 *===========================================================================*/

// Original char map from:
// https://arduinoforgreekpeople.blogspot.com/2_X9/_7/arduino-lcd-X6_2-2__4.html

// Chars 5x8 (width x height)

#include "sapi.h"
#include "font_bytes.h"

// Custom char bitmaps --------------------------------------------------------

#define GAMA_BITMAP       \
{                         \
   ___XXXXX              ,\
   ___X____              ,\
   ___X____              ,\
   ___X____              ,\
   ___X____              ,\
   ___X____              ,\
   ___X____              ,\
   ________              ,\
}

#define DELTA_BITMAP      \
{                         \
   _____X__              ,\
   _____X__              ,\
   ____X_X_              ,\
   ____X_X_              ,\
   ____X_X_              ,\
   ___X___X              ,\
   ___XXXXX              ,\
   ________              ,\
}

#define THITA_BITMAP      \
{                         \
   ____XXX_              ,\
   ___XX_XX              ,\
   ___X___X              ,\
   ___XXXXX              ,\
   ___X___X              ,\
   ___XX_XX              ,\
   ____XXX_              ,\
   ________              ,\
}

#define LAMDA_BITMAP      \
{                         \
   _____X__              ,\
   _____X__              ,\
   ____X_X_              ,\
   ____X_X_              ,\
   ____X_X_              ,\
   ___X___X              ,\
   ___X___X              ,\
   ________              ,\
}

#define KSI_BITMAP        \
{                         \
   ___XXXXX              ,\
   ________              ,\
   ________              ,\
   ____XXX_              ,\
   ________              ,\
   ________              ,\
   ___XXXXX              ,\
   ________              ,\
}

#define PI_BITMAP         \
{                         \
   ___XXXXX              ,\
   ___X___X              ,\
   ___X___X              ,\
   ___X___X              ,\
   ___X___X              ,\
   ___X___X              ,\
   ___X___X              ,\
   ________              ,\
}

#define SIGMA_BITMAP      \
{                         \
   ___XXXXX              ,\
   ____X___              ,\
   _____X__              ,\
   ______X_              ,\
   _____X__              ,\
   ____X___              ,\
   ___XXXXX              ,\
   ________              ,\
}

#define FI_BITMAP         \
{                         \
   _____X__              ,\
   ____XXX_              ,\
   ___X_X_X              ,\
   ___X_X_X              ,\
   ___X_X_X              ,\
   ____XXX_              ,\
   _____X__              ,\
   ________              ,\
}

#define PSI_BITMAP        \
{                         \
   ___X___X              ,\
   ___X_X_X              ,\
   ___X_X_X              ,\
   ___X_X_X              ,\
   ____XXX_              ,\
   _____X__              ,\
   _____X__              ,\
   ________              ,\
}

#define OMEGA_BITMAP      \
{                         \
   ____XXX_              ,\
   ___X___X              ,\
   ___X___X              ,\
   ___X___X              ,\
   ____X_X_              ,\
   ____X_X_              ,\
   ___XX_XX              ,\
   ________              ,\
}

#define NU_BITMAP         \
{                         \
   ___X_XX_              ,\
   ___XX__X              ,\
   ___X___X              ,\
   ___X___X              ,\
   ___X___X              ,\
   _______X              ,\
   _______X              ,\
}

// Custom char variables ------------------------------------------------------

const uint8_t GamaBitmap[8]  = GAMA_BITMAP;
const uint8_t DeltaBitmap[8] = DELTA_BITMAP;
const uint8_t ThitaBitmap[8] = THITA_BITMAP;
const uint8_t LamdaBitmap[8] = LAMDA_BITMAP;
const uint8_t KsiBitmap[8]   = KSI_BITMAP;
const uint8_t PiBitmap[8]    = PI_BITMAP;
const uint8_t SigmaBitmap[8] = SIGMA_BITMAP;
const uint8_t FiBitmap[8]    = FI_BITMAP;

const uint8_t PsiBitmap[8]   = PSI_BITMAP;
const uint8_t OmegaBitmap[8] = OMEGA_BITMAP;
const uint8_t NuBitmap[8]    = NU_BITMAP;

lcdCustomChar_t gamaChar = {
   .address = 0,
   .bitmap = GAMA_BITMAP
};

lcdCustomChar_t deltaChar = {
   .address = 1,
   .bitmap = DELTA_BITMAP
};

lcdCustomChar_t thitaChar = {
   .address = 2,
   .bitmap = THITA_BITMAP
};

lcdCustomChar_t lamdaChar = {
   .address = 3,
   .bitmap = LAMDA_BITMAP
};

lcdCustomChar_t ksiChar = {
   .address = 4,
   .bitmap = KSI_BITMAP
};

lcdCustomChar_t piChar = {
   .address = 5,
   .bitmap = PI_BITMAP
};

lcdCustomChar_t sigmaChar = {
   .address = 6,
   .bitmap = SIGMA_BITMAP
};

lcdCustomChar_t fiChar = {
   .address = 7,
   .bitmap = FI_BITMAP
};


lcdCustomChar_t psiChar = {
   .address = 0,
   .bitmap = PSI_BITMAP
};

lcdCustomChar_t omegaChar = {
   .address = 1,
   .bitmap = OMEGA_BITMAP
};

lcdCustomChar_t nuChar = {
   .address = 2,
   .bitmap = NU_BITMAP
};

// Custom char array ----------------------------------------------------------

lcdCustomChar_t* greekChars[11] = {
   &gamaChar,
   &deltaChar,
   &thitaChar,
   &lamdaChar,
   &ksiChar,
   &piChar,
   &sigmaChar,
   &fiChar,
   &psiChar,
   &omegaChar,
   &nuChar, 
};