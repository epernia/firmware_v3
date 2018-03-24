   .syntax unified
   .cpu cortex-m4
   .fpu softvfp
   .thumb

   // static volatile uint32_t tick_ct = 0;
   .section .data
tick_ct:
   .word 0

   .section .text

   /*
   * void SysTick_Handler(void) {
   *      tick_ct++;
   * }
   */
   .type  SysTick_Handler, %function
   .global SysTick_Handler
SysTick_Handler:
   // The '= <address>' is for place address
   // This is translate to:
   //
   //    ldr r3, [pc, #<offset> ]
   //    ... rest of code ...
   // <place of offset>:
   //    .word <address>
   push { lr }       // save pc
   ldr r1, =tick_ct  // r1 <- address of tick_ct
   ldr r0, [r1]      // r0 <- mem[r1]
   add r0, #1        // r0 <- r0 + 1
   str r0, [r1]      // mem[r1] <- r0
   pop { pc }        // return
   .size SysTick_Handler, . - SysTick_Handler

   /*
   * void delay(uint32_t tk) {
   *    uint32_t end = tick_ct + tk;
   *    while(tick_ct < end);
   * }
   */
   .type delay, %function
   .global delay
main_delay:
   ldr r3, =tick_ct // r3 <- tick_ct
   ldr r3, [r3]     // r3 <- mem[r3]
   add r4, r3, r0   // r4 (end) <- r3 + r0 (tk)
main_delay_loop:
   ldr r3, =tick_ct    // r3 <- tick_ct
   ldr r3, [r3]        // r3 <- mem[r3]
   cmp r4, r3          // if r4 <= r3 -> 
   bge main_delay_loop
   blx lr
   .size main_delay, . - main_delay

   /*
    * int main( void ) {
    *    SystemCoreClockUpdate();
    *    Board_Init();
    *    SysTick_Config( SystemCoreClock / TICKRATE_HZ );
    * 
    *    while( 1 ) {
    *       Board_LED_Toggle( LED_1 );
    *       delay( 100 );
    *       printf( "Hola mundo at %d\r\n", tick_ct );
    *    }
    * }
    */
   .type  main, %function
   .global  main
main:
   bl SystemCoreClockUpdate
   bl Board_Init
   ldr r3, =SystemCoreClock
   ldr r3, [r3]
   movs r4, #1000
   udiv r0, r3, r4
   bl _SysTick_Config
main_loop:
   mov r0, #0
   bl Board_LED_Toggle
   mov r0, #100
   bl main_delay
   ldr r0, =printf_msg1
   ldr r1, =tick_ct
   ldr r1, [r1]
   bl printf
   b main_loop // Infinite loop on stop
   .size main, . - main

printf_msg1:
   .string "Hola mundo at %d\r\n"

