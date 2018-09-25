/* Copyright 2017, Pablo Gomez - Agustin Bassi.
 * Copyright 2016, Marcelo Vieytes.
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

/* Date: 2017-14-11 */

/*==================[inlcusiones]============================================*/

#include "sapi.h"
#include "sapi_esp8266.h"
#include <string.h>
#include "sapi_stdio.h"

/*==================[definiciones y macros]==================================*/

#define ESP8266_UART          UART_232
#define ESP8266_BAUD_RATE     115200
#define ESP8266_TMO           5000
#define ESP8266_PAUSE         5000
#define ESP8266_WAIT          1000

#define MAX_COMMAND_LENGHT	   40
#define MAX_HTTP_WEB_LENGHT   1500

typedef enum Esp8266State {
   ESP_INIT,
   ESP_SEND_AT,
   ESP_WAIT_AT,
   ESP_SEND_CWJAP_CONS,
   ESP_WAIT_CWJAP_CONS_1,
   ESP_WAIT_CWJAP_CONS_2,
   ESP_SEND_CWMODE_SET,
   ESP_WAIT_CWMODE_SET,
   ESP_SEND_CWJAP_SET,
   ESP_WAIT_CWJAP_SET_1,
   ESP_WAIT_CWJAP_SET_2,
   ESP_WAIT_CWJAP_SET_3,
   ESP_CONNECTED,
   ESP_SEND_START,
   ESP_WAIT_START_1,
   ESP_WAIT_START_2,
   ESP_SEND_SEND,
   ESP_WAIT_SEND,
   ESP_SEND_DATA,
   ESP_WAIT_DATA_1,
   ESP_WAIT_DATA_2,
   ESP_CIPMUX_SEND,
   ESP_WAIT_CIPMUX,
   ESP_SEND_CIPSERVER,
   ESP_WAIT_CIPSERVER,
   ESP_WAIT_GET,
   ESP_SEND_CIPSTATUS,
   ESP_WAIT_CIPSTATUS_3,
   ESP_WAIT_GET_ID,
   ESP_SEND_CIPSEND,
   ESP_WAIT_CIPSEND,
   ESP_SEND_HTTP,
   ESP_WAIT_HTTP,
   ESP_WAIT_CIPSTATUS_OK,
   ESP_WAIT_CIPSTATUS,
   ESP_SEND_CIPCLOSE,
   ESP_WAIT_CIPCLOSE,
   ESP_SEND_CIFSR,
   ESP_WAIT_CIFSR,
   ESP_LOAD_IP,
   ESP_SEND_CWMODE,
   ESP_WAIT_CWMODE
} Esp8266Status_t;

/*==================[definiciones de datos internos]=========================*/

