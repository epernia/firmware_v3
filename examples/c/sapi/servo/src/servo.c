/* Copyright 2016, Eric Pernia.
 * All rights reserved.
 *
 * This file is part sAPI library for microcontrollers.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * 3. Neither the name of the copyright holder nor the names of its
 *    contributors may be used to endorse or promote products derived from this
 *    software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

/*
 * Date: 2016-07-03
 */

/*==================[inclusions]=============================================*/

#include "sapi.h"      // <= sAPI header

/*==================[macros and definitions]=================================*/

#define SERVO_N   SERVO0
/*
   SERVO0 <---> T_FIL1 de EDU-CIAA-NXP
   SERVO1 <---> T_COL0 de EDU-CIAA-NXP
   SERVO2 <---> T_FIL2 de EDU-CIAA-NXP
   SERVO3 <---> T_FIL3 de EDU-CIAA-NXP
   SERVO4 <---> GPIO8 de EDU-CIAA-NXP
   SERVO5 <---> LCD1 de EDU-CIAA-NXP
   SERVO6 <---> LCD2 de EDU-CIAA-NXP
   SERVO7 <---> LCD3 de EDU-CIAA-NXP
   SERVO8 <---> GPIO2 de EDU-CIAA-NXP

   Notas:

   Descargue el ejemplo y luego conecte el servo.

   Tenga en cuenta tambien la corriente maxima del regulador al
   conectar uno o mas servos.

   Puede requerir conectar un cargador de celular al puerto USB_OTG en
   caso que su computadora no entregue suficiente corriente en el USB_DEBUG.

   Puede requerir un borrado completo de la memoria flash luego del
   uso de este programa para permitir grabar programas posteriores.
   Para hcerlo debe puentear el Jumper JP5 y resetear la placa con el
   puente sostenido hasta soltar el boton de reset entrando en modo
   ISP (se prenden todos los leds), luego aplique "make erase" y debe
   resetear nuevamente la placa para permitir grabar nuevos programas.
*/

/*==================[internal data declaration]==============================*/

/*==================[internal functions declaration]=========================*/

/*==================[internal data definition]===============================*/

/*==================[external data definition]===============================*/

/*==================[internal functions definition]==========================*/

/*==================[external functions definition]==========================*/

// FUNCION PRINCIPAL, PUNTO DE ENTRADA AL PROGRAMA LUEGO DE RESET.
int main(void)
{

   // ------------- INICIALIZACIONES ----------------

   // Inicializar la placa
   boardConfig();

   bool_t valor = 0;

   uint8_t servoAngle = 0; // 0 a 180 grados

   // Configurar Servo
   valor = servoConfig( 0, SERVO_ENABLE );

   valor = servoConfig( SERVO_N, SERVO_ENABLE_OUTPUT );

   // Usar Servo
   valor = servoWrite( SERVO_N, servoAngle );
   servoAngle = servoRead( SERVO_N );

   gpioWrite( LEDB, 1 );

   // ------------- REPETIR POR SIEMPRE -------------
   while(1) {
      servoWrite( SERVO_N, 0 );
      delay(500);

      servoWrite( SERVO_N, 90 );
      delay(500);

      servoWrite( SERVO_N, 180 );
      delay(500);
   }

   // NO DEBE LLEGAR NUNCA AQUI, debido a que a este programa no es llamado
   // por ningun S.O.
   return 0 ;
}

/*==================[end of file]============================================*/
