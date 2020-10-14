/* Copyright 2016, Ian Olivieri.
 * Copyright 2016, Eric Pernia.
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
 *
 */

/*
 * Date: 2016-04-26
 */

/* 
ESP01 (ESP8266) connections:

   +--------------------------------------+
   |  |          +----+                   |           
   |  +--+  |    |    |      RX o o VCC   |
   |     |  |    +----+   GPIO0 o o RST   |         
   |  +--+  |    +----+   GPIO2 o o CH_PD |
   |  |     |    |    |     GND o o TX    |
   |  +-----+    +----+                   |
   +--------------------------------------+

   VCC ESP8266 <--> +3.3V EDU-CIAA-NXP
   RST ESP8266 <--> (SIN CONEXION)
 CH_PD ESP8266 <--> +3.3V EDU-CIAA-NXP
    TX ESP8266 <--> RX EDU-CIAA-NXP

    RX ESP8266 <--> TX EDU-CIAA-NXP
 GPIO0 ESP8266 <--> (SIN CONEXION)
 GPIO0 ESP8266 <--> (SIN CONEXION)
   GND ESP8266 <--> GND EDU-CIAA-NXP

  AT commands: http://www.pridopia.co.uk/pi-doc/ESP8266ATCommandsSet.pdf
*/

/*==================[inclusions]=============================================*/

#include "sapi.h"
#include "wifi_module.h"

/*==================[macros and definitions]=================================*/

#define BAUD_RATE 115200 // Baudrate por defecto del ESP8266

/*==================[internal data declaration]==============================*/

/*==================[internal functions declaration]=========================*/

/*==================[internal data definition]===============================*/

/*==================[external data definition]===============================*/

/*==================[internal functions definition]==========================*/

/*==================[external functions definition]==========================*/

void imprimirMensajeDeBienvenida( void )
{
   /*   
   AT+CWMODE?
   AT+CWMODE=3
   */ 
   // Imprimo el mensaje de bienvenida
   uartWriteString( UART_USB,
      "Bievenido al asistente de configuracion del modulo ESP8266\r\n" );
   uartWriteString( UART_USB,
      "Antes de continuar, por favor asegurese que su terminal\r\n" );
   uartWriteString( UART_USB,
      "serie dispone del terminador CR+LF (enter)\r\n\r\n" );
   uartWriteString( UART_USB,
      "A continuacion se realiza un listado de algunos de los\r\n" );
   uartWriteString( UART_USB, "comandos AT disponibles:\r\n\r\n" );
   uartWriteString( UART_USB,
      "> Saber si el modulo responde correctamente: AT\r\n" );
   uartWriteString( UART_USB,
      "> Version del Firmware: AT+GMR\r\n" );
   uartWriteString( UART_USB, "> Resetear el modulo: AT+RST\r\n" );
   uartWriteString( UART_USB,
      "> Listar todas las redes disponibles: AT+CWLAP\r\n" );
   uartWriteString( UART_USB,
      "> Checkear la red actual: AT+CWJAP?\r\n" );
   uartWriteString( UART_USB,
      "> Unirse a una red: AT+CWJAP=\"nombreRedInalambrica\",\"password\"\r\n" );
   uartWriteString( UART_USB,
      "  - NOTA: Las comillas dobles son parte del mensaje\r\n" );
   uartWriteString( UART_USB,
      "> Salir de la red: AT+CWQAP\r\n" );

   delay(100);
}


wifiModuleRequestResult_t res = 0;

bool sentAt = false;
bool sentReset = false;
bool sentInit = false;

bool sentIsConnected = false;
bool sentConnectWithAp = false;
bool sentGetIp = false;

bool pruebaComandos = false;

void pruebaComandos1( void );
void pruebaComandos2( void );

/* FUNCION PRINCIPAL, PUNTO DE ENTRADA AL PROGRAMA LUEGO DE RESET. */
int main(void){

   /* ------------- INICIALIZACIONES ------------- */

   /* Inicializar la placa */
   boardInit();

   /* Inicializar las UART a 115200 baudios (si no funciona probar a 9600) */
   uartConfig( UART_USB, BAUD_RATE );
   uartConfig( UART_232, BAUD_RATE );

   uint8_t dato  = 0;
   
   imprimirMensajeDeBienvenida();

   bool modeBridge = true;
   gpioWrite( LEDB, modeBridge );
   gpioWrite( LEDR, modeBridge );

   /* ------------- REPETIR POR SIEMPRE ------------- */
   while(1) {
      if( !gpioRead( TEC1 ) ){
         gpioToggle( LEDB );
         modeBridge = gpioRead(LEDB);
         delay(500);
      }

      if ( modeBridge ) {
         if( uartReadByte( UART_USB, &dato ) ){
            uartWriteByte( UART_232, dato );
         }
         if( uartReadByte( UART_232, &dato ) ){
            uartWriteByte( UART_USB, dato );
         }         
         if( !gpioRead( TEC2 ) ){
            gpioToggle( LEDR );
            pruebaComandos = gpioRead(LEDR);
            delay(500);
         }
      } else { // Mode test commands
         if ( pruebaComandos ) {
            pruebaComandos1();
         } else {
            pruebaComandos2();
         }
      }

   }

   /* NO DEBE LLEGAR NUNCA AQUI, debido a que a este programa no es llamado
      por ningun S.O. */
   return 0 ;
}

