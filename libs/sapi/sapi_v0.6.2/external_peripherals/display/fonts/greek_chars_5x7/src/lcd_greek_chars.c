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

// Original char map from:
// https://arduinoforgreekpeople.blogspot.com/2_X9/_7/arduino-lcd-X6_2-2__4.html

// Chars 5x7 (width x height)

#define GAMA_BITMAP       \
{                         \
   ___XXXXX              ,\
   ___X____              ,\
   ___X____              ,\
   ___X____              ,\
   ___X____              ,\
   ___X____              ,\
   ___X____              ,\
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


const uint8_t GamaBitmap[8]  = GAMA_BITMAP;
const uint8_t DeltaBitmap[8] = DELTA_BITMAP;
const uint8_t ThitaBitmap[8] = THITA_BITMAP;
const uint8_t LamdaBitmap[8] = LAMDA_BITMAP;
const uint8_t KsiBitmap[8]   = KSI_BITMAP;
const uint8_t PiBitmap[8]    = PI_BITMAP;
const uint8_t SigmaBitmap[8] = SIGMA_BITMAP;
const uint8_t FiBitmap[8]          = FI_BITMAP;
const uint8_t PsiBitmap[8]         = PSI_BITMAP;
const uint8_t OmegaBitmap[8]       = OMEGA_BITMAP;

const uint8_t NuBitmap[8]  = NU_BITMAP;


lcdCustomChar_t gamaChar = {
   .address = 0,           // Custom character address
   .bitmap = GAMA_BITMAP   // Custom character bitmap
};

lcdCustomChar_t deltaChar = {
   .address = 1,           // Custom character address
   .bitmap = DELTA_BITMAP  // Custom character bitmap
};

lcdCustomChar_t thitaChar = {
   .address = 2,           // Custom character address
   .bitmap = THITA_BITMAP  // Custom character bitmap
};

lcdCustomChar_t lamdaChar = {
   .address = 3,           // Custom character address
   .bitmap = LAMDA_BITMAP  // Custom character bitmap
};

lcdCustomChar_t ksiChar = {
   .address = 4,           // Custom character address
   .bitmap = KSI_BITMAP    // Custom character bitmap
};

lcdCustomChar_t piChar = {
   .address = 5,           // Custom character address
   .bitmap = PI_BITMAP     // Custom character bitmap
};

lcdCustomChar_t sigmaChar = {
   .address = 6,           // Custom character address
   .bitmap = SIGMA_BITMAP  // Custom character bitmap
};

lcdCustomChar_t fiChar = {
   .address = 7,           // Custom character address
   .bitmap = FI_BITMAP     // Custom character bitmap
};

lcdCustomChar_t psiChar = {
   .address = 4,           // Custom character address
   .bitmap = PSI_BITMAP,   // Custom character bitmap
};

lcdCustomChar_t omegaChar = {
   .address = 2,           // Custom character address
   .bitmap = OMEGA_BITMAP  // Custom character bitmap
};


lcdCustomChar_t nuChar = {
   .address = 7,           // Custom character address
   .bitmap = NU_BITMAP     // Custom character bitmap
};


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
