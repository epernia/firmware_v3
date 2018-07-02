#==============================================================================
# Select a project to compile, or comment and uncomment an example
#==============================================================================

# ---------- Project path ----------------------------------------------
# Project path 
# (relative to this folder, leave void if the project is in this folder)
PROJECT_PATH =
# ---------- Project names ---------------------------------------------
PROJECT_NAME = app

#==============================================================================
# Examples
# Notas: Los proyectos indentados compilan pero no se probo con el periferico
#        desde el pasaje de Firmware v2 a este repo.
#        Los proyectos marcados con ################# no compilan.
#==============================================================================

#------------------------------------------------------------------------------
# Languages and libs
#------------------------------------------------------------------------------

#---------- Projects path ---------------------------------------------
#PROJECT_PATH = examples/languages_and_libs
#---------- Project names (uncomment only one) ------------------------
#PROJECT_NAME = C_without_libs
#PROJECT_NAME = C_LPCOpen
#PROJECT_NAME = C_sAPI
#PROJECT_NAME = CPP_sAPI
#PROJECT_NAME = C_FreeRTOS_sAPI
#PROJECT_NAME = ASM_LPCOpen
#PROJECT_NAME = ASM_sAPI

#------------------------------------------------------------------------------
# sAPI bare metal
#------------------------------------------------------------------------------

#---------- Projects path ---------------------------------------------
#PROJECT_PATH = examples/sAPI/bare_metal
#---------- Project names (uncomment only one) ------------------------
	#PROJECT_NAME = adc_dac_01
#PROJECT_NAME = cycles_counter_01
	#PROJECT_NAME = keypad_7segment_01
	#PROJECT_NAME = lcd_01
#PROJECT_NAME = rtc_01
	#PROJECT_NAME = servo_01
		# Nota: Este ejemplo de servo puede requerir un borrado completo de
		# la memoria flash luego de su uso para grabar programas posteriores.
		# Para hcerlo debe puentear el Jumper JP5 y resetear la placa con el
		# puente sostenido hasta soltar el boton de reset entrando en modo
		# ISP (se prenden todos los leds), luego aplique "make erase" y debe
		# resetear nuevamente la placa para permitir grabar nuevos programas.
		# Tenga en cuenta tambien la corriente maxima del regulador al
		# conectar uno o mas servos.
#PROJECT_NAME = spi_01_sdCard_fatFileSystem
#PROJECT_NAME = stdio_01_printf_sprintf
#PROJECT_NAME = tick_01_tickHook

# ---------- Projects path ---------------------------------------------
#PROJECT_PATH = examples/sAPI/bare_metal/gpio
# ---------- Project names (uncomment only one) ------------------------
#PROJECT_NAME = gpio_01_switches_leds
#PROJECT_NAME = gpio_02_blinky
#PROJECT_NAME = gpio_03_blinky_switch
#PROJECT_NAME = gpio_04_led_sequences

	#---------- Projects path ---------------------------------------------
	#PROJECT_PATH = examples/sAPI/bare_metal/i2c
	#---------- Project names (uncomment only one) ------------------------
	#PROJECT_NAME = i2c_02_qmc5883l
	#PROJECT_NAME = i2c_01_hmc5883l

# ---------- Projects path ---------------------------------------------
#PROJECT_PATH = examples/sAPI/bare_metal/pwm
# ---------- Project names (uncomment only one) ------------------------
#PROJECT_NAME = pwm_01
#PROJECT_NAME = pwm_02_rgb_controller_uart

	#---------- Projects path ---------------------------------------------
	#PROJECT_PATH = examples/sAPI/bare_metal/temperature_humidity_sensors
	#---------- Project names (uncomment only one) ------------------------
	#PROJECT_NAME = dht11_01

#---------- Projects path ---------------------------------------------
#PROJECT_PATH = examples/sAPI/bare_metal/uart
#---------- Project names (uncomment only one) ------------------------
#PROJECT_NAME = uart_01_echo
#PROJECT_NAME = uart_02_receive_string_blocking
#PROJECT_NAME = uart_03_receive_string

	#---------- Projects path ---------------------------------------------
	#PROJECT_PATH = examples/sAPI/bare_metal/ultrasonicSensors
	#---------- Project names (uncomment only one) ------------------------
	#PROJECT_NAME = ultrasonicSensor_HCSR04_01

	#---------- Projects path ---------------------------------------------
	#PROJECT_PATH = examples/sAPI/bare_metal/ultrasonicSensors
	#---------- Project names (uncomment only one) ------------------------
	#PROJECT_NAME = ultrasonicSensor_HCSR04_01

