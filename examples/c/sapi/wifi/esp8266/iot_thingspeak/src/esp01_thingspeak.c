/* Copyright 2015-2017, Eric Pernia.
   All rights reserved.

This file is part sAPI library for microcontrollers.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice,
   this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice,
   this list of conditions and the following disclaimer in the documentation
   and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its
   contributors may be used to endorse or promote products derived from this
   software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
POSSIBILITY OF SUCH DAMAGE.
 */

// Date: 2017-12-14

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

/*==================[inlcusiones]============================================*/

#include "sapi.h"        // <= Biblioteca sAPI
#include <string.h>   // <= Biblioteca de manejo de Strings, ver:
// https://es.wikipedia.org/wiki/String.h
// http://www.alciro.org/alciro/Programacion-cpp-Builder_12/funciones-cadenas-caracteres-string.h_448.htm

/*==================[definiciones y macros]==================================*/

// UART list:
//  - UART_GPIO or UART_485
//  - UART_USB or UART_ENET
//  - UART_232

#define UART_DEBUG                 UART_USB
#define UART_ESP01                 UART_232
#define UARTS_BAUD_RATE            115200

#define ESP01_RX_BUFF_SIZE         1024

#define WIFI_SSID                  "miWifi"     // Setear Red Wi-Fi
#define WIFI_PASSWORD              "miPassWifi" // Setear password

#define THINGSPEAK_SERVER_URL      "api.thingspeak.com"
#define THINGSPEAK_SERVER_PORT     80
#define THINGSPEAK_WRITE_API_KEY   "7E7IOJ276BSDLOBA"
#define THINGSPEAK_FIELD_NUMBER    1

/*==================[definiciones de datos internos]=========================*/

CONSOLE_PRINT_ENABLE
DEBUG_PRINT_ENABLE

/*==================[definiciones de datos externos]=========================*/

/*==================[declaraciones de funciones internas]====================*/

/*==================[declaraciones de funciones externas]====================*/

// ESP01 Rx Buffer
char espResponseBuffer[ ESP01_RX_BUFF_SIZE ];
uint32_t espResponseBufferSize = ESP01_RX_BUFF_SIZE;

// UARTs
uartMap_t uartEsp01 = UART_232;
uartMap_t uartDebug = UART_USB;

char tcpIpDataToSend[100];
int64_t sensorValue = 0;


bool_t esp01Init( uartMap_t uartForEsp, uartMap_t uartForDebug, uint32_t baudRate );

void esp01CleanRxBuffer( void );

bool_t esp01ShowWiFiNetworks( void );

bool_t esp01ConnectToWifiAP( char* wiFiSSID, char* wiFiPassword );

bool_t esp01ConnectToServer( char* url, uint32_t port );

bool_t esp01SendTCPIPData( char* strData, uint32_t strDataLen );

bool_t esp01SendTPCIPDataToServer( char* url, uint32_t port, char* strData, uint32_t strDataLen );


void esp01CleanRxBuffer( void ){
   espResponseBufferSize = ESP01_RX_BUFF_SIZE;
   memset( espResponseBuffer, 0, espResponseBufferSize );
}

// AT+CIPSTART="TCP","api.thingspeak.com",80
bool_t esp01SendTPCIPDataToServer( char* url, uint32_t port, char* strData, uint32_t strDataLen ){

   // Enviar dato "data" al servidor "url", puerto "port".
   debugPrintlnString( ">>>> ===========================================================" );
   debugPrintString( ">>>> Enviar dato: \"" );
   debugPrintString( strData );
   debugPrintString( "\"\r\n>>>> al servidor \"" );
   debugPrintString( url );
   debugPrintString( "\", puerto \"" );
   debugPrintInt( port );
   debugPrintlnString( "\"..." );
   debugPrintEnter();

   // AT+CIPSTART="TCP","url",port ---------------------------
   if( !esp01ConnectToServer( url, port ) )
      return FALSE;

   // Ejemplo:
   // AT+CIPSEND=47 ------------------------------------------
   // GET /update?api_key=7E7IOJ276BSDLOBA&field1=66 ---------
   if( !esp01SendTCPIPData( strData, strDataLen ) )
      return FALSE;

   return TRUE;
}



