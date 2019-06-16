/*

You need to add

DEFINES+=TICK_OVER_RTOS
DEFINES+=USE_FREERTOS

on config.mk to tell SAPI to use FreeRTOS Systick

*/

#include "FreeRTOS.h"
#include "FreeRTOSConfig.h"
#include "task.h"
#include "sapi.h"

void tickTask( void* pvParameters )
{
   while(TRUE) {
      // Una tarea muy bloqueante para demostrar que la interrupcion funcina
      gpioToggle(LEDB);
      vTaskDelay(1000/portTICK_RATE_MS);
   }
}

void onRx( void *noUsado )
{
   char c = uartRxRead( UART_USB );
   printf( "Recibimos <<%c>> por UART\r\n", c );
}

int main(void)
{
   /* Inicializar la placa */
   boardConfig();

   /* Inicializar la UART_USB junto con las interrupciones de Tx y Rx */
   uartConfig(UART_USB, 115200);     
   // Seteo un callback al evento de recepcion y habilito su interrupcion
   uartCallbackSet(UART_USB, UART_RECEIVE, onRx, NULL);
   // Habilito todas las interrupciones de UART_USB
   uartInterrupt(UART_USB, true);
   
   xTaskCreate(
      tickTask,                     // Funcion de la tarea a ejecutar
      (const char *)"tickTask",     // Nombre de la tarea como String amigable para el usuario
      configMINIMAL_STACK_SIZE*2, // Cantidad de stack de la tarea
      0,                          // Parametros de tarea
      tskIDLE_PRIORITY+1,         // Prioridad de la tarea
      0                           // Puntero a la tarea creada en el sistema
   );

   vTaskStartScheduler();

   return 0;
}