void pruebaComandos1( void )
{
   //-----------------------------------------------------------------------
   // Detect module
   if( !gpioRead( TEC2 ) ){
      gpioWrite(LED1,ON);
      delay(500);   
      printf("WIFI_MODULE_DETECTION_STARTED\r\n");
      wifiModuleStartDetection();
      sentAt = true;
      gpioWrite(LED1,OFF);
   }
   if ( sentAt ) {
      wifiModuleRequestResult_t res = wifiModuleDetectionResponse();
      while( res != WIFI_MODULE_DETECTED &&
             res != WIFI_MODULE_NOT_DETECTED ) {
         res = wifiModuleDetectionResponse();
         if( !gpioRead( TEC1 ) ) {
            delay(500);
            sentAt = false;
            break;
         }
      }
      if ( res == WIFI_MODULE_DETECTED ) {
         printf("WIFI_MODULE_DETECTED\r\n");
      } else if ( res == WIFI_MODULE_NOT_DETECTED ) {
         printf("WIFI_MODULE_NOT_DETECTED\r\n");
      }
      sentAt = false;
   }
   //-----------------------------------------------------------------------
   // Reset module
   if( !gpioRead( TEC3 ) ){
      gpioWrite(LED2,ON);
      delay(500);   
      printf("WIFI_MODULE_RESET_STARTED\r\n");
      wifiModuleStartReset();
      sentReset = true;
      gpioWrite(LED2,OFF);
   }
   if ( sentReset ) {    
      wifiModuleRequestResult_t res = wifiModuleResetResponse();
      while( res != WIFI_MODULE_RESET_COMPLETE &&
             res != WIFI_MODULE_NOT_DETECTED ) {
         res = wifiModuleResetResponse();
         if( !gpioRead( TEC1 ) ) {
            delay(500);
            sentReset = false;
            break;
         }
      }
      if ( res == WIFI_MODULE_RESET_COMPLETE ) {
         printf("WIFI_MODULE_RESET_COMPLETE\r\n");
      } else if ( res == WIFI_MODULE_NOT_DETECTED ) {
         printf("WIFI_MODULE_NOT_DETECTED\r\n");
      }
      sentReset = false;
   }
   //-----------------------------------------------------------------------
   // Initialize module
   if( !gpioRead( TEC4 ) ){
      gpioWrite(LED3,ON);
      delay(500);
      printf("WIFI_MODULE_INIT_STARTED\r\n");
      wifiModuleStartInit();            
      sentInit = true;
      gpioWrite(LED3,OFF);
   }
   if ( sentInit ) {  
      wifiModuleRequestResult_t res = wifiModuleInitResponse();
      while( res != WIFI_MODULE_INIT_COMPLETE &&
             res != WIFI_MODULE_NOT_DETECTED ) {
         res = wifiModuleInitResponse();
         if( !gpioRead( TEC1 ) ) {
            delay(500);
            sentInit = false;
            break;
         }
      }
      if ( res == WIFI_MODULE_INIT_COMPLETE ) {
         printf("WIFI_MODULE_INIT_COMPLETE\r\n");
      } else if ( res == WIFI_MODULE_NOT_DETECTED ) {
         printf("WIFI_MODULE_NOT_DETECTED\r\n");
      }
      sentInit = false;
   }
   //-----------------------------------------------------------------------
}

