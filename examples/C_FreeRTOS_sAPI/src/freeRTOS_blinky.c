#include "FreeRTOS.h"
#include "task.h"
#include "sapi.h"

DEBUG_PRINT_ENABLE;

StackType_t myTaskStack[configMINIMAL_STACK_SIZE];
StaticTask_t myTaskTCB;

void myTask( void* taskParmPtr )
{
   gpioWrite( LED2, ON );
   debugPrintConfigUart( UART_USB, 115200 );
   debugPrintlnString( "Blinky con freeRTOS y sAPI." );
   while(TRUE) {
      gpioToggle( LEDR );
      vTaskDelay( 500 );
   }
}

int main(void)
{
   boardConfig();

   xTaskCreateStatic(myTask, "myTask", configMINIMAL_STACK_SIZE, NULL,
                   tskIDLE_PRIORITY+1, myTaskStack, &myTaskTCB);

   vTaskStartScheduler();
   
   while(1);
   
   return 0;
}