//Nombres de los estados en forma de strings para imprimir por pantalla.
static const char Esp8266StatusToString[][MAX_COMMAND_LENGHT] = {
   "ESP_INIT",
   "ESP_SEND_AT",
   "ESP_WAIT_AT",
   "ESP_SEND_CWJAP_CONS",
   "ESP_WAIT_CWJAP_CONS_1",
   "ESP_WAIT_CWJAP_CONS_2",
   "ESP_SEND_CWMODE_SET",
   "ESP_WAIT_CWMODE_SET",
   "ESP_SEND_CWJAP_SET",
   "ESP_WAIT_CWJAP_SET_1",
   "ESP_WAIT_CWJAP_SET_2",
   "ESP_WAIT_CWJAP_SET_3",
   "ESP_CONNECTED",
   "ESP_SEND_START",
   "ESP_WAIT_START_1",
   "ESP_WAIT_START_2",
   "ESP_SEND_SEND",
   "ESP_WAIT_SEND",
   "ESP_SEND_DATA",
   "ESP_WAIT_DATA_1",
   "ESP_WAIT_DATA_2",
   "ESP_CIPMUX_SEND",
   "ESP_WAIT_CIPMUX",
   "ESP_SEND_CIPSERVER",
   "ESP_WAIT_CIPSERVER",
   "ESP_WAIT_GET",
   "ESP_SEND_CIPSTATUS",
   "ESP_WAIT_CIPSTATUS_3",
   "ESP_WAIT_GET_ID",
   "ESP_SEND_CIPSEND",
   "ESP_WAIT_CIPSEND",
   "ESP_SEND_HTTP",
   "ESP_WAIT_HTTP",
   "ESP_WAIT_CIPSTATUS_OK",
   "ESP_WAIT_CIPSTATUS",
   "ESP_SEND_CIPCLOSE",
   "ESP_WAIT_CIPCLOSE",
   "ESP_SEND_CIFSR",
   "ESP_WAIT_CIFSR",
   "ESP_LOAD_IP",
   "ESP_SEND_CWMODE",
   "ESP_WAIT_CWMODE"
};
// Respuestas del ESP8266
static const char Response_OK[] = "OK";
static const char Response_CWJAP_OK[] = "+CWJAP:";
static const char Response_CWJAP_1[] = "WIFI CONNECTED";
static const char Response_CWJAP_2[] = "WIFI GOT IP";
static const char Response_SEND_OK[] = "SEND OK";
static const char Response_STATUS_3[] = "STATUS:3";
static const char Response_CIPSTATUS[] = "+CIPSTATUS:";
static const char Response_CIPCLOSE[] = "CLOSED";
static const char Response_CIFSR[] = "+CIFSR:STAIP,\"";
// Memoria asociada a las conexiones
static uint8_t CurrentConnectionId;
static char WifiName [30] = "";
static char WifiPass [30] = "";
static char WifiIp   [20];
// Punteros a la pagina web a mostrar
static char * PointerOfHttpBody;
static const char * PointerOfHttpHeader;
static const char * PointerOfHttpEnd;
// Variables utilizadas en la maquina de estados.
static const char * Esp8266ResponseToWait;
static delay_t Esp8266Delay;
static uint8_t Esp8266Status = ESP_INIT;
static uartMap_t Esp8266DebugUart = UART_485;
static uint32_t Esp8266DebugBaudRate = 0;

/*==================[definiciones de datos externos]=========================*/

/*==================[declaraciones de funciones internas]====================*/

static bool_t IsWaitedResponse(void);
static void SetEsp8622Status(uint8_t statusToSend);
static void ExcecuteHttpServerFsm(void);

/*==================[declaraciones de funciones externas]====================*/

/**
 * Funcion para obtener la direccion IP del modulo Esp8266
 * @return devuelve un puntero al arreglo local donde esta alojado el dato.
 */
char * esp8266GetIpAddress()
{
   return WifiIp;
}

/**
 * Funcion para obtener el nombre de la red del modulo Esp8266
 * @return devuelve un puntero al arreglo local donde esta alojado el dato.
 */
char * esp8266GetWifiName()
{
   return WifiName;
}

/**
 * Funcion para obtener la contrasenia de la red del modulo Esp8266
 * @return devuelve un puntero al arreglo local donde esta alojado el dato.
 */
char * esp8266GetWifiPass()
{
   return WifiPass;
}

/**
 * Devuelve al usuario el indice de la conexion establecida
 * @return un entero correspondiente a
 */
uint8_t esp8266GetConnectionId()
{
   return CurrentConnectionId - '0';
}

/**
 * Configura la conexion para que el modulo Esp8266 sea un servidor HTTP.
 * Realiza llamadas no bloqueantes a la maquina de estados que maneja la conexion.
 * La variable parametersReceived sirve para cargar por unica vez los datos de la red
 * @param wifiName puntero con el nombre de la red Wifi
 * @param wifiPass puntero con la contrasenia de la red Wifi
 * @param debugUart es la uart por donde va a reportar los datos. Este parametro es opcional, poner 0 sino se necesita.
 * @param debugBaudRate es la velocidad de la uart de debug. Poner en 0 si no se necesita.
 * @return TRUE si se configuro correctamente como servidor HTTP, FALSE caso contrario.
 */
