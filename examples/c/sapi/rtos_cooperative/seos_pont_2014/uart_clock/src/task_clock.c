/*============================================================================
 * License: BSD-3-Clause
 * Copyright 2018, Eric Pernia <ericpernia@gmail.com>
 * All rights reserved.
 * Date: 2018/10/04
 *===========================================================================*/

/*==================[inlcusiones]============================================*/

#include "task_clock.h"

/*==================[definiciones y macros]==================================*/

/*==================[definiciones de datos internos]=========================*/

static uint8_t Reloj_string[18] = "RELOJ: HH:MM:SS\r\n";

static uint8_t hs;
static uint8_t min;
static uint8_t seg;

static const uint8_t CharMap[10] = {'0','1','2','3','4','5','6','7','8','9'};

/*==================[definiciones de datos externos]=========================*/

/*==================[declaraciones de funciones internas]====================*/

/*==================[declaraciones de funciones externas]====================*/

/*==================[definiciones de funciones internas]=====================*/

/*==================[definiciones de funciones externas]=====================*/

void taskClockInit( uint8_t h, uint8_t m, uint8_t s )
{
   hs  = h;
   min = m;
   seg = s;
}

void taskClock( void* taskParam )
{
   // Actualizo reloj
   if ( ++seg == 60 ){
      seg = 0;
      if ( ++min == 60 ){
         min = 0;
           if ( ++hs == 24 ){
           hs = 0;
         }
      }
   }

   // Armo mensaje a visualizar
   Reloj_string[7]  = CharMap[hs/10];
   Reloj_string[8]  = CharMap[hs%10];
   Reloj_string[10] = CharMap[min/10];
   Reloj_string[11] = CharMap[min%10];
   Reloj_string[13] = CharMap[seg/10];
   Reloj_string[14] = CharMap[seg%10];

   // Lo envi­o al Buffer
   Write_String_To_Buffer(Reloj_string);
}

/*==================[fin del archivo]========================================*/