// AT+CIPSEND=39
// GET /update?key=7E7IOJ276BSDLOBA&1=69
bool_t esp01SendTCPIPData( char* strData, uint32_t strDataLen ){

   bool_t retVal = FALSE;

   // "GET /update?key=7E7IOJ276BS\"DL\"OBA&1=69"
   // AT+CIPSEND=strDataLen strData --------------------------

   // Limpiar Buffer (es necesario antes de usar
   // "receiveBytesUntilReceiveStringOrTimeoutBlocking")
   esp01CleanRxBuffer();

   // Envio datos TCP/IP al servidor.
   debugPrintlnString( ">>>> Envio datos TCP/IP al servidor..." );

   debugPrintString( ">>>> AT+CIPSEND=" );
   debugPrintInt( strDataLen + 2 ); // El mas 2 es del \r\n
   debugPrintString( "\r\n" );

   consolePrintString( "AT+CIPSEND=" );
   consolePrintInt( strDataLen + 2 ); // El mas 2 es del \r\n
   consolePrintString( "\r\n" );

   // No poner funciones entre el envio de comando y la espera de respuesta
   retVal = receiveBytesUntilReceiveStringOrTimeoutBlocking(
               uartEsp01,
               "\r\n\r\nOK\r\n>", strlen("\r\n\r\nOK\r\n>"),
               espResponseBuffer, &espResponseBufferSize,
               5000
            );
   if( retVal ){

      // Imprimo todo lo recibido
      debugPrintString( espResponseBuffer );

      // strData\r\n --------------------------------------------

      // Limpiar Buffer (es necesario antes de usar
      // "receiveBytesUntilReceiveStringOrTimeoutBlocking")
      esp01CleanRxBuffer();

      // Envio los datos TCP/IP ------------------
      consolePrintString( strData );
      consolePrintString( "\r\n" );

      // No poner funciones entre el envio de comando y la espera de respuesta
      retVal = receiveBytesUntilReceiveStringOrTimeoutBlocking(
                  uartEsp01,
                  "SEND OK\r\n", 9,
                  espResponseBuffer, &espResponseBufferSize,
                  5000
               );
      if( retVal ){

         // Imprimo todo lo recibido
         debugPrintString( espResponseBuffer );

         // Limpiar Buffer (es necesario antes de usar
         // "receiveBytesUntilReceiveStringOrTimeoutBlocking")
         esp01CleanRxBuffer();

         // Envio los datos TCP/IP ------------------
         consolePrintlnString( strData );

         // No poner funciones entre el envio de comando y la espera de respuesta
         retVal = receiveBytesUntilReceiveStringOrTimeoutBlocking(
                     uartEsp01,
                     "CLOSED\r\n", 8,
                     espResponseBuffer, &espResponseBufferSize,
                     5000
                  );

         if( retVal ){

            // DATO RECIBIDOOOOOOOOOOO -----------------

            // Imprimo todo lo recibido
            debugPrintString( espResponseBuffer );



         } else{
            debugPrintlnString( ">>>> Error al enviar los datos TCP/IP, en el envio del string" );
            debugPrintlnString( ">>>> \"strData\", cuando el ESP01 pone el prompt > " );
            debugPrintlnString( ">>>> y no se recibe la respuesta y \"CLOSED\"!!\r\n" );

            // Imprimo todo lo recibido
            debugPrintString( espResponseBuffer );
         }


      } else{
         debugPrintlnString( ">>>> Error al enviar los datos TCP/IP, en el envio del string" );
         debugPrintlnString( ">>>> \"strData\", cuando el ESP01 pone el prompt > " );
         debugPrintlnString( ">>>> y no se recibe \"SEND OK\"!!\r\n" );

         // Imprimo todo lo recibido
         debugPrintString( espResponseBuffer );
      }

   } else{
      debugPrintlnString( ">>>> Error al enviar los datos TCP/IP, en comando" );
      debugPrintlnString( ">>>> \"AT+CIPSEND\"!!\r\n" );
      // Imprimo todo lo recibido
      debugPrintString( espResponseBuffer );
   }
   return retVal;
}