bool_t esp8266InitHttpServer(char * wifiName, char * wifiPass)
{
   static bool_t parametersReceived = FALSE;

   if (!parametersReceived) {
      strcpy(WifiName, wifiName);
      strcpy(WifiPass, wifiPass);
      SetEsp8622Status(ESP_INIT);
      parametersReceived = TRUE;
   }
   ExcecuteHttpServerFsm();

   return (Esp8266Status == ESP_SEND_CIPSTATUS);
}

/**
 * Funcion para determinar si hay alguna peticion HTTP realizada desde una
 * pagina web, realizada por algun cliente.
 * Ademas realiza llamadas no bloqueantes a la maquina de estados del
 * modulo que administra las conexiones y peticiones de clientes.
 * @return TRUE si se recibio una peticion, FALSE caso contrario.
 */
bool_t esp8266ReadHttpServer()
{
   ExcecuteHttpServerFsm();
   return (Esp8266Status == ESP_SEND_CIPSEND);
}

/**
 * Funcion para enviar una pagina web actualizada en respuesta a la
 * peticion del cliente.
 * Corrobora quue haya una peticion real del usuario antes de enviar datos.
 * Si hay una peticion, carga en los punteros correspondientes toda la
 * pagina web que se le enviara al cliente.
 * Un detalle importante es que tanto el header como el end de la web son
 * constantes, ya que se supone que no cambian a lo largo del programa.
 * Lo que si cambiara es el body HTTP que es el que contiene la informacion
 * actualizada del estado del sistema.
 * @param webHttpHeader puntero al header http (debe ser parte de la aplicacion de usuario).
 * @param webHttpBody puntero al body http (debe ser parte de la aplicacion de usuario).
 * @param webHttpEnd puntero al end http (debe ser parte de la aplicacion de usuario).
 * @return TRUE si pudo mandar la web correctamente, FALSE caso contrario.
 */
bool_t esp8266WriteHttpServer(const char * webHttpHeader, char * webHttpBody, const char * webHttpEnd)
{
   //antes de enviar se asegura que haya datos pendientes para enviar
   if (Esp8266Status == ESP_SEND_CIPSEND) {
      PointerOfHttpHeader = webHttpHeader;
      PointerOfHttpBody   = webHttpBody;
      PointerOfHttpEnd    = webHttpEnd;
   }
   ExcecuteHttpServerFsm();

   return (Esp8266Status == ESP_SEND_CIPCLOSE);
}

/*==================[definiciones de funciones internas]=====================*/

/**
 * Funcion principal del modulo Wifi Esp8266 para funcionar como servidor HTTP.
 * Desde aca se manejan los comandos a enviar, los tiempos a esperar y
 * las respuestas a recibir.
 * Automaticamente cambia de estados en funcion de los eventos que ocurran.
 */
