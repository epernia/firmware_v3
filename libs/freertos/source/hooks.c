#include <FreeRTOS.h>
#include <task.h>

void vApplicationStackOverflowHook(TaskHandle_t xTask,
                                   signed char *pcTaskName)
{
    while(1)
        ;
}

void vApplicationMallocFailedHook( void )
{
    while(1)
        ;
}
