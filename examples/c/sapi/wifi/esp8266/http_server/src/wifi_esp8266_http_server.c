/* Copyright 2016, Marcelo Vieytes.
 * Copyright 2017, Pablo Gomez - Agustin Bassi.
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
#include "sapi_esp8266.h"
#include "sapi_stdio.h"

/*==================[macros and definitions]=================================*/

// Se deben definir los datos del nombre de la red y la contrasenia.
//#define WIFI_SSID                  "miWifi"     // Setear Red Wi-Fi
//#define WIFI_PASSWORD              "miPassWifi" // Setear password
#define WIFI_SSID                  "Wifi SweetHome"
#define WIFI_PASSWORD              "CeMaThBe09241727"

// El maximo tiempo que se espera una respuesta del modulo ESP8266
#define WIFI_MAX_DELAY    60000

// El inicio y fin HTML le dan formato al texto enviado por el usuario
#define BEGIN_USER_LINE   "<h3 style=\"text-align: center;\"><strong>"
#define END_USER_LINE     "</strong></h3>"

/*==================[internal functions declaration]=========================*/

/*==================[internal data definition]===============================*/

const char HttpWebPageHeader [] =
   "<!DOCTYPE HTML>"
   "<html>"
   "<head><title>EDU-CIAA NXP</title>"
   "<meta http-equiv=\"refresh\" content=\"15\">"
   "</head>"
   "<body bgcolor=\"#E2E1E3\">"
   "<p style=\"text-align: center;\">&nbsp;</p>"
   "<p style=\"text-align: center;\"><span style=\"color: #0000ff;\"><strong><img src=\"https://encrypted-tbn0.gstatic.com/images?q=tbn:ANd9GcT2moZZ0qZ5Az_Qt4kRv6NsJtIxKJl8phyn0APAyfsshhpvpjPs\" alt=\"Logo\" width=\"135\" height=\"135\" /></strong></span></p>"
   "<h1 style=\"text-align: center;\"><span style=\"color: #0000ff;\"><strong>Servidor Web HTTP<br />ESP8266 - EDU CIAA NXP</strong></span></h1>"
   "<h4 style=\"text-align: center;\"><strong><em>En esta pagina web se muestran datos provenientes de la EDU-CIAA NXP</em></strong><br /><strong><em>enviados cuando un cliente HTTP como esta pagina realiza una peticion al servidor</em></strong></h4>"
   "<p style=\"text-align: center;\">INICIO USER HTML</p>"
   "<p style=\"text-align: center;\">&nbsp;</p>"
   "<p style=\"text-align: center;\">&nbsp;</p>"
   ;

const char HttpWebPageEnd [] =
   "<p style=\"text-align: center;\">&nbsp;</p>"
   "<p style=\"text-align: center;\">&nbsp;</p>"
   "<p style=\"text-align: center;\">FIN USER HTML</p>"
   "<hr />"
   "<p style=\"text-align: center;\"><em>Copyright&nbsp;Agustin Bassi -&nbsp;</em><em>Pablo Gomez</em></p>"
   "<p style=\"text-align: center;\">CURSOS INET 2017</p>"
   "<p style=\"text-align: center;\"><a href=\"http://www.proyecto-ciaa.com.ar\">www.proyecto-ciaa.com.ar</a></p>"
   "</body>"
   "</html>"
   ;

char HttpWebPageBody [200];

/*==================[external data definition]===============================*/

/*==================[internal functions definition]==========================*/

/*==================[external functions definition]==========================*/

/* FUNCION PRINCIPAL, PUNTO DE ENTRADA AL PROGRAMA LUEGO DE RESET. */
int main(void)
{
   bool_t error;
   uint8_t counter = 0;
   delay_t wifiDelay;

   // Inicializar la placa
   boardConfig ();
   // Configura la UART USB a 115200 baudios
   uartConfig (UART_USB, 115200);
   // Envia un mensaje de bienvenida.
   stdioPrintf(UART_USB, "\n\rBienvenido al servidor HTTP Esp8266 con EDU CIAA");
   stdioPrintf(UART_USB, "\n\rLa configuracion puede tardar hasta 1 minuto.");

   error = FALSE;
   // Configura un delay para salir de la configuracion en caso de error.
   delayConfig(&wifiDelay, WIFI_MAX_DELAY);

   // Mientras no termine la configuracion o mientras no pase el tiempo maximo, ejecuta la configuracion.
   // A la configuracion se le pasa nombre y contrasenia de RED
   while (!esp8266ConfigHttpServer(WIFI_SSID, WIFI_PASSWORD) && !error) {
      if (delayRead(&wifiDelay)) {
         error = TRUE;
      }
   }

   // Avisa al usuario como salio la configuracion
   if (!error) {
      stdioPrintf(UART_USB, "\n\rServidor HTTP configurado. IP: %s", esp8266GetIpAddress());
      // Enciende LEDG indicando que el modulo esta configurado.
      gpioWrite(LEDG, TRUE);
   } else {
      stdioPrintf(UART_USB, "\n\rError al configurar servidor HTTP.");
      // Enciende LEDR indicando que el modulo esta en error.
      gpioWrite(LEDR, TRUE);
   }

   /* ------------- REPETIR POR SIEMPRE ------------- */
   while(1) {
      // Si llego una peticion al servidor http
      if (esp8266ReadHttpServer()) {

         // Los datos a enviar a la web deben estar en formato HTML. Notar que
         // BEGIN_USER_LINE y END_USER_LINE solo formatean la cadena a enviar con tags HTML.
         stdioSprintf(HttpWebPageBody, "%s VALOR CONTADOR: %d - VALOR ADC: %d %s", BEGIN_USER_LINE, counter, 3+counter*2, END_USER_LINE);

         error = FALSE;
         // Configura un delay para salir de la configuracion en caso de error.
         delayConfig(&wifiDelay, WIFI_MAX_DELAY);

         // Mientras no termine el envio o mientras no pase el tiempo maximo, ejecuta el envio.
         while (!esp8266WriteHttpServer(HttpWebPageHeader, HttpWebPageBody, HttpWebPageEnd) && !error) {
            if (delayRead(&wifiDelay)) {
               error = TRUE;
            }
         }

         // Avisa al usuario como fue el envio
         if (!error) {
            stdioPrintf(UART_USB, "\n\rPeticion respondida al cliente HTTP %d.", esp8266GetConnectionId());
            gpioToggle(LEDG);
         } else {
            stdioPrintf(UART_USB, "\n\rPeticion no respondida al cliente HTTP %d.", esp8266GetConnectionId());
            gpioToggle(LEDR);
         }

         counter++;
      }
   }

   /* NO DEBE LLEGAR NUNCA AQUI, debido a que a este programa no es llamado
      por ningun S.O. */
   return 0 ;
}

/*==================[end of file]============================================*/
