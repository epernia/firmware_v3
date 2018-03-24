/* Copyright 2014, ChaN
 * Copyright 2016, Matias Marando
 * Copyright 2016-2017, Eric Pernia
 * All rights reserved.
 *
 * This file is part of sAPI Library.
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

/*==================[inlcusiones]============================================*/

// Includes de FreeRTOS
#include "FreeRTOS.h"
#include "FreeRTOSConfig.h"
#include "task.h"

#include "sd_spi.h"   // <= su propio archivo de cabecera

#include "sapi.h"     // <= sAPI header

#include "ff.h"       // <= Biblioteca FAT FS

#include <string.h>   // <= Biblioteca de manejo de Strings, ver:
// https://es.wikipedia.org/wiki/String.h
// http://www.alciro.org/alciro/Programacion-cpp-Builder_12/funciones-cadenas-caracteres-string.h_448.htm

/*==================[definiciones y macros]==================================*/

#define FILENAME  "log.txt"

#define SEPARATOR ","

/*==================[definiciones de datos internos]=========================*/

static FATFS fs;           // <-- FatFs work area needed for each volume
static FIL fp;             // <-- File object needed for each open file

static char rtcString[22];
static char adcString[6];
static char textToWrite[52];

/*==================[definiciones de datos externos]=========================*/

DEBUG_PRINT_ENABLE;

/*==================[declaraciones de funciones internas]====================*/

// Se termina el programa dejandolo en un loop infinito
static void stopProgram( void );

// Se monta el sistema de archivos y Tarjeta SD
static void mountSDCard( void );

// Se graba un texto en la Tarjeta SD, si falla detiene el programa
static void saveStringInSDCard( char* str, uint32_t strLen );

// Guarda en str la fecha y hora convertido a string en formato "DD/MM/YYYY HH:MM:SS"
static void rtcToString( char* str, rtc_t* rtc );

/*==================[declaraciones de funciones externas]====================*/

// Prototipo de funcion de la tarea myTask
void myTask( void* taskParmPtr );

// Prototipo de funcion de la tarea diskTask
void diskTask( void *taskParmPtr );

// Prototipo de funcion de la tarea logTask
void logTask( void *taskParmPtr );

/*==================[funcion principal]======================================*/

// FUNCION PRINCIPAL, PUNTO DE ENTRADA AL PROGRAMA LUEGO DE ENCENDIDO O RESET.
int main(void)
{
   // ---------- CONFIGURACIONES ------------------------------
   // Inicializar y configurar la plataforma
   boardConfig();

   // UART for debug messages
   debugPrintConfigUart( UART_USB, 115200 );
   debugPrintlnString( "Este programa loguea el ADC en un archivo en la tarjeta SD hasta que se mantenga presionada TEC1 o pase un minuto." );
   debugPrintEnter();

   // Led para dar se�al de vida
   gpioWrite( LED3, ON );

   // Crear tarea myTask en freeRTOS
	xTaskCreate(
                myTask,                     // Funcion de la tarea a ejecutar
		          (const char *)"myTask",     // Nombre de la tarea como String amigable para el usuario
					 configMINIMAL_STACK_SIZE*2, // Tama�o del stack de la tarea
					 0,                          // Parametros de tarea
					 tskIDLE_PRIORITY+1,         // Prioridad de la tarea
					 0                           // Puntero a la tarea creada en el sistema
              );

   // Crear tarea logTask en freeRTOS
	xTaskCreate( logTask, (const char *)"logTask", configMINIMAL_STACK_SIZE*2,
                0, tskIDLE_PRIORITY+2, 0 );

   // Crear tarea diskTask en freeRTOS
	xTaskCreate( diskTask, (const char *)"diskTask", configMINIMAL_STACK_SIZE*2,
                0, tskIDLE_PRIORITY+3, 0 );

   // Iniciar scheduler
	vTaskStartScheduler();

   // ---------- REPETIR POR SIEMPRE --------------------------
   while( TRUE )
   {
      // Si cae en este while 1 significa que no pudo iniciar el scheduler
      sleepUntilNextInterrupt();
   }
   // NO DEBE LLEGAR NUNCA AQUI, debido a que a este programa se ejecuta
   // directamenteno sobre un microcontroladore y no es llamado por ningun
   // Sistema Operativo, como en el caso de un programa para PC.
   return 0;
}


/*==================[definiciones de funciones internas]=====================*/

// Se termina el programa dejandolo en un loop infinito
static void stopProgram( void ){
   debugPrintlnString( "Fin del programa." );
   while(TRUE){
      sleepUntilNextInterrupt();
   }
}

