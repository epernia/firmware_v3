   .syntax unified
   .cpu cortex-m4
   .fpu softvfp
   .thumb

.equ LEDR, 40
.equ LEDB, 41
.equ LEDG, 42
.equ LED1, 43
.equ LED2, 44
.equ LED3, 45

.equ DELAY, 100

    .section .text
    .type  main, %function
    .global  main
main:
    bl boardInit // boardInit();
main_loop:
    movs r0, #LED3
    bl gpioToggle // gpioToggle( LED3 );
    movs r0, #DELAY & 0xFFFFFFFF // <- r0 DELAY[31:0] 
    movs r1, #DELAY >> 31 //32        // <- r1 DELAY[63:32]
    bl delay // delay( 100 ); // tick_t is a 64-bits data type!
    b main_loop // Infinite loop on stop
    .size main, . - main

/*
// FUNCION PRINCIPAL, PUNTO DE ENTRADA AL PROGRAMA LUEGO DE ENCENDIDO O RESET.
int main( void ){

   // ---------- CONFIGURACIONES ------------------------------

   // Inicializar y configurar la plataforma
   boardInit();

   // ---------- REPETIR POR SIEMPRE --------------------------
   while( TRUE )
   {
      // Intercambiar el valor del pin conectado a LED3
      gpioToggle( LED3 );

      // Retardo bloqueante durante 100ms
      delay( 100 );
   }

   // NO DEBE LLEGAR NUNCA AQUI, debido a que a este programa se ejecuta
   // directamenteno sobre un microcontroladore y no es llamado por ningun
   // Sistema Operativo, como en el caso de un programa para PC.
   return 0;
}    
*/