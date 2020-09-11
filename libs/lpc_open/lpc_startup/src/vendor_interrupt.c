#define WEAK __attribute__ ((__used__, weak))
#define ALIAS(f) __attribute__ ((__used__, weak, alias (#f)))

WEAK void IntDefaultHandler(void);

#if __CORTEX_M == 0U
void RTC_IRQHandler(void) ALIAS(IntDefaultHandler);
void M4_IRQHandler(void) ALIAS(IntDefaultHandler);
void DMA_IRQHandler(void) ALIAS(IntDefaultHandler);
void FLASH_EEPROM_ATIMER_IRQHandler(void) ALIAS(IntDefaultHandler);
void ETH_IRQHandler(void) ALIAS(IntDefaultHandler);
void SDIO_IRQHandler(void) ALIAS(IntDefaultHandler);
void LCD_IRQHandler(void) ALIAS(IntDefaultHandler);
void USB0_IRQHandler(void) ALIAS(IntDefaultHandler);
void USB1_IRQHandler(void) ALIAS(IntDefaultHandler);
void SCT_IRQHandler(void) ALIAS(IntDefaultHandler);
void RIT_WDT_IRQHandler(void) ALIAS(IntDefaultHandler);
void TIMER0_IRQHandler(void) ALIAS(IntDefaultHandler);
void GINT1_IRQHandler(void) ALIAS(IntDefaultHandler);
void GPIO4_IRQHandler(void) ALIAS(IntDefaultHandler);
void TIMER3_IRQHandler(void) ALIAS(IntDefaultHandler);
void MCPWM_IRQHandler(void) ALIAS(IntDefaultHandler);
void ADC0_IRQHandler(void) ALIAS(IntDefaultHandler);
void I2C0_IRQHandler(void) ALIAS(IntDefaultHandler);
void SGPIO_IRQHandler(void) ALIAS(IntDefaultHandler);
void SPI_DAC_IRQHandler (void) ALIAS(IntDefaultHandler);
void ADC1_IRQHandler(void) ALIAS(IntDefaultHandler);
void SSP0_SSP1_IRQHandler(void) ALIAS(IntDefaultHandler);
void EVRT_IRQHandler(void) ALIAS(IntDefaultHandler);
void UART0_IRQHandler(void) ALIAS(IntDefaultHandler);
void UART1_IRQHandler(void) ALIAS(IntDefaultHandler);
void UART2_CAN1_IRQHandler(void) ALIAS(IntDefaultHandler);
void UART3_IRQHandler(void) ALIAS(IntDefaultHandler);
void I2S0_I2S1_QEI_IRQHandler(void) ALIAS(IntDefaultHandler);
void CAN0_IRQHandler(void) ALIAS(IntDefaultHandler);
void SPIFI_ADCHS_IRQHandler(void) ALIAS(IntDefaultHandler);
void M0SUB_IRQHandler(void) ALIAS(IntDefaultHandler);

// Chip Level - LPC43 (M4)
__attribute__ ((used,section(".isr_vendor_vector")))
void (* const g_pfnVendorVectors[])(void) = {
    // Chip Level - 43xx M0 core
    RTC_IRQHandler,                 // 16 RTC
    M4_IRQHandler,   	            // 17 CortexM4/M0 (LPC43XX ONLY)
    DMA_IRQHandler,                 // 18 General Purpose DMA
    0,                              // 19 Reserved
    FLASH_EEPROM_ATIMER_IRQHandler, // 20 ORed flash Bank A & B, EEPROM and ATIMER interrupts
    ETH_IRQHandler,                 // 21 Ethernet
    SDIO_IRQHandler,                // 22 SD/MMC
    LCD_IRQHandler,                 // 23 LCD
    USB0_IRQHandler,                // 24 USB0
    USB1_IRQHandler,                // 25 USB1
    SCT_IRQHandler,                 // 26 State Configurable Timer
    RIT_WDT_IRQHandler,             // 27 ORed Repetitive Interrupt Timer, WWDT
    TIMER0_IRQHandler,              // 28 Timer0
    GINT1_IRQHandler,               // 29 GINT1
    GPIO4_IRQHandler,               // 30 GPIO4
    TIMER3_IRQHandler,              // 31 Timer 3
    MCPWM_IRQHandler,               // 32 Motor Control PWM
    ADC0_IRQHandler,                // 33 A/D Converter 0
    I2C0_IRQHandler,                // 34 ORed I2C0, I2C1
    SGPIO_IRQHandler,               // 35 SGPIO (LPC43XX ONLY)
    SPI_DAC_IRQHandler,             // 36 ORed SPI, DAC (LPC43XX ONLY)
    ADC1_IRQHandler,                // 37 A/D Converter 1
    SSP0_SSP1_IRQHandler,           // 38 ORed SSP0, SSP1
    EVRT_IRQHandler,                // 39 Event Router
    UART0_IRQHandler,               // 40 UART0
    UART1_IRQHandler,               // 41 UART1
    UART2_CAN1_IRQHandler,          // 42 ORed USART2 and C_CAN1
    UART3_IRQHandler,               // 43 USRT3
    I2S0_I2S1_QEI_IRQHandler,       // 44 ORed I2S0, I2S1, QEI
    CAN0_IRQHandler,                // 45 C_CAN0
    SPIFI_ADCHS_IRQHandler,         // 46 SPIFI OR ADCHS interrupt
    M0SUB_IRQHandler,               // 47 M0SUB core
};