static void ExcecuteHttpServerFsm(void)
{
   uint16_t lenghtOfHttpLines;
   static uint8_t byteReceived, auxIndex;

   switch (Esp8266Status) {

      case ESP_INIT:
         uartConfig(ESP8266_UART, ESP8266_BAUD_RATE);
         if (Esp8266DebugBaudRate > 0) {
            uartConfig(Esp8266DebugUart, Esp8266DebugBaudRate);
         }
         delayConfig(&Esp8266Delay, ESP8266_PAUSE);
         SetEsp8622Status(ESP_SEND_AT);
      break;

      case ESP_SEND_AT:
         if (delayRead(&Esp8266Delay)) {
            stdioPrintf(ESP8266_UART, "AT\r\n");
            Esp8266ResponseToWait = Response_OK;
            delayConfig(&Esp8266Delay, ESP8266_TMO);
            SetEsp8622Status(ESP_WAIT_AT);
         }
      break;

      case ESP_WAIT_AT:
         if (IsWaitedResponse()) {
            delayConfig(&Esp8266Delay, ESP8266_PAUSE);
            SetEsp8622Status(ESP_SEND_CWMODE);
         }
         //Si no recibe OK vuelve a enviar AT
         if (delayRead(&Esp8266Delay)) {
            delayConfig(&Esp8266Delay, ESP8266_PAUSE);
            SetEsp8622Status(ESP_SEND_AT);
         }
      break;

      case ESP_SEND_CWMODE:
         if (delayRead(&Esp8266Delay)) {
            stdioPrintf(ESP8266_UART, "AT+CWMODE=3\r\n");
            Esp8266ResponseToWait = Response_OK;
            delayConfig(&Esp8266Delay, ESP8266_TMO);
            SetEsp8622Status(ESP_WAIT_CWMODE);
         }
      break;

      case ESP_WAIT_CWMODE:
         if (IsWaitedResponse()) {
            delayConfig(&Esp8266Delay, ESP8266_PAUSE);
            SetEsp8622Status(ESP_SEND_CWJAP_CONS);
         }
         //Si no recibe OK vuelve a enviar AT
         if (delayRead(&Esp8266Delay)) {
            delayConfig(&Esp8266Delay, ESP8266_PAUSE);
            SetEsp8622Status(ESP_SEND_CWMODE);
         }
      break;

      case ESP_SEND_CWJAP_CONS:
         if (delayRead(&Esp8266Delay)) {
            stdioPrintf(ESP8266_UART, "AT+CWJAP?\r\n");
            Esp8266ResponseToWait = Response_CWJAP_OK;
            delayConfig(&Esp8266Delay, ESP8266_TMO);
            SetEsp8622Status(ESP_WAIT_CWJAP_CONS_1);
         }
      break;

      case ESP_WAIT_CWJAP_CONS_1:
         if (IsWaitedResponse()) {
            Esp8266ResponseToWait = Response_OK;
            SetEsp8622Status(ESP_WAIT_CWJAP_CONS_2);
         }
         if (delayRead(&Esp8266Delay)) {
            delayConfig(&Esp8266Delay, ESP8266_PAUSE);
            SetEsp8622Status(ESP_SEND_CWMODE_SET);
         }
      break;

      case ESP_WAIT_CWJAP_CONS_2:
         if (IsWaitedResponse()) {
            delayConfig(&Esp8266Delay, ESP8266_PAUSE);
            SetEsp8622Status(ESP_CIPMUX_SEND);
         }
         if (delayRead(&Esp8266Delay)) {
            delayConfig(&Esp8266Delay, ESP8266_PAUSE);
            SetEsp8622Status(ESP_SEND_AT);
         }
      break;

      case ESP_SEND_CWMODE_SET:
         if (delayRead(&Esp8266Delay)) {
            stdioPrintf(ESP8266_UART, "AT+CWMODE=3\r\n");
            Esp8266ResponseToWait = Response_OK;
            delayConfig(&Esp8266Delay, ESP8266_TMO);
            SetEsp8622Status(ESP_WAIT_CWMODE_SET);
         }
      break;

      case ESP_WAIT_CWMODE_SET:
         if (IsWaitedResponse()) {
            delayConfig(&Esp8266Delay, ESP8266_PAUSE);
            SetEsp8622Status(ESP_SEND_CWJAP_SET);
         }
         if (delayRead(&Esp8266Delay)) {
            delayConfig(&Esp8266Delay, ESP8266_PAUSE);
            SetEsp8622Status(ESP_SEND_AT);
         }
      break;

      case ESP_SEND_CWJAP_SET:
         if (delayRead(&Esp8266Delay)) {
            stdioPrintf(ESP8266_UART, "AT+CWJAP=\"%s\",\"%s\"", WifiName, WifiPass);
            Esp8266ResponseToWait = Response_CWJAP_1;
            delayConfig(&Esp8266Delay, ESP8266_TMO);
            SetEsp8622Status(ESP_WAIT_CWJAP_SET_1);
         }
      break;

      case ESP_WAIT_CWJAP_SET_1:
         if (IsWaitedResponse()) {
            Esp8266ResponseToWait = Response_CWJAP_2;
            SetEsp8622Status(ESP_WAIT_CWJAP_SET_2);
         }
         if (delayRead(&Esp8266Delay)) {
            delayConfig(&Esp8266Delay, ESP8266_PAUSE);
            SetEsp8622Status(ESP_SEND_AT);
         }
         break;

      case ESP_WAIT_CWJAP_SET_2:
         if (IsWaitedResponse()) {
            Esp8266ResponseToWait = Response_OK;
            SetEsp8622Status(ESP_WAIT_CWJAP_SET_3);
         }
         if (delayRead(&Esp8266Delay)) {
            delayConfig(&Esp8266Delay, ESP8266_PAUSE);
            SetEsp8622Status(ESP_SEND_AT);
         }
      break;

      case ESP_WAIT_CWJAP_SET_3:
         if (IsWaitedResponse())
            SetEsp8622Status(ESP_CIPMUX_SEND);
         if (delayRead(&Esp8266Delay)) {
            delayConfig(&Esp8266Delay, ESP8266_PAUSE);
            SetEsp8622Status(ESP_SEND_AT);
         }
      break;

      case ESP_CIPMUX_SEND:
         if (delayRead(&Esp8266Delay)) {
            stdioPrintf(ESP8266_UART, "AT+CIPMUX=1\r\n");
            Esp8266ResponseToWait = Response_OK;
            delayConfig(&Esp8266Delay, ESP8266_TMO);
            SetEsp8622Status(ESP_WAIT_CIPMUX);
            auxIndex=0;
         }
      break;

      case ESP_WAIT_CIPMUX:
         if (IsWaitedResponse()) {
            delayConfig(&Esp8266Delay, ESP8266_PAUSE);
            SetEsp8622Status(ESP_SEND_CIPSERVER);
         }
         if (delayRead(&Esp8266Delay)) {
            delayConfig(&Esp8266Delay, ESP8266_PAUSE);
            // cierra todas las posibles conexioes
            stdioPrintf(ESP8266_UART, "AT+CIPCLOSE=%d\r\n", auxIndex);
            if (++auxIndex >= 4) {
               SetEsp8622Status(ESP_CIPMUX_SEND);
            }
         }
      break;

      case ESP_SEND_CIPSERVER:
         if (delayRead(&Esp8266Delay)) {
            stdioPrintf(ESP8266_UART, "AT+CIPSERVER=1,80\r\n");
            Esp8266ResponseToWait = Response_OK;
            delayConfig(&Esp8266Delay, ESP8266_TMO);
            SetEsp8622Status(ESP_WAIT_CIPSERVER);
         }
      break;

      case ESP_WAIT_CIPSERVER:
         if (IsWaitedResponse()) {
            delayConfig(&Esp8266Delay, ESP8266_PAUSE);
            SetEsp8622Status(ESP_SEND_CIFSR);
         }
         if (delayRead(&Esp8266Delay)) {
            delayConfig(&Esp8266Delay, ESP8266_PAUSE);
            SetEsp8622Status(ESP_SEND_AT);
         }
      break;

      case ESP_SEND_CIFSR:
         if (delayRead(&Esp8266Delay)) {
            stdioPrintf(ESP8266_UART, "AT+CIFSR\r\n");
            Esp8266ResponseToWait = Response_CIFSR;
            delayConfig(&Esp8266Delay, ESP8266_PAUSE);
            SetEsp8622Status(ESP_WAIT_CIFSR);
         }
      break;

      case ESP_WAIT_CIFSR:
         if (IsWaitedResponse()) {
            SetEsp8622Status(ESP_LOAD_IP);
            auxIndex=0;
         }
         if (delayRead(&Esp8266Delay)) {
            delayConfig(&Esp8266Delay, ESP8266_PAUSE);
            SetEsp8622Status(ESP_SEND_CIFSR);
         }
      break;

      //Recibe byte a byte la direccion IP y la almacena en WifiIp
      case ESP_LOAD_IP:
         if (uartReadByte(ESP8266_UART, &byteReceived)) {
            if (byteReceived != '"') {
               WifiIp [auxIndex] = byteReceived;
               auxIndex++;
            } else {
               WifiIp [auxIndex] = '\0';
               SetEsp8622Status(ESP_SEND_CIPSTATUS);
            }
         }
      break;

      // En este estado el modulo ya esta configurado como servidor HTTP
      // entonces cada vez que pasa el delay ESP8266_PAUSE mediante
      // el compando CIP_STATUS le pregunta al moduloWifi si hay una nueva peticion
      case ESP_SEND_CIPSTATUS:
         if (delayRead(&Esp8266Delay)) {
            stdioPrintf(ESP8266_UART, "AT+CIPSTATUS\r\n");
            Esp8266ResponseToWait = Response_STATUS_3;
            delayConfig(&Esp8266Delay, ESP8266_PAUSE);
            SetEsp8622Status(ESP_WAIT_CIPSTATUS_3);
         }
      break;

      case ESP_WAIT_CIPSTATUS_3:
         if (IsWaitedResponse()) {
            delayConfig(&Esp8266Delay, ESP8266_PAUSE);
            Esp8266ResponseToWait = Response_CIPSTATUS;
            SetEsp8622Status(ESP_WAIT_CIPSTATUS);
         }
         if (delayRead(&Esp8266Delay)) {
            delayConfig(&Esp8266Delay, ESP8266_PAUSE);
            SetEsp8622Status(ESP_SEND_CIPSTATUS);
         }
      break;

      case ESP_WAIT_CIPSTATUS:
         if (IsWaitedResponse()) {
            SetEsp8622Status(ESP_WAIT_GET_ID);
         }
         if (delayRead(&Esp8266Delay)) {
            delayConfig(&Esp8266Delay, ESP8266_PAUSE);
            SetEsp8622Status(ESP_SEND_CIPSTATUS);
         }
      break;

      case ESP_WAIT_GET_ID:
         if (uartReadByte(ESP8266_UART, &byteReceived)) {
            CurrentConnectionId = byteReceived;
            Esp8266ResponseToWait = Response_OK;
            SetEsp8622Status(ESP_WAIT_CIPSTATUS_OK);
            auxIndex = 0;
         }
      break;

      case ESP_WAIT_CIPSTATUS_OK:
         if (IsWaitedResponse()) {
            SetEsp8622Status(ESP_SEND_CIPSEND);
         }
         if (delayRead(&Esp8266Delay)) {
            delayConfig(&Esp8266Delay, ESP8266_PAUSE);
            SetEsp8622Status(ESP_SEND_CIPSTATUS);
         }
      break;

      //En este estado le dice al modulo cuantos bytes va a mandar
      // El comando esta compuesto por el comando CIPSEND, mas el ID
      // de la conexion abierta, mas la cantidad en ASCII de los bytes
      // que tiene la pagina web (o al menos los bytes de la primer linea).
      case ESP_SEND_CIPSEND:
         lenghtOfHttpLines = (strlen(PointerOfHttpHeader) + strlen(PointerOfHttpBody) + strlen(PointerOfHttpEnd));
         // Si se pasa del maximo largo permitido lo avisa en la web
         if (lenghtOfHttpLines >= MAX_HTTP_WEB_LENGHT) {
            stdioSprintf(PointerOfHttpBody, "ERROR: La longitud de datos HTTP supera el maximo permitido de %d bytes.", MAX_HTTP_WEB_LENGHT);
            lenghtOfHttpLines = (strlen(PointerOfHttpHeader) + strlen(PointerOfHttpBody) + strlen(PointerOfHttpEnd));
         }
         stdioPrintf(ESP8266_UART, "AT+CIPSEND=%c,%d\r\n", CurrentConnectionId, lenghtOfHttpLines);
         SetEsp8622Status(ESP_WAIT_CIPSEND);
         Esp8266ResponseToWait = Response_OK;
      break;

      case ESP_WAIT_CIPSEND:
         if (IsWaitedResponse()) {
            delayConfig(&Esp8266Delay, ESP8266_PAUSE);
            SetEsp8622Status(ESP_SEND_HTTP);
         }
         if (delayRead(&Esp8266Delay)) {
            delayConfig(&Esp8266Delay, ESP8266_PAUSE);
            SetEsp8622Status(ESP_SEND_CIPSTATUS);
         }
      break;

      case ESP_SEND_HTTP:
         stdioPrintf(ESP8266_UART, "%s%s%s", PointerOfHttpHeader, PointerOfHttpBody, PointerOfHttpEnd);
         SetEsp8622Status(ESP_WAIT_HTTP);
         Esp8266ResponseToWait = Response_SEND_OK;
      break;

      case ESP_WAIT_HTTP:
         if (IsWaitedResponse()) {
            delayConfig(&Esp8266Delay, ESP8266_PAUSE);
            SetEsp8622Status(ESP_SEND_CIPCLOSE);
         }
         if (delayRead(&Esp8266Delay)) {
            delayConfig(&Esp8266Delay, ESP8266_PAUSE);
            SetEsp8622Status(ESP_SEND_CIPSEND);
         }
      break;

      case ESP_SEND_CIPCLOSE:
         if (delayRead(&Esp8266Delay)) {
            stdioPrintf(ESP8266_UART, "AT+CIPCLOSE=%c\r\n", CurrentConnectionId);
            Esp8266ResponseToWait  = Response_CIPCLOSE;
            delayConfig(&Esp8266Delay, ESP8266_PAUSE);
            SetEsp8622Status(ESP_WAIT_CIPCLOSE);
         }
      break;

      case ESP_WAIT_CIPCLOSE:
         if (IsWaitedResponse()) {
            delayConfig(&Esp8266Delay, ESP8266_PAUSE);
            SetEsp8622Status(ESP_SEND_CIPSTATUS);
         }
         if (delayRead(&Esp8266Delay)) {
            delayConfig(&Esp8266Delay, ESP8266_PAUSE);
            SetEsp8622Status(ESP_SEND_CIPSTATUS);
         }
      break;
   }
}

