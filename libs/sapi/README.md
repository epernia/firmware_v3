# sAPI library for microcontrollers

This library implements a simple API that acts as a HAL (Hardware Abstraction
Layer) for microcontrollers.

It takes ideas from *Wiring library*, but use the concept of *peripheral* instead
the concept of *pin*, making the API regardless of the number of pins that use
certain peripheral.

## Documentation

**NOTE:** Always use the [released versions](../../releases) because these are tested all examples and the API documentation is consistent. The master branch may contain inconsistencies because this library is currently under development.

### Included modules

**Internal Peripherals**

- Data types.
- Peripheral Map.
- Board.
- Tick.
- GPIO.
- UART.
- ADC.
- DAC.
- I2C.
- RTC.
- Sleep.
- PWM.

**Delays**

- Delay.

**External Peripherals**

- 7-segment display.
- Keypad.
- Angular Servo (0 to 180°).
- Magnetometer (compass) sensor HMC5883L.

Every module includes an example.

### Software layers

![ "sapi-modulos-capas.png" image not found](docs/assets/img/sapi-modulos-capas.png "Modules an layers of sAPI library")

### Boards

Now available for boards:

- EDU-CIAA-NXP (NXP LPC4337 microcontroller). [Download documentation.](docs/assets/pdf/EDU-CIAA-NXP_sAPI_bm_A4_v1r0_ES.pdf)
- CIAA-NXP (NXP LPC4337 microcontroller).