void pruebaComandos2( void )
{
   static char ip[20] = "";
   //-----------------------------------------------------------------------
   // Check if connected with AP
   if( !gpioRead( TEC2 ) ){
      gpioWrite(LED1,ON);
      delay(500);   
      printf("WIFI_MODULE_IS_CONNECTED_AP_STARTED\r\n");
      wifiModuleStartIsConnectedWithAP();
      sentIsConnected = true;
      gpioWrite(LED1,OFF);
   }
   if ( sentIsConnected ) {
      wifiModuleRequestResult_t res = wifiModuleIsConnectedWithAPResponse();
      while( res != WIFI_MODULE_IS_CONNECTED &&
             res != WIFI_MODULE_IS_NOT_CONNECTED &&
             res != WIFI_MODULE_NOT_DETECTED ) {
         res = wifiModuleIsConnectedWithAPResponse();
         if( !gpioRead( TEC1 ) ) {
            delay(500);
            sentIsConnected = false;
            break;
         }
      }
      if ( res == WIFI_MODULE_IS_CONNECTED ) {
         printf("WIFI_MODULE_IS_CONNECTED\r\n");
      } else if ( res == WIFI_MODULE_IS_NOT_CONNECTED ) {
         printf("WIFI_MODULE_IS_NOT_CONNECTED\r\n");
      } else if ( res == WIFI_MODULE_NOT_DETECTED ) {
         printf("WIFI_MODULE_NOT_DETECTED\r\n");
      }
      sentIsConnected = false;
   }
   //-----------------------------------------------------------------------
   // Connect with AP
   if( !gpioRead( TEC3 ) ){
      gpioWrite(LED2,ON);
      delay(500);   
      printf("WIFI_MODULE_CONNECT_AP_STARTED\r\n");
      wifiModuleStartConnectWithAP();
      sentConnectWithAp = true;
      gpioWrite(LED2,OFF);
   }
   if ( sentConnectWithAp ) {
      wifiModuleRequestResult_t res = wifiModuleConnectWithAPResponse();
      while( res != WIFI_MODULE_IS_CONNECTED &&
             res != WIFI_MODULE_IS_NOT_CONNECTED &&
             res != WIFI_MODULE_CONNECT_AP_ERR_TIMEOUT &&
             res != WIFI_MODULE_CONNECT_AP_ERR_WRONG_PASS &&
             res != WIFI_MODULE_CONNECT_AP_ERR_AP_NOT_FOUND &&
             res != WIFI_MODULE_CONNECT_AP_ERR_CONN_FAIL &&
             res != WIFI_MODULE_NOT_DETECTED ) {
         res = wifiModuleConnectWithAPResponse();
         if( !gpioRead( TEC1 ) ) {
            delay(500);
            sentConnectWithAp = false;
            break;
         }
      }
      if ( res == WIFI_MODULE_IS_CONNECTED ) {
         printf("WIFI_MODULE_IS_CONNECTED\r\n");
      } else if ( res == WIFI_MODULE_IS_NOT_CONNECTED ) {
         printf("WIFI_MODULE_IS_NOT_CONNECTED\r\n");
      } else if ( res == WIFI_MODULE_CONNECT_AP_ERR_TIMEOUT ) {
         printf("WIFI_MODULE_CONNECT_AP_ERR_TIMEOUT\r\n");
      } else if ( res == WIFI_MODULE_CONNECT_AP_ERR_WRONG_PASS ) {
         printf("WIFI_MODULE_CONNECT_AP_ERR_WRONG_PASS\r\n");
      } else if ( res == WIFI_MODULE_CONNECT_AP_ERR_AP_NOT_FOUND ) {
         printf("WIFI_MODULE_CONNECT_AP_ERR_AP_NOT_FOUND\r\n");
      } else if ( res == WIFI_MODULE_CONNECT_AP_ERR_CONN_FAIL ) {
         printf("WIFI_MODULE_CONNECT_AP_ERR_CONN_FAIL\r\n");
      } else if ( res == WIFI_MODULE_NOT_DETECTED ) {
         printf("WIFI_MODULE_NOT_DETECTED\r\n");
      }
      sentConnectWithAp = false;
   }
   //-----------------------------------------------------------------------
   // Get IP address
   if( !gpioRead( TEC4 ) ){
      gpioWrite(LED3,ON);
      delay(500);   
      printf("WIFI_MODULE_IP_GET_STARTED\r\n");
      wifiModuleStartIpGet();
      ip[0] = '0';
      sentGetIp = true;
      gpioWrite(LED3,OFF);
   }
   if ( sentGetIp ) {
      wifiModuleRequestResult_t res = wifiModuleIpGetResponse( ip );
      while( res != WIFI_MODULE_IP_GET_COMPLETE &&
             res != WIFI_MODULE_NOT_DETECTED ) {
         res = wifiModuleIpGetResponse( ip );
         if( !gpioRead( TEC1 ) ) {
            delay(500);
            sentGetIp = false;
            break;
         }
      }
      if ( res == WIFI_MODULE_IP_GET_COMPLETE ) {
         printf("WIFI_MODULE_IP_GET_COMPLETE\r\n");
         printf("IP: %s\r\n", ip);
      } else if ( res ==  WIFI_MODULE_NOT_DETECTED ) {
         printf("WIFI_MODULE_NOT_DETECTED\r\n");
      }
      sentGetIp = false;
   }
   //-----------------------------------------------------------------------
}

/*==================[end of file]============================================*/