/**
 * Determina si la respuesta que envio el modulo Esp8266 al
 * sistema embebido sea correcta.
 * Dentro de esta funcion va leyendo datos de la UART asociada
 * al modulo Esp8266 hasta encontrar el patron de datos necesario.
 * @return TRUE si es la respuesta esperada, FALSE caso contrario.
 */
static bool_t IsWaitedResponse(void)
{
   static uint8_t index = 0;
   uint8_t byteReceived;
   bool_t moduleResponse = FALSE;

   if (uartReadByte(ESP8266_UART, &byteReceived)) {
      if (Esp8266DebugBaudRate > 0) {
         stdioPrintf(Esp8266DebugUart, "%c", byteReceived);
      }
      if (byteReceived == Esp8266ResponseToWait[index]) {
         index++;
         if (Esp8266ResponseToWait[index] == '\0') {
            index = 0;
            moduleResponse = TRUE;
            if (Esp8266DebugBaudRate > 0) {
               stdioPrintf(Esp8266DebugUart, "\n\r", byteReceived);
            }
         }
      } else {
         index = 0;
      }
   }
   return moduleResponse;
}

/**
 * Setea el estado global del modulo Esp8266.
 * @param status Estado a setear.
 */
static void SetEsp8622Status(Esp8266Status_t status)
{
   Esp8266Status = status;
   if (Esp8266DebugBaudRate > 0) {
//		stdioPrintf(Esp8266DebugUart, "\n\rESP8266 State = %s\n\r", Esp8266StatusToString[status]);
   }
}

/*==================[fin del archivo]========================================*/