// Se monta el sistema de archivos y Tarjeta SD
static void mountSDCard( void ){
   // Give a work area to the default drive
   if( f_mount( &fs, "", 0 ) != FR_OK ){
      // If this fails, it means that the function could not register a file
      // system object. Check whether the SD card is correctly connected.
      debugPrintlnString( "Error intentando montar la tarjeta SD." );
      stopProgram();
   }
}

// Se graba un texto en la Tarjeta SD, si falla detiene el programa
static void saveStringInSDCard( char* str, uint32_t strLen ){

   UINT nbytes = 0;
   uint8_t i=0;

   // Create/open a file, then write a string and close it
   if( f_open( &fp, FILENAME, FA_WRITE | FA_OPEN_APPEND ) == FR_OK ){

      f_write( &fp, textToWrite, strlen(textToWrite), &nbytes );
      f_close(&fp);

      if( strlen(textToWrite) == nbytes ){
         // Blink 2 times LEDB if the write operation was successful
         for( i=0; i<2; i++ ){
            gpioToggle( LEDB );
            // Envia la tarea al estado bloqueado durante 50ms
         	vTaskDelay( 50 / portTICK_RATE_MS );
         }
      } else{
         // Blink 2 times LEDR if the write operation was fail
         for( i=0; i<2; i++ ){
            gpioToggle( LEDR );
            // Envia la tarea al estado bloqueado durante 50ms
         	vTaskDelay( 50 / portTICK_RATE_MS );
         }
         debugPrintlnString( "Error de escritura en achivo." );
      }

   } else{
      // Turn ON LEDR if can't create/open this file
      gpioWrite( LEDR, ON );
      debugPrintlnString( "Error intentando crear/abrir el archivo LOG.TXT." );
      stopProgram();
   }
}


// Guarda en str la fecha y hora convertido a string en formato "DD/MM/YYYY HH:MM:SS"
static void rtcToString( char* str, rtc_t* rtc ){

   char rtcMemberString[10];
   rtcString[0] = 0;

   // "DD/MM/YYYY "

   // Conversion de entero a string con base 10 (decimal)
   int64ToString( (rtc->mday), rtcMemberString, 10 );
   if( (rtc->mday)<10 ){
      // Concateno rtcString+"0"
      strncat( rtcString, "0", strlen("0") );
   }
   // Concateno rtcString+mday+"/"
   strncat( rtcString, rtcMemberString, strlen(rtcMemberString) );
   strncat( rtcString, "/", strlen("/") );

   // Conversion de entero a string con base 10 (decimal)
   int64ToString( (rtc->month), rtcMemberString, 10 );
   if( (rtc->month)<10 ){
      // Concateno rtcString+"0"
      strncat( rtcString, "0", strlen("0") );
   }
   // Concateno rtcString+month+"/"
   strncat( rtcString, rtcMemberString, strlen(rtcMemberString) );
   strncat( rtcString, "/", strlen("/") );

   // Conversion de entero a string con base 10 (decimal)
   int64ToString( (rtc->year), rtcMemberString, 10 );
   // Concateno rtcString+year+" "
   strncat( rtcString, rtcMemberString, strlen(rtcMemberString) );
   strncat( rtcString, " ", strlen(" ") );

   // "HH:MM:SS"

   // Conversion de entero a string con base 10 (decimal)
   int64ToString( (rtc->hour), rtcMemberString, 10 );
   if( (rtc->hour)<10 ){
      // Concateno rtcString+"0"
      strncat( rtcString, "0", strlen("0") );
   }
   // Concateno rtcString+hour+":"
   strncat( rtcString, rtcMemberString, strlen(rtcMemberString) );
   strncat( rtcString, ":", strlen(":") );

   // Conversion de entero a string con base 10 (decimal)
   int64ToString( (rtc->min), rtcMemberString, 10 );
   if( (rtc->min)<10 ){
      // Concateno rtcString+"0"
      strncat( rtcString, "0", strlen("0") );
   }
   // Concateno rtcString+min+":"
   strncat( rtcString, rtcMemberString, strlen(rtcMemberString) );
   strncat( rtcString, ":", strlen(":") );

   // Conversion de entero a string con base 10 (decimal)
   int64ToString( (rtc->sec), rtcMemberString, 10 );
   if( (rtc->sec)<10 ){
      // Concateno rtcString+"0"
      strncat( rtcString, "0", strlen("0") );
   }
   // Concateno rtcString+sec
   strncat( rtcString, rtcMemberString, strlen(rtcMemberString) );
}


/*==================[definiciones de funciones externas]=====================*/