#else
WEAK void DAC_IRQHandler(void) ALIAS(IntDefaultHandler);
WEAK void M0APP_IRQHandler(void) ALIAS(IntDefaultHandler);
WEAK void DMA_IRQHandler(void) ALIAS(IntDefaultHandler);
WEAK void FLASH_EEPROM_IRQHandler(void) ALIAS(IntDefaultHandler);
WEAK void ETH_IRQHandler(void) ALIAS(IntDefaultHandler);
WEAK void SDIO_IRQHandler(void) ALIAS(IntDefaultHandler);
WEAK void LCD_IRQHandler(void) ALIAS(IntDefaultHandler);
WEAK void USB0_IRQHandler(void) ALIAS(IntDefaultHandler);
WEAK void USB1_IRQHandler(void) ALIAS(IntDefaultHandler);
WEAK void SCT_IRQHandler(void) ALIAS(IntDefaultHandler);
WEAK void RIT_IRQHandler(void) ALIAS(IntDefaultHandler);
WEAK void TIMER0_IRQHandler(void) ALIAS(IntDefaultHandler);
WEAK void TIMER1_IRQHandler(void) ALIAS(IntDefaultHandler);
WEAK void TIMER2_IRQHandler(void) ALIAS(IntDefaultHandler);
WEAK void TIMER3_IRQHandler(void) ALIAS(IntDefaultHandler);
WEAK void MCPWM_IRQHandler(void) ALIAS(IntDefaultHandler);
WEAK void ADC0_IRQHandler(void) ALIAS(IntDefaultHandler);
WEAK void I2C0_IRQHandler(void) ALIAS(IntDefaultHandler);
WEAK void SPI_IRQHandler(void) ALIAS(IntDefaultHandler);
WEAK void I2C1_IRQHandler(void) ALIAS(IntDefaultHandler);
WEAK void ADC1_IRQHandler(void) ALIAS(IntDefaultHandler);
WEAK void SSP0_IRQHandler(void) ALIAS(IntDefaultHandler);
WEAK void SSP1_IRQHandler(void) ALIAS(IntDefaultHandler);
WEAK void UART0_IRQHandler(void) ALIAS(IntDefaultHandler);
WEAK void UART1_IRQHandler(void) ALIAS(IntDefaultHandler);
WEAK void UART2_IRQHandler(void) ALIAS(IntDefaultHandler);
WEAK void UART3_IRQHandler(void) ALIAS(IntDefaultHandler);
WEAK void I2S0_IRQHandler(void) ALIAS(IntDefaultHandler);
WEAK void I2S1_IRQHandler(void) ALIAS(IntDefaultHandler);
WEAK void SPIFI_IRQHandler(void) ALIAS(IntDefaultHandler);
WEAK void SGPIO_IRQHandler(void) ALIAS(IntDefaultHandler);
WEAK void GPIO0_IRQHandler(void) ALIAS(IntDefaultHandler);
WEAK void GPIO1_IRQHandler(void) ALIAS(IntDefaultHandler);
WEAK void GPIO2_IRQHandler(void) ALIAS(IntDefaultHandler);
WEAK void GPIO3_IRQHandler(void) ALIAS(IntDefaultHandler);
WEAK void GPIO4_IRQHandler(void) ALIAS(IntDefaultHandler);
WEAK void GPIO5_IRQHandler(void) ALIAS(IntDefaultHandler);
WEAK void GPIO6_IRQHandler(void) ALIAS(IntDefaultHandler);
WEAK void GPIO7_IRQHandler(void) ALIAS(IntDefaultHandler);
WEAK void GINT0_IRQHandler(void) ALIAS(IntDefaultHandler);
WEAK void GINT1_IRQHandler(void) ALIAS(IntDefaultHandler);
WEAK void EVRT_IRQHandler(void) ALIAS(IntDefaultHandler);
WEAK void CAN1_IRQHandler(void) ALIAS(IntDefaultHandler);
WEAK void ADCHS_IRQHandler(void) ALIAS(IntDefaultHandler);
WEAK void ATIMER_IRQHandler(void) ALIAS(IntDefaultHandler);
WEAK void RTC_IRQHandler(void) ALIAS(IntDefaultHandler);
WEAK void WDT_IRQHandler(void) ALIAS(IntDefaultHandler);
WEAK void M0SUB_IRQHandler(void) ALIAS(IntDefaultHandler);
WEAK void CAN0_IRQHandler(void) ALIAS(IntDefaultHandler);
WEAK void QEI_IRQHandler(void) ALIAS(IntDefaultHandler);

