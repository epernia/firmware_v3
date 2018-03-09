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
     * void ledDelay(void) {
     * }
     */
    .type ledDelay, %function
    .global ledDelay
ledDelay:
        ldr r1, = 3000000
delay1: subs r1, 1
        bne delay1
        ldr r1, = 3000000
delay2: subs r1, 1
        bne delay2
        mov pc, lr
    .size ledDelay, . - ledDelay

    /*
     * int main(void) {
     *    SystemCoreClockUpdate();
     *    Board_Init();
     *    SysTick_Config(SystemCoreClock / TICKRATE_HZ);
     * 
     *    while (1) {
     *        Board_LED_Toggle(LED_3);
     *        ledDelay();
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
main_loop:
    mov r0, #2
    bl Board_LED_Toggle
    bl ledDelay
    b main_loop // Infinite loop on stop
    .size main, . - main