// Implementacion de funcion de la tarea myTask
void myTask( void* taskParmPtr )
{
   // ---------- CONFIGURACIONES ------------------------------

   // ---------- REPETIR POR SIEMPRE --------------------------
	while(TRUE)
   {
      // Intercambia el estado del LEDB
		gpioToggle( LED3 );

      // Envia la tarea al estado bloqueado durante 500ms
		vTaskDelay( 500 / portTICK_RATE_MS );
	}
}


// Implementacion de funcion de la tarea diskTask
void diskTask( void *taskParmPtr )
{
   // ---------- CONFIGURACIONES ------------------------------

   // ---------- REPETIR POR SIEMPRE --------------------------
	while(TRUE)
   {
      disk_timerproc();   // Disk timer process

      // Envia la tarea al estado bloqueado durante 10ms
	   vTaskDelay( 10 / portTICK_RATE_MS );
	}
}


// Implementacion de funcion de la tarea logTask
void logTask( void *taskParmPtr )
{
   debugPrintlnString( "Configurando..." );

   // Inicializar ADC
   debugPrintString( "Configurando ADC..." );
   adcConfig( ADC_ENABLE );
   debugPrintlnString( "ADC OK" );

   // SPI configuration
   debugPrintString( "Configurando SPI..." );
   spiConfig( SPI0 );
   debugPrintlnString( "SPI OK" );


   // RTC configuration
   debugPrintString( "Configurando RTC..." );

   // Estructura RTC
   rtc_t rtc;
   rtc.year = 2017;
   rtc.month = 6;
   rtc.mday = 10;
   rtc.wday = 1;
   rtc.hour = 10;
   rtc.min = 35;
   rtc.sec= 0;
   // Inicializar RTC
   rtcConfig( &rtc );

   // Envia la tarea al estado bloqueado durante 1s para darle tiempo a cambiar el valor del RTC
	vTaskDelay( 1000 / portTICK_RATE_MS );

   debugPrintlnString( "RTC OK" );


   // Montar sistema de archivo y tarjeta SD
   debugPrintString( "Montando SD..." );
   mountSDCard();
   debugPrintlnString( "SD OK" );

   debugPrintEnter();
   debugPrintlnString( "Comienzo de Log de muestras, NO RETIRAR la tarjeta SD hasta concluya." );

   uint8_t counter = 0;
   uint16_t adcValue = 0;

   portTickType xLastWakeTime;
   portTickType xPeriodo =  1000 / portTICK_RATE_MS;

   xLastWakeTime = xTaskGetTickCount();

   // ---------- REPETIR POR SIEMPRE --------------------------
   while( TRUE )
   {
      // Leer fecha y hora
      rtcRead( &rtc );
      // Convertir lectura de fecha y hora a string en formato "DD/MM/YYYY HH:MM:SS"
      rtcToString( rtcString, &rtc );
      // Agrego SEPARATOR al final
      strncat( rtcString, SEPARATOR, 1 );
      // Muestro por UART
      debugPrintString( rtcString );

      // Leer adc
      // Se debe esperar minimo 67ms entre lecturas su la tasa es de 15Hz
      adcValue = adcRead( CH1 );
      // Convertir lectura del ADC a string (va a dar un valor de "0" a "1023")
      int64ToString( (int64_t)adcValue, adcString, 10 );
      // Agrego SEPARATOR al final
      strncat( adcString, SEPARATOR, 1 );
      // Muestro por UART
      debugPrintString( adcString );
      debugPrintEnter();

      // Concatento textToWrite con rtcString quedando en textToWrite: rtcString
      strncat( textToWrite, rtcString, strlen(rtcString) );
      // Concatento textToWrite con magString quedando textToWrite: rtcString+adcString
      strncat( textToWrite, adcString, strlen(adcString) );
      // Concatento textToWrite con "\r\n" quedando textToWrite: rtcString+adcString+"\r\n"
      strncat( textToWrite, "\r\n", 2 );

      // Grabar muestra en la Tarjeta SD
      saveStringInSDCard( textToWrite, strlen(textToWrite) );

      // Detengo el programa con la tecla TEC1 o si guardo 60 muestras
      if( !gpioRead(TEC1) || (counter >= 60) ){
         debugPrintlnString( "Log completo, puede retirar la SD." );
         // Turn ON LEDG if program is done
         gpioWrite( LEDG, ON );
         stopProgram();
      }
      // Incremento el contador de muestras guardadas
      counter++;

      // Reeteo variables para la proxima muestra
      adcValue = 0;
      textToWrite[0] = 0;

      // Envia la tarea al estado bloqueado durante 1s para preparase para la proxima muestras
	   vTaskDelayUntil( &xLastWakeTime, xPeriodo );
   }


}

/*==================[fin del archivo]========================================*/
