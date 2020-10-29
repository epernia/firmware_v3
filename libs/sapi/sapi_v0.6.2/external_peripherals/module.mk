EXTERNAL_PERIPH_BASE=$(SAPI_BASE)/external_peripherals

# Display ---------------------------------------------------------------------

-include $(EXTERNAL_PERIPH_BASE)/display/module.mk

# IMU -------------------------------------------------------------------------

INCLUDES += -I$(EXTERNAL_PERIPH_BASE)/imu/mpu60X0/inc
INCLUDES += -I$(EXTERNAL_PERIPH_BASE)/imu/mpu9250/inc

SRC += $(wildcard $(EXTERNAL_PERIPH_BASE)/imu/mpu60X0/src/*.c)
SRC += $(wildcard $(EXTERNAL_PERIPH_BASE)/imu/mpu9250/src/*.c)

# Keypad ----------------------------------------------------------------------

INCLUDES += -I$(EXTERNAL_PERIPH_BASE)/keypad/inc
SRC += $(wildcard $(EXTERNAL_PERIPH_BASE)/keypad/src/*.c)

# LED RGB ---------------------------------------------------------------------

INCLUDES += -I$(EXTERNAL_PERIPH_BASE)/led_rgb/inc
SRC += $(wildcard $(EXTERNAL_PERIPH_BASE)/led_rgb/src/*.c)

# MAGNETOMETER ----------------------------------------------------------------

INCLUDES += -I$(EXTERNAL_PERIPH_BASE)/magnetometer/hmc5883l/inc
INCLUDES += -I$(EXTERNAL_PERIPH_BASE)/magnetometer/qmc5883l/inc

SRC += $(wildcard $(EXTERNAL_PERIPH_BASE)/magnetometer/hmc5883l/src/*.c)
SRC += $(wildcard $(EXTERNAL_PERIPH_BASE)/magnetometer/qmc5883l/src/*.c)

# MEMORY ----------------------------------------------------------------------

INCLUDES += -I$(EXTERNAL_PERIPH_BASE)/memory/eeprom/inc
SRC += $(wildcard $(EXTERNAL_PERIPH_BASE)/memory/eeprom/src/*.c)

# MOTOR -----------------------------------------------------------------------

INCLUDES += -I$(EXTERNAL_PERIPH_BASE)/motor/servo/inc
SRC += $(wildcard $(EXTERNAL_PERIPH_BASE)/motor/servo/src/*.c)

# Temperature and Humidity ----------------------------------------------------

INCLUDES += -I$(EXTERNAL_PERIPH_BASE)/temperature_humudity/dht11/inc
SRC += $(wildcard $(EXTERNAL_PERIPH_BASE)/temperature_humudity/dht11/src/*.c)

# Ultrasonlic -----------------------------------------------------------------

INCLUDES += -I$(EXTERNAL_PERIPH_BASE)/ultrasonic/hcsr04/inc
SRC += $(wildcard $(EXTERNAL_PERIPH_BASE)/ultrasonic/hcsr04/src/*.c)

# Wi-Fi -----------------------------------------------------------------------

INCLUDES += -I$(EXTERNAL_PERIPH_BASE)/wifi/esp8266_at/inc
SRC += $(wildcard $(EXTERNAL_PERIPH_BASE)/wifi/esp8266_at/src/*.c)

# Pressure_temperature --------------------------------------------------------

INCLUDES += -I$(EXTERNAL_PERIPH_BASE)/pressure_temperature/bmp280/inc
SRC += $(wildcard $(EXTERNAL_PERIPH_BASE)/pressure_temperature/bmp280/src/*.c)

# mcp23017 --------------------------------------------------------------------

INCLUDES += -I$(EXTERNAL_PERIPH_BASE)/mcp23017/inc
SRC += $(wildcard $(EXTERNAL_PERIPH_BASE)/mcp23017/src/*.c)

# Adc Texas -------------------------------------------------------------------

INCLUDES += -I$(EXTERNAL_PERIPH_BASE)/adc/adc128d818/inc
SRC += $(wildcard $(EXTERNAL_PERIPH_BASE)/adc/adc128d818/src/*.c)