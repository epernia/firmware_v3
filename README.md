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

 - Simplie library. Consist in a folder (with a non-spaces name) that includes inside 2 folders, one named ```src``` (here go, .c, .cpp or .s source code files), and another one named ```inc``` (here go, .h or .hpp source header files). This kind of library compiles automaticaly by the Makefile.
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
    - stdio_01_printf_sprintf: Ejemplo de uso de printf() y similares de la biblioteca de C estándar.
 - ADC, DAC:
    - adc_dac_01: Utilización de los periféricos ADC y DAC de la EDU-CIAA-NXP.
 - TIMER, RTC:
    - cycles_counter_01: Funciones de conteo de ciclos de reloj, funciona únicamente en modo debug y permite tener trazabilidad de tiempos de ejecución.
    - tick_01_tickHook: Función de tick periódico y como ejecutar periódicamente una función callback desde dicha interrupción.
    - rtc_01: Periférico RTC para reloj de fecha/hora.
    - pwm_01: Ejemplo de PWM aplicado a los LEDs de la EDU-CIAA-NXP.
    - pwm_02_rgb_controller_uart: Control de LED RGB para formar cualquier color.
    - servo_01: Ejemplo de Servomotor conectado a la EDU-CIAA-NXP.
 - Periféricos externos:
    - Magnetómetros I2C. Puede venir cualquiera de ellos en el módulo GY-273, son compatibles en cuanto a disposición y tamaño de pines pero con diferente mapa de registros. Para diferenciarlos revise el chip con una lupa:
       - i2c_01_hmc5883l: Ejemplo de magnetómetro HMC5883L. 
       - i2c_02_qmc5883l: Ejemplo de magnetómetro QMC5883L.
    - dht11_01: Sensor de humedad y temperatura.
    - ultrasonicSensor_HCSR04_01: Sensor ultrasónico de  distancia HC-SR04.
    - spi_01_sdCard_fatFileSystem: Ejemplo de logueo de valores del ADC en tarjeta SD (conectada a la EDU-CIAA-NXP por SPI) utilizando un sistema de archivos FAT (ChanFS).
    - Módulo WiFi ESP01 (ESP8266):
       - 01_uart_bridge: Conexión directa con módulo ESP01 para enviar comandos AT al mismo.
       - 02_http_server: Servidor embebido en la EDU-CIAA-NXP para ver datos de la misma.
       - 03_thingspeak: Envío de datos a un dashboard en thingspeak para visualización de datos por internet.

**Embedded Operating Systems with sAPI library examples**

 - Cooperative O.S. Cooperativos (from M.J. Pont, see at https://www.safetty.net/publications/pttes )
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

**LPC Open examples**

 - LPC4337:
     - adc_fir_dac 
     - asm
     - blinky
     - blinky_ram 	
     - blinky_rit 	
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
 - LPC4337 RTOS:
     - freertos_blinky
     - blinky_osek
     - multicore 	