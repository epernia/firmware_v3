# Ejemplos

Los ejemplos incluidos son:

### Asembler de ARM Cortex M4

 - `asm_lpc_open`: asembler con biblioteca LPC Open.
 - `asm_sapi`: asembler con biblioteca sAPI.

### Lenguaje C

 - **sAPI library**
    - Bare metal:
        - `adc_dac`: ADC y DAC.
        - bluetooth
            - `hm10_uart_bridge`: Transimsión y recepción con módulo Bluettoth HM10 por comandos AT.
        - `cycles_counter`: Funciones para conteo de cciclios de clocks, solo funcionan en modo debug. Permite realizar trazabilidad sobre el tiempo de ejecución del programa.
        - `dht11_temp_humidity`: Sensor de humedad y temperatura DHT11.
        - gpio
            - `gpio_01_switches_leds`: cada botón activa el led sobre el mismo.
            - `gpio_02_blinky`: led destellando con retardo bloqueante.
            - `gpio_03_blinky_switch`: led destellando con retardo no bloqueante que permite controlar otro led con un switch al mismo tiempo.
            - `gpio_04_led_sequences`: secuencia de leds usando retardo no bloqueante.
        - `hcsr04_ultrasonic_sensor`:  Sensor ultrasónico de distancia HC-SR04.
        - i2c:
            - IMUs
                - `mpu9250`: MPU9250 IMU 9 grados de libertad (Giróscopo, Acelerómetro y Magnetómettro) + Temperatura.
            - magnetometers. El módulo Chino GY-273 puede estar soldado uno de estos 2 magnetómetros, que tiene el mismo pinout pero diferente mapa de registros. Para diferenciarlos se debe ignorar la serigrafía de la placa y revisar el código del chip.
                - `hmc5883l`: Magnetómetro HMC5883L. 
                - `qmc5883l`: Magnetómetro QMC5883L.
        - `keypad_7segment`: Teclado matricial 4x4 y display 7 segmentos.
        - `lcd`: Display LCD (por GPIO o I2C).
        - pwm
            - `pwm_01`: PWM aplicado a dimerizado de LEDs.
            - `pwm_02_rgb_controller_uart`: Led RGB.
        - `rtc`: Periférico RTC para tener fecha  hora.
        - `sapi_basic_example`: ejemplo básico con biblioteca sAPI.
        - `servo`: Control de servomotor angular con PWM.
        - spi_sd_card
            - `spi_sd_card_01_wite_file`: Escribir archivo de texto en una tarjeta SD/MicroSD (conectada por SPI) utilizando un sistema de archivos FAT (biblioteca Elm-Chan FAT FS).
            - `spi_sd_card_02_log_time_stamp`: Escribir archivo de texto en una tarjeta SD/MicroSD (conectada por SPI) utilizando un sistema de archivos FAT (biblioteca Elm-Chan FAT FS) con estampa de tiempo (*time-stamp*)  en las muestras.
            - `spi_sd_card_03_list_files`: Listar los archivos y carpetas coontenidos en una tarjeta SD/MicroSD (conectada por SPI) utilizando un sistema de archivos FAT (biblioteca Elm-Chan FAT FS). Muestra el resultado por UART.
        - `tick_hook`: periodic tick function (interrupt-based) with periodic callback.
        - uart
            - `uart_01_echo`: UART echo, it respond the same that you send from PC.
            - `uart_02_receive_string_blocking`: waits until receive a certain pattern String in a UART or timeout expire (blocking code). Don't save received string.
            - `uart_03_receive_string`: waits until receive a certain pattern String in a UART or timeout expire (non-blocking code). Don't save received string.
        - wifi_esp8266
            - `wifi_esp8266_01_uart_bridge`: use this to send AT commands directly to ESP01 module.
            - `wifi_esp8266_02_thingspeak`: send data to thingspeak dashboards.
        - usb
            - `usb_device_01_hid_keyboard`: USB Device. Board as USB Keyboard.
            - `usb_device_02_cdc_virtualSerialPort`: USB Device. Using USB OTG as a virtual serial port.
    - Operating Systems:
        - rtos_cooperative (see M.J. Pont's book at https://www.safetty.net/publications/pttes )
            - `scheduler_01_seos`: first cooperative scheduler
            - `scheduler_02_seos_background_foreground`: second cooperative scheduler
            - `seos_pont_2014_01_first_app`: third cooperative scheduler
        - rtos_freertos
            - dynamic_mem
                - `freeRTOS_01_blinky`
                - `freeRTOS_03_ChanFatFS_SPI_SdCard_ADC_log`: ADC logging in a SD/MicroSD Card (SPI connected) by using a FAT File System (ChanFS), freeRTOS and sAPI.
            - static_mem
                - `freeRTOS_01_blinky`
 - **LPC Open library**
    - bare_metal
        - `blinky`
        - `blinky_input`
        - `blinky_ram`
        - `blinky_rit`
        - `boot`
        - `i2c`
        - `lpc_open_basic_example`
        - `mpu`
        - `pwm`
    - operating_systems
        - `freertos_blinky`
    - statecharts
        - `statechart`
 - `newlib_printf_scanf`
 - `without_libs`

### Lenguaje C++

 - `cpp_sapi_basic_example`: a C++ basic example with sAPI library.
