# CESE, FI-UBA projects templates for EDU-CIAA-NXP

CESE, FI-UBA: http://laboratorios.fi.uba.ar/lse/cursos.html

Available libraries:
   - LPCOpen v3.01
   - sAPI r0.5.0.
   - FreeRTOS Kernel V10.0.1

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
* ```git clone https://github.com/pridolfi/workspace.git && cd workspace```
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

**Bare-metal with sAPI library examples**

 - GPIO:
    - gpio_01_switches_leds: each switch drives the upper led.
    - gpio_02_blinky: the simply led blinky with a blocking delay.
    - gpio_03_blinky_switch: led blinky with a with a non-blocking delay, to allow you to respond to a switch at the same time.
    - gpio_04_led_sequences: led sequences by using a non-blocking delay.
    - keypad_7segment_01: Drives a keypad and 7 segment display.
    - lcd_01: Drives an LCD display.
 - UART:
    - uart_01_echo: UART echo, it respond the same that you send from PC.
    - uart_02_receive_string_blocking: Waits until receive a certain pattern String in a UART or timeout expire (blocking code).
    - uart_03_receive_string:  Waits until receive a certain pattern String in a UART or timeout expire (non-blocking code).
 - Printf
    - stdio_01_printf_sprintf: printf() and other similar standard C libray functions.
 - ADC, DAC:
    - adc_dac_01: ADC and DAC example.
 - TIMER, RTC:
    - cycles_counter_01: clock cycles counter functions, only work in debug mode. Allows execution time trazability.
    - tick_01_tickHook: Periodic tick function (interrupt-based) with periodic callback.
    - rtc_01: RTC peripheral to have date and time clock.
    - pwm_01: PWM applied to LEDs.
    - pwm_02_rgb_controller_uart: RGB LED example.
    - servo_01: Servomotor PWM control example.
 - External peripherals:
    - I2C Magnetometers. In Chinese GY-273 module you can have one of this magnetometers, that have the same pinout but different register map. To difference them see the chip, ignore the board serigraphy:
       - i2c_01_hmc5883l: HMC5883L magnetometer. 
       - i2c_02_qmc5883l: QMC5883L magnetometer.
    - dht11_01: Humidity an Temperature sensor.
    - ultrasonicSensor_HCSR04_01: HC-SR04 utrasonic distance sensor.
    - spi_01_sdCard_fatFileSystem: ADC logging in a SD/MicroSD Card (SPI connected) by using a FAT File System (ChanFS).
    - WiFi ESP01 (ESP8266) module:
       - 01_uart_bridge: Use this to send AT commands directly to ESP01 module.
       - 02_http_server: Embedded web Server to see sensor values.
       - 03_thingspeak: Send data to thingspeak dashboards.

**Embedded Operating Systems with sAPI library examples**

 - Cooperative O.S. (see M.J. Pont's book at https://www.safetty.net/publications/pttes )
    - scheduler_01_seos: Cooperative O.S. introduction.
    - scheduler_02_seos_background_foreground: Cooperative O.S., foreground-background.
    - seos_Pont2014_01: SEOS Cooperative O.S. from M.J. Pont (2014).
    - seos_pont_02_microwave: SEOS Cooperative O.S. from M.J. Pont (2014), microwave example.
 - FreeOSEK
    - freeOSEK_01_blinky: Blinky led with freeOSEK RTOS.
 - FreeRTOS
    - freeRTOS_01_blinky: Blinky led with freeRTOS RTOS and sAPI.
    - freeRTOS_02_Queue: Queue management with freeRTOS and sAPI.
    - freeRTOS_03_ChanFatFS_SPI_SdCard_ADC_log: ADC logging in a SD/MicroSD Card (SPI connected) by using a FAT File System (ChanFS), freeRTOS and sAPI.
    - freeRTOS_book: Richard Barry's book examples with FreeRTOS and sAPI:
       - EXAMPLE001: Creating tasks
       - EXAMPLE002: Using the task parameter
       - EXAMPLE003: Experimenting with priorities
       - EXAMPLE004: Using the Blocked state to create delay
       - EXAMPLE005: Converting the example tasks to use vTaskDelayUntil()
       - EXAMPLE006: Combining blocking and non-blocking tasks
       - EXAMPLE007: Defining an idle task hook function
       - EXAMPLE008: Changing task priorities
       - EXAMPLE009: Deleting tasks
       - EXAMPLE010: Blocking when receiving from a queue
       - EXAMPLE011: Blocking when sending to a queue or sending structures on a queue
       - EXAMPLE012: Using a queue set
       - EXAMPLE013: Creating one-shot and auto-reload timers
       - EXAMPLE014: Using the callback function parameter and the software timer ID
       - EXAMPLE015: Resetting a software timer
       - EXAMPLE016: Using a binary semaphore to synchronize a task with an interrupt
       - EXAMPLE017: Using a counting semaphore to synchronize a task with an interrupt
       - EXAMPLE018: Centralized deferred interrupt processing
       - EXAMPLE019: Sending and receiving on a queue from within an interrupt
       - EXAMPLE020: Re-writing vPrintString() to use a semaphore
       - EXAMPLE021: Re-writing vPrintString() to use a gatekeeper task
       - EXAMPLE022: Experimenting with event groups
       - EXAMPLE023: Synchronizing tasks
       - EXAMPLE024: Using a task notification in place of a semaphore, method 1
       - EXAMPLE025: Using a task notification in place of a semaphore, method 2

**Statecharts with sAPI library examples**
       
**LPC Open examples**

 - LPC4337 LPC Open:
     - adc_fir_dac 
     - asm
     - blinky
     - blinky_ram 	
     - blinky_rit
     - blinky_input
     - boot: LPCBootloader example
     - mpu
     - start_m0 	
     - trilat
     - i2c
     - pwm
     - sd_spi
     - usb_cdc
     - usb_msc_host
     - usb_rom_cdc
     - statechart
     - tcpecho
 - LPC4337 LPC Open and RTOS:
     - freertos_blinky
     - blinky_osek
     - multicore 	
