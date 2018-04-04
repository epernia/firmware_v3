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

Cada proyecto es una carpeta (con nombre sin espacios) que incluye dentro, una carpeta src (contiene los archivos .c), una carpeta inc (contiene los archivos .h) y un archivo config.mk, donde se ponen algunos parámetros de compilación y se elige qué bibliotecas incluirá el proyecto.

## Add a new library

La carpeta libs permite incluir una nueva bilioteca de 2 formas:

 - Biblioteca sencilla, una carpeta con adentro de libs que en su interior contenga una carpeta "src" con los archivos .c y una carpeta "inc" con los archivos .h. Este tipo de biblioteca se compila automáticamente por como está definido el makefile.
 - Biblioteca avanzada, una biblioteca compleja como LibUSB o cosas raras con una esturtura complicada de carpetas y subcarpetas. En este caso hay que hacerle un pequeño makefile que agregue dichas rutas y particularidades. Se pueden usar de ejemplos los makefiles que hay en las 3 bibliotecas que trae.


## Examples

Actualmente contamos con los siguientes, todos probados con la EDU-CIAA-NXP:

**Ejemplos Bare-metal con biblioteca sAPI**

 - GPIO:
    - gpio_01_switches_leds: teclas y leds de la EDU-CIAA-NXP.
    - gpio_02_blinky: led destellando de la EDU-CIAA-NXP.
    - gpio_03_blinky_switch: ejemplo de destello de led y manejo de tecla con destello utilizando delay no bloqueante.
    - gpio_04_led_sequences: ejemplo de secuencias de leds.
    - keypad_7segment_01: Manejo de teclado matricial y display 7 segmentos.
    - lcd_01: Manejo de display LCD.
 - UART:
    - uart_01_echo: Eco de por UART,  la EDU-CIAA-NXP responde lo que le transmito desde la PC.
    - uart_02_receive_string_blocking: Espera a recibir cierta cadena patrón por UART o sale por timeout (funciona de manera bloqueante).
    - uart_03_receive_string: Espera a recibir cierta cadena patrón por UART o sale por timeout (funciona de manera no bloqueante mediante una FSM).
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

**Ejemplos con sistemas operativos y biblioteca sAPI**

 - S.O. Cooperativos (de M.J. Pont, ver )
    - scheduler_01_seos: Introducción al sistema operativo cooperativo.
    - scheduler_02_seos_background_foreground: Sistema operativo cooperativo, variante foreground-background.
    - seos_Pont2014_01: Sistema operativo cooperativo completo de M.J. Pont.
    - seos_pont_02_microwave: Sistema operativo cooperativo completo de M.J. Pont, ejemplo de microondas.
 - FreeOSEK
    - freeOSEK_01_blinky: Ejemplo de Led destellando con freeOSEK.
 - FreeRTOS
    - freeRTOS_01_blinky: Ejemplo deLed destellando con freeRTOS.
    - freeRTOS_02_Queue: Ejemplo de utilización de colas en freeRTOS.
    - freeRTOS_03_ChanFatFS_SPI_SdCard_ADC_log: Ejemplo de logueo de valores del ADC en tarjeta SD (conectada a la EDU-CIAA-NXP por SPI) utilizando un sistema de archivos FAT (ChanFS) y freeRTOS.
    - freeRTOS_book: Ejemplos del libro de Richard Barry con FreeRTOS y sAPI. Estos son:
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

**Ejemplos sin biblioteca sAPI**

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