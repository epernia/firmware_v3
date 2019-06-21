#include <FreeRTOS.h>
#include <stdio.h>
#include <task.h>

#define WEAK __attribute__ ((weak))

WEAK void vAssertCalled( uint32_t ulLine, const char * const pcFile )
{
// The following two variables are just to ensure the parameters are not
// optimised away and therefore unavailable when viewed in the debugger.
   volatile uint32_t ulLineNumber = ulLine, ulSetNonZeroInDebuggerToReturn = 0;
   volatile const char * const pcFileName = pcFile;

   taskENTER_CRITICAL();
   {
      printf( "\r\nvAssertCalled()\r\n   LLine Number = %d\r\n   File Name = %s\r\n\r\n",
              ulLineNumber, pcFileName ); // @Eric
      while( ulSetNonZeroInDebuggerToReturn == 0 ) {
         // If you want to set out of this function in the debugger to see
         // the assert() location then set ulSetNonZeroInDebuggerToReturn to a
         // non-zero value.
      }
   }
   taskEXIT_CRITICAL();

//   printf( "Stop in a while(1)\r\n\r\n" ); // @Eric
//   while(1); // @Eric
}
/*-----------------------------------------------------------*/

WEAK void vApplicationMallocFailedHook( void )
{
   // vApplicationMallocFailedHook() will only be called if
   // configUSE_MALLOC_FAILED_HOOK is set to 1 in FreeRTOSConfig.h. It is a hook
   // function that will get called if a call to pvPortMalloc() fails.
   // pvPortMalloc() is called internally by the kernel whenever a task, queue,
   // timer, event group, or semaphore is created.  It is also called by various
   // parts of the demo application.  If heap_1.c, heap_2.c or heap_4.c are used,
   // then the size of the heap available to pvPortMalloc() is defined by
   // configTOTAL_HEAP_SIZE in FreeRTOSConfig.h, and the xPortGetFreeHeapSize()
   // API function can be used to query the size of free heap space that remains.
   // More information is provided in the book text.
   printf( "Application Malloc Failed Hook!\r\n" );
   vAssertCalled( __LINE__, __FILE__ );
}
/*-----------------------------------------------------------*/

// An example vApplicationIdleHook() implementation is included here for
// completeness, but it is not actually built (it is excluded by the #if 0) as it
// is also defined by the examples that actually make use of the function.
#if 0
WEAK void vApplicationIdleHook( void )
{
   // vApplicationIdleHook() will only be called if configUSE_IDLE_HOOK is set
   // to 1 in FreeRTOSConfig.h.  It will be called on each iteration of the idle
   // task.  It is essential that code added to this hook function never attempts
   // to block in any way (for example, call xQueueReceive() with a block time
   // specified, or call vTaskDelay()).  If the application makes use of the
   // vTaskDelete() API function then it is also important that
   // vApplicationIdleHook() is permitted to return to its calling function,
   // because it is the responsibility of the idle task to clean up memory
   // allocated by the kernel to any task that has since been deleted.  More
   // information is provided in the book text.
   printf( "Application Idle Hook!\r\n" );
   vAssertCalled( __LINE__, __FILE__ );
}
#endif /* 0 */
/*-----------------------------------------------------------*/

WEAK void vApplicationStackOverflowHook( TaskHandle_t pxTask, char *pcTaskName )
{
   //( void ) pcTaskName;
   //( void ) pxTask;

   // Run time stack overflow checking is performed if
   // configCHECK_FOR_STACK_OVERFLOW is defined to 1 or 2. This hook function is
   // called if a stack overflow is detected. More information is provided in the
   // book text.
   printf( "\r\nApplication Stack Overflow!! on Task: %s\r\n", (char*)pcTaskName );
   vAssertCalled( __LINE__, __FILE__ );
}
/*-----------------------------------------------------------*/

WEAK void vApplicationTickHook( void )
{
   // This function will be called by each tick interrupt if
   // configUSE_TICK_HOOK is set to 1 in FreeRTOSConfig.h. User code can be
   // added here, but the tick hook is called from an interrupt context, so
   // code must not attempt to block, and only the interrupt safe FreeRTOS API
   // functions can be used (those that end in FromISR()).
   printf( "Application Tick Hook!\r\n" );
   vAssertCalled( __LINE__, __FILE__ );
}
/*-----------------------------------------------------------*/