#---------- Projects path ---------------------------------------------
#PROJECT_PATH = examples/sAPI/bare_metal/wifi_esp01_esp8266
#---------- Project names (uncomment only one) ------------------------
#PROJECT_NAME = 01_uart_bridge
#PROJECT_NAME = 02_http_server
#PROJECT_NAME = 03_thingspeak

#------------------------------------------------------------------------------
# sAPI Operating Systems
#------------------------------------------------------------------------------

#---------- Projects path ---------------------------------------------
#PROJECT_PATH = examples/sAPI/operating_systems/coperative
#---------- Project names (uncomment only one) ------------------------
#PROJECT_NAME = scheduler_01_seos
#PROJECT_NAME = scheduler_02_seos_background_foreground
#PROJECT_NAME = seos_Pont2014_01_firstApp
#PROJECT_NAME = seos_Pont2014_02_microwave

#---------- Projects path ---------------------------------------------
#PROJECT_PATH = examples/sAPI/operating_systems/freeRTOS
#---------- Project names (uncomment only one) ------------------------
#PROJECT_NAME = freeRTOS_01_blinky
#################PROJECT_NAME = freeRTOS_02_Queue
	#################PROJECT_NAME = freeRTOS_03_ChanFatFS_SPI_SdCard_ADC_log

	#---------- Projects path ---------------------------------------------
	#PROJECT_PATH = examples/sAPI/operating_systems/freeRTOS/freeRTOS_book
#################---------- Project names (uncomment only one) ------------------------
	#PROJECT_NAME = Example001
	#PROJECT_NAME = Example002
	#PROJECT_NAME = Example003
	#PROJECT_NAME = Example004
	#PROJECT_NAME = Example005
	#PROJECT_NAME = Example006
	#PROJECT_NAME = Example007
	#PROJECT_NAME = Example008
	#PROJECT_NAME = Example009
	#PROJECT_NAME = Example010
	#PROJECT_NAME = Example011
	#PROJECT_NAME = Example012
	#PROJECT_NAME = Example013
	#PROJECT_NAME = Example014
	#PROJECT_NAME = Example015
	#PROJECT_NAME = Example016
	#PROJECT_NAME = Example017
	#PROJECT_NAME = Example018
	#PROJECT_NAME = Example019
	#PROJECT_NAME = Example020
	#PROJECT_NAME = Example021
	#PROJECT_NAME = Example022

#---------- Projects path ---------------------------------------------
#PROJECT_PATH = examples/sAPI/operating_systems/freeOSEK
#---------- Project names (uncomment only one) ------------------------
#################PROJECT_NAME = freeOSEK_01_blinky # OSEK aun no esta portado

#------------------------------------------------------------------------------
# sAPI Statecharts
#------------------------------------------------------------------------------

#---------- Projects path ---------------------------------------------
#PROJECT_PATH = examples/sAPI/statecharts
#---------- Project names (uncomment only one) ------------------------
#################PROJECT_NAME = statecharts_bare_metal

#---------- Projects path ---------------------------------------------
#PROJECT_PATH = examples/sAPI/statecharts/statecharts_os/coperative_os_examples
#---------- Project names (uncomment only one) ------------------------
#PROJECT_NAME = 

#---------- Projects path ---------------------------------------------
#PROJECT_PATH = examples/sAPI/statecharts/statecharts_os/freeRTOS_examples
#---------- Project names (uncomment only one) ------------------------
#PROJECT_NAME = 


#------------------------------------------------------------------------------
# LPC Open
#------------------------------------------------------------------------------

#---------- Projects path ---------------------------------------------
#PROJECT_PATH = examples/lpcOpen
#---------- Project names (uncomment only one) ------------------------
##########################PROJECT_NAME = adc_fir_dac   # arreglar el makefile
#PROJECT_NAME = blinky
#PROJECT_NAME = blinky_ram
#PROJECT_NAME = blinky_rit
##########################PROJECT_NAME = boot
##########################PROJECT_NAME = freertos_blinky
#PROJECT_NAME = i2c	# Solo se testeo compilacion
##########################PROJECT_NAME = mpu
##########################PROJECT_NAME = statechart

#---------- Projects path ---------------------------------------------
#PROJECT_PATH = examples/lpcOpen/asm
#---------- Project names (uncomment only one) ------------------------
##########################PROJECT_NAME = first
##########################PROJECT_NAME = conv
##########################PROJECT_NAME = packer
