/*=============================================================================
 * Copyright (c) 2019, Eric Pernia <ericpernia@gmail.com>
 * All rights reserved.
 * License: bsd-3-clause (see LICENSE.txt)
 * Date: 2019/04/30
 * Version: 1.0.0
 *===========================================================================*/

#include "sapi.h"

/* FUNCION PRINCIPAL, PUNTO DE ENTRADA AL PROGRAMA LUEGO DE RESET. */
int main(void)
{
   /* ------------- INICIALIZACIONES ------------- */
   boardInit();

   // Crear estructura RTC
   rtc_t rtc;

   // Completar estructura RTC
   rtc.year = 2018;
   rtc.month = 4;
   rtc.mday = 30;
   rtc.wday = 3;
   rtc.hour = 12;
   rtc.min = 15;
   rtc.sec= 0;
  
   // Inicializar RTC 
   rtcInit();
   rtcWrite( &rtc );
   
   /* ------------- REPETIR POR SIEMPRE ------------- */
   while(1) {

      // Leer fecha y hora
      rtcRead( &rtc ); // en la variable de estructura rtc te queda la fecha/hora actual

      // Envio por UART de forma humanamente legible
      // %02d == %d y ademas completa con 2 0 a izquierda
      printf( "%02d/%02d/%04d, %02d:%02d:%02d\r\n", 
              rtc.mday, rtc.month, rtc.year, 
              rtc.hour, rtc.min, rtc.sec );
      // Note: printf() use sAPI UART_USB (Chip USART2 on EDU-CIAA-NXP) at 115200, 8N1 

      delay(1000);
   }
   /* NO DEBE LLEGAR NUNCA AQUI, debido a que a este programa no es llamado
   por ningun S.O. */
   return 0 ;
}