// AT+CIPSTART="TCP","api.thingspeak.com",80
bool_t esp01ConnectToServer( char* url, uint32_t port ){

   bool_t retVal = FALSE;

   // AT+CIPSTART="TCP","url",port ---------------------------

   // Limpiar Buffer (es necesario antes de usar
   // "receiveBytesUntilReceiveStringOrTimeoutBlocking")
   esp01CleanRxBuffer();

   debugPrintString( ">>>> Conectando al servidor \"" );
   debugPrintString( url );
   debugPrintString( "\", puerto \"" );
   debugPrintInt( port );
   debugPrintlnString( "\"..." );

   debugPrintString( ">>>> AT+CIPSTART=\"TCP\",\"" );
   debugPrintString( url );
   debugPrintString( "\"," );
   debugPrintInt( port );
   debugPrintString( "\r\n" );

   consolePrintString( "AT+CIPSTART=\"TCP\",\"" );
   consolePrintString( url );
   consolePrintString( "\"," );
   consolePrintInt( port );
   consolePrintString( "\r\n" );

   // No poner funciones entre el envio de comando y la espera de respuesta
   retVal = receiveBytesUntilReceiveStringOrTimeoutBlocking(
               uartEsp01,
               "CONNECT\r\n\r\nOK\r\n", 15,
               espResponseBuffer, &espResponseBufferSize,
               10000
            );
   if( !retVal ){
      debugPrintString( ">>>>    Error: No se puede conectar al servidor: \"" );
      debugPrintlnString( url );
      debugPrintString( "\"," );
      debugPrintInt( port );
      debugPrintlnString( "\"!!\r\n" );
   }
   // Imprimo todo lo recibido
   debugPrintString( espResponseBuffer );
   return retVal;
}


bool_t esp01ConnectToWifiAP( char* wiFiSSID, char* wiFiPassword ){

   bool_t retVal = FALSE;
   char* index = 0;

   // AT+CWJAP="wiFiSSID","wiFiPassword" ---------------------

   // Limpiar Buffer (es necesario antes de usar
   // "receiveBytesUntilReceiveStringOrTimeoutBlocking")
   esp01CleanRxBuffer();

   // Conectar a la red Wi-Fi. se envia AT+CWJAP="wiFiSSID","wiFiPassword"
   debugPrintString( ">>>> Conectando a la red Wi-Fi: \"" );
   debugPrintString( wiFiSSID );
   debugPrintlnString( "\"..." );

   consolePrintString( "AT+CWJAP=\"" );
   consolePrintString( wiFiSSID );
   consolePrintString( "\",\"" );
   consolePrintString( wiFiPassword );
   consolePrintString( "\"\r\n" );

   // No poner funciones entre el envio de comando y la espera de respuesta
   retVal = receiveBytesUntilReceiveStringOrTimeoutBlocking(
               uartEsp01,
               "WIFI CONNECTED\r\nWIFI GOT IP\r\n\r\nOK\r\n", 35,
               espResponseBuffer, &espResponseBufferSize,
               10000
            );
   if( retVal ){

      // Imprimo todo lo recibido filtrando la parte que muestra el password, llega:

      // AT+CWJAP="wiFiSSID","wiFiPassword"
      //
      // WIFI DISCONNECT ----> Opcional
      // WIFI CONNECTED
      // WIFI GOT IP
      //
      // OK

      // Pero imprimo:

      // WIFI CONNECTED
      // WIFI GOT IP
      //
      // OK

      index = strstr( (const char*)espResponseBuffer, (const char*)"WIFI CONNECTED" );
      if( index != 0 ){
         // Muestro desde " WIFI CONNECTED" en adelante
         debugPrintString( index );
      } else{
         // Muestro todo en caso de error
         debugPrintString( espResponseBuffer );
      }
   } else{
      debugPrintString( ">>>>    Error: No se puede conectar a la red: \"" );
      debugPrintlnString( wiFiSSID );
      debugPrintlnString( "\"!!\r\n" );

      // Muestro todo en caso de error
      debugPrintString( espResponseBuffer );
   }
   return retVal;
}


bool_t esp01ShowWiFiNetworks( void ){

   bool_t retVal = FALSE;

   // AT+CWLAP -----------------------------------------------

   // Limpiar Buffer (es necesario antes de usar
   // "receiveBytesUntilReceiveStringOrTimeoutBlocking")
   esp01CleanRxBuffer();

   // Mostrar lista de AP enviando "AT+CWLAP"
   debugPrintlnString( ">>>> Consultando las redes Wi-Fi disponibles.\r\n>>>>    Enviando \"AT+CWLAP\"..." );
   consolePrintString( "AT+CWLAP\r\n" );
   // No poner funciones entre el envio de comando y la espera de respuesta
   retVal = receiveBytesUntilReceiveStringOrTimeoutBlocking(
               uartEsp01,
               ")\r\n\r\nOK\r\n", 9,
               espResponseBuffer, &espResponseBufferSize,
               20000
            );
   if( !retVal ){
      debugPrintlnString( ">>>>    Error: No se encuentran redes disponibles!!\r\n" );
   }
   // Imprimo todo lo recibido
   debugPrintString( espResponseBuffer );
   return retVal;
}