// Chip Level - LPC43 (M4)
__attribute__ ((used,section(".isr_vendor_vector")))
void (* const g_pfnVendorVectors[])(void) = {
    DAC_IRQHandler,           // 16
    M0APP_IRQHandler,         // 17 CortexM4/M0 (LPC43XX ONLY)
    DMA_IRQHandler,           // 18
    0,                        // 19
    FLASH_EEPROM_IRQHandler,  // 20 ORed flash Bank A, flash Bank B, EEPROM interrupts
    ETH_IRQHandler,           // 21
    SDIO_IRQHandler,          // 22
    LCD_IRQHandler,           // 23
    USB0_IRQHandler,          // 24
    USB1_IRQHandler,          // 25
    SCT_IRQHandler,           // 26
    RIT_IRQHandler,           // 27
    TIMER0_IRQHandler,        // 28
    TIMER1_IRQHandler,        // 29
    TIMER2_IRQHandler,        // 30
    TIMER3_IRQHandler,        // 31
    MCPWM_IRQHandler,         // 32
    ADC0_IRQHandler,          // 33
    I2C0_IRQHandler,          // 34
    I2C1_IRQHandler,          // 35
    SPI_IRQHandler,           // 36
    ADC1_IRQHandler,          // 37
    SSP0_IRQHandler,          // 38
    SSP1_IRQHandler,          // 39
    UART0_IRQHandler,         // 40
    UART1_IRQHandler,         // 41
    UART2_IRQHandler,         // 42
    UART3_IRQHandler,         // 43
    I2S0_IRQHandler,          // 44
    I2S1_IRQHandler,          // 45
    SPIFI_IRQHandler,         // 46
    SGPIO_IRQHandler,         // 47
    GPIO0_IRQHandler,         // 48
    GPIO1_IRQHandler,         // 49
    GPIO2_IRQHandler,         // 50
    GPIO3_IRQHandler,         // 51
    GPIO4_IRQHandler,         // 52
    GPIO5_IRQHandler,         // 53
    GPIO6_IRQHandler,         // 54
    GPIO7_IRQHandler,         // 55
    GINT0_IRQHandler,         // 56
    GINT1_IRQHandler,         // 57
    EVRT_IRQHandler,          // 58
    CAN1_IRQHandler,          // 59
    0,                        // 60
    ADCHS_IRQHandler,         // 61 ADCHS combined interrupt
    ATIMER_IRQHandler,        // 62
    RTC_IRQHandler,           // 63
    0,                        // 64
    WDT_IRQHandler,           // 65
    M0SUB_IRQHandler,         // 66
    CAN0_IRQHandler,          // 67
    QEI_IRQHandler,           // 68
};
#endif

__attribute__ ((section(".after_vectors")))
void IntDefaultHandler(void) {
    while (1) {
    }
}
