Ejemplos de código no bloqueante. Para poder realizar diferentes tareas en
simultáneo con un microcontrolador, como por ejemplo, hacer destellar un led
con tiempo de 10 segundos prendido y 10 segundos apagado, mientras un pulsador
maneja el encendido de otro led; observamos que debemos evitar bloquear al
microcontrolador esperando estos largos tiempos. 

Como primer alternatva vimos la utilización pequeño retardo bloqueante de 1ms y
una variable contador:

Programa **little_delay**.

```c
#include "sapi.h"

int main( void )
{
   boardInit();
   bool_t tec1 = 0;
   uint32_t contador = 0;

   while(TRUE) {
      tec1 = !gpioRead(TEC1);
      gpioWrite( LEDB, tec1 );

      delay(1);
      contador++;
      if( contador >= 5000 ){
         gpioToggle( LED1 );
         contador = 0;
      }
   }

   return 0;
}
```

Otra alternativa es utilizar las funciones de delay no bloqueante de la biblioteca sAPI:

Programa **non_blocking_delay**.

```c
#include "sapi.h"

int main( void )
{
   boardInit();
   bool_t tec1 = 0;
	
   delay_t led1Delay;
   delayConfig( &led1Delay, 5000 );

   while(TRUE) {
      tec1 = !gpioRead(TEC1);
      gpioWrite( LEDB, tec1 );

      if( delayRead(&led1Delay) ){
         gpioToggle( LED1 );
      }
   }

   return 0;
}
```

Otra alternativa es utilizar las funciones de manejo del Timer SysTick de la arquitectura ARM Cortex M mediante las funciones tick de la biblioteca sAPI. No lo llegamos a hacer en clase pero se puede encarar de la siguiente manera:

Programa **tick_callback**.

```c
##include "sapi.h"

int main( void )
{
   boardInit(); // Adentro llama a tickInit(1);
	
   bool_t tec1 = 0;
   tick_t tiempoEnTicks = 0;

   while(TRUE) {
      tec1 = !gpioRead(TEC1);
      gpioWrite( LEDB, tec1 );

      tiempoEnTicks = tickRead();
      if( tiempoEnTicks >= 5000 ){
         gpioToggle( LED1 );
         tickWrite(0);
      }
   }

   return 0;
}
```
