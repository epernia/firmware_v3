#include "FreeRTOS.h"
#include "task.h"
#include "sapi.h"

StackType_t myTaskStack[configMINIMAL_STACK_SIZE];
StaticTask_t myTaskTCB;

void myTask( void* taskParmPtr )
{
   printf( "Blinky con freeRTOS y sAPI.\r\n" );

   gpioWrite( LED1, ON );
   // Envia la tarea al estado bloqueado durante 1 s (delay)
   vTaskDelay( 1000 / portTICK_RATE_MS );
   gpioWrite( LED1, OFF ); 

   // Tarea periodica cada 500 ms
   portTickType xPeriodicity =  500 / portTICK_RATE_MS;
   portTickType xLastWakeTime = xTaskGetTickCount();
   
   while(TRUE) {
      gpioToggle( LEDB );
      printf( "Blink!\r\n" );
      // Envia la tarea al estado bloqueado durante xPeriodicity (delay periodico)
      vTaskDelayUntil( &xLastWakeTime, xPeriodicity );
   }
}

int main(void)
{
   boardConfig();

   xTaskCreateStatic( myTask, "myTask", configMINIMAL_STACK_SIZE, NULL,
                     tskIDLE_PRIORITY+1, myTaskStack, &myTaskTCB);

   vTaskStartScheduler();
   
   while(1);

   return 0;
}
