#define WEAK __attribute__ ((__used__, weak))
#define ALIAS(f) __attribute__ ((__used__, weak, alias (#f)))

WEAK void IntDefaultHandler(void);

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

__attribute__ ((section(".after_vectors")))
void IntDefaultHandler(void) {
    while (1) {
    }
}