bool_t esp01Init( uartMap_t uartForEsp, uartMap_t uartForDebug, uint32_t baudRate ){

   bool_t retVal = FALSE;

   uartEsp01 = uartForEsp;
   uartDebug = uartForDebug;

   // Initialize HW ------------------------------------------

   // Inicializar UART_USB como salida de debug
   debugPrintConfigUart( uartDebug, baudRate );
   debugPrintlnString( ">>>> UART_USB configurada como salida de debug." );

   // Inicializr otra UART donde se conecta el ESP01 como salida de consola
   consolePrintConfigUart( uartEsp01, baudRate );
   debugPrintlnString( ">>>> UART_ESP (donde se conecta el ESP01), \r\n>>>> configurada como salida de consola.\r\n" );

   // AT -----------------------------------------------------

   // Chequear si se encuentra el modulo Wi-Fi enviandole "AT"
   debugPrintlnString( ">>>> Chequear si se encuentra el modulo Wi-Fi.\r\n>>>>    Enviando \"AT\"..." );
   consolePrintString( "AT\r\n" );
   // No poner funciones entre el envio de comando y la espera de respuesta
   retVal = waitForReceiveStringOrTimeoutBlocking( uartEsp01, "AT\r\n", 4, 500 );
   if( retVal ){
      debugPrintlnString( ">>>>    Modulo ESP01 Wi-Fi detectado.\r\n" );
   } else{
      debugPrintlnString( ">>>>    Error: Modulo ESP01 Wi-Fi No detectado!!\r\n" );
      return retVal;
   }

   // AT+CWLAP -----------------------------------------------
   return esp01ShowWiFiNetworks();
}


void stopProgramError( void ){
   // Si hay un error grave me quedo en un bucle infinito
   // en modo bajo consumo
   while( TRUE ){
      sleepUntilNextInterrupt();
   }
}

/*==================[funcion principal]======================================*/

// FUNCION PRINCIPAL, PUNTO DE ENTRADA AL PROGRAMA LUEGO DE ENCENDIDO O RESET.
int main( void ){

   // ---------- CONFIGURACIONES ------------------------------

   // Inicializar y configurar la plataforma
   boardConfig();

   // Inicializar UART_USB como salida de consola
   debugPrintConfigUart( UART_DEBUG, UARTS_BAUD_RATE );
   
   if( !esp01Init( UART_ESP01, UART_DEBUG, UARTS_BAUD_RATE ) ){
      stopProgramError(); // Como dio falso (error) me quedo en un bucle infinito
   }

   if( !esp01ConnectToWifiAP( WIFI_SSID, WIFI_PASSWORD ) ){
      stopProgramError(); // Como dio falso (error) me quedo en un bucle infinito
   }

   // ---------- REPETIR POR SIEMPRE --------------------------
   while( TRUE )
   {
      // Armo el dato a enviar, en este caso para grabar un dato en el canal de Thinspeak
      // Ejemplo: "GET /update?api_key=7E7IOJ276BSDLOBA&field1=66"

      tcpIpDataToSend[0] = 0; // Reseteo la cadena que guarda las otras agregando un caracter NULL al principio

      strcat( tcpIpDataToSend, "GET /update?api_key=" );     // Agrego la peticion de escritura de datos

      strcat( tcpIpDataToSend, THINGSPEAK_WRITE_API_KEY );   // Agrego la clave de escritura del canal

      strcat( tcpIpDataToSend, "&field" );                   // Agrego field del canal
      strcat( tcpIpDataToSend, intToStringGlobal(THINGSPEAK_FIELD_NUMBER) );

      strcat( tcpIpDataToSend, "=" );                        // Agrego el valor a enviar
      strcat( tcpIpDataToSend, intToStringGlobal( sensorValue ) );

      // Envio los datos TCP/IP al Servidor de Thingpeak
      // Ver en: https://thingspeak.com/channels/377497/
      esp01SendTPCIPDataToServer( THINGSPEAK_SERVER_URL, THINGSPEAK_SERVER_PORT,
                                  tcpIpDataToSend, strlen( tcpIpDataToSend ) );

      sensorValue++;
      if( sensorValue > 10 ){
         sensorValue = 0;
      }

      delay(20000);
   }

   // NO DEBE LLEGAR NUNCA AQUI, debido a que a este programa se ejecuta
   // directamenteno sobre un microcontroladore y no es llamado por ningun
   // Sistema Operativo, como en el caso de un programa para PC.
   return 0;
}

/*==================[definiciones de funciones internas]=====================*/

/*==================[definiciones de funciones externas]=====================*/

/*==================[fin del archivo]========================================*/
