# CESE, FI-UBA projects templates for EDU-CIAA-NXP

CESE, FI-UBA: http://laboratorios.fi.uba.ar/lse/cursos.html

Available libraries:
   - LPCOpen v3.01
   - sAPI r0.5.0.
   - FreeRTOS Kernel V10.0.1
   - Elm-Chan FatFS (SSP & USB).

## IMPORTANT

**This environment is under construction!!**

**Always use the [released versions](../../releases) because in these all examples are tested and the API documentation is consistent. The master branch may contain inconsistencies because this environment is currently under development.**

## Supported boards
- CIAA-NXP (LPC4337).
- EDU-CIAA-NXP (LPC4337).

## Supported toolchains
- gcc-arm-none-eabi

## Usage
* Make sure you have an ```arm-none-eabi-*``` toolchain configured in your ```PATH```. If you don't have it, download [GCC ARM Embedded](https://developer.arm.com/open-source/gnu-toolchain/gnu-rm).
* Define ```PROJECT_NAME```  and ```PROJECT_PATH``` variables in ```project.mk``` according to the project you want to compile (PROJECT_PATH is relative to this folder, leave void if the project is in this folder).
* Compile with ```make```.
* Clean with ```make clean```. Clean for all targets with ```make clean_all```.
* Download to target via OpenOCD with ```make download```.

## Create a new project

Each project consist in a folder (with a non-spaces name) that includes inside 2 folders, one named ```src``` (here go, .c, .cpp or .s source code files), and another one named ```inc``` (here go, .h or .hpp source header files) and one file named ```config.mk```, where you may configure which libraries you include and compiler options.

## Add a new library

The ```libs``` folder allow you to include 2 types of libraries:

 - Simplie library. Consist in a folder (with a non-spaces name) that includes inside 2 folders, one named ```src``` (here go .c, .cpp or .s source code files), and another one named ```inc``` (here go .h or .hpp header files). This kind of library compiles automaticaly by the Makefile.
 - Advanced library. Consist in a library whit a complex folder and files strcuture, i.e. LibUSB. This case reuire make your own makefile. You can inspire from sAPI makefile to do that.


## Examples

Included examples are:

### ARM Cortex M4 Asembler
 - ```asm_lpc_open```: assembler with LPC Open library
 - ```asm_sapi```: assembler with sAPI library
 
### C language
 - **sAPI library**
    - bare_metal
        - ```adc_dac_01```: ADC and DAC example.
        - bluetooth
            - ```hm10_uart_bridge```: Bluettoth HM10 module AT, tx and rx example.
        - ```cycles_counter_01```: clock cycles counter functions, only work in debug mode. Allows execution time trazability.
        - gpio
            - ```gpio_01_switches_leds```: each switch drives the upper led.
            - ```gpio_02_blinky```: the simply led blinky with a blocking delay.
            - ```gpio_03_blinky_switch```: led blinky with a with a non-blocking delay, to allow you to respond to a switch at the same time.
            - ```gpio_04_led_sequences```: led sequences by using a non-blocking delay.
        - i2c: 
            - magnetometers. In Chinese GY-273 module you can have one of this magnetometers, that have the same pinout but different register map. To difference them see the chip, ignore the board serigraphy.
                - ```hmc5883l```: HMC5883L magnetometer. 
                - ```qmc5883l```: QMC5883L magnetometer.
            - IMUs
                - ```mpu9250```: MPU9250 IMU 9DOF (Gyroscope, Accelerometer and Magnetometer) + Temperature.
        - ```keypad_7segment_01```: Drives a keypad and 7 segment display.
        - ```lcd_01```: Drives a LCD display.
        - pwm
            - ```pwm_01```: PWM applied to LEDs.
            - ```pwm_02_rgb_controller_uart```: RGB LED example.
        - ```rtc_01```: RTC peripheral to have date and time clock.
        - ```sapi_basic_example```: a basic sAPI library example
        - ```servo_01```: angular servomotor PWM control example.
        - ```spi_01_sd_card_fat_filesystem```: ADC logging in a SD/MicroSD Card (SPI connected) by using a FAT File System (ChanFS).
        - temperature_humidity_sensors
            - ```dht11_01```: humidity and temperature sensor.
        - ```tick_01_tick_hook```: periodic tick function (interrupt-based) with periodic callback.
        - uart
            - ```uart_01_echo```: UART echo, it respond the same that you send from PC.
            - ```uart_02_receive_string_blocking```: waits until receive a certain pattern String in a UART or timeout expire (blocking code). Don't save received string.
            - ```uart_03_receive_string```: waits until receive a certain pattern String in a UART or timeout expire (non-blocking code). Don't save received string.
        - ultrasonic_sensors
            - ```hcsr04_01```:  HC-SR04 utrasonic distance sensor.
        - wifi_esp01_esp8266
            - ```01_uart_bridg```: use this to send AT commands directly to ESP01 module.
            - ```02_http_server```: embedded web server to see sensor values.
            - ```03_thingspeak```: send data to thingspeak dashboards.
    - operating_systems
        - coperative (see M.J. Pont's book at https://www.safetty.net/publications/pttes )
            - ```scheduler_01_seos```: first cooperative scheduler
            - ```scheduler_02_seos_background_foreground```: second cooperative scheduler
            - ```seos_pont_2014_01_first_app```: third cooperative scheduler
        - free_rtos
            - dynamic_mem
                - ```freeRTOS_01_blinky```
                - ```freeRTOS_03_ChanFatFS_SPI_SdCard_ADC_log```: ADC logging in a SD/MicroSD Card (SPI connected) by using a FAT File System (ChanFS), freeRTOS and sAPI.
            - static_mem
                - ```freeRTOS_01_blinky```
    - statecharts
        - ```statechart_bare_metal```
 - **LPC Open library**
    - bare_metal
        - ```blinky```
        - ```blinky_input```
        - ```blinky_ram```
        - ```blinky_rit```
        - ```boot```
        - ```i2c```
        - ```lpc_open_basic_example```
        - ```mpu```
        - ```pwm```
    - operating_systems
        - ```freertos_blinky```
    - statecharts
        - ```statechart```
 - ```newlib_printf_scanf```
 - ```without_libs```
 
### C++ language
 - ```cpp_sapi_basic_example```: a C++ basic example with sAPI library.