EXTERNAL_PERIPH_BASE=$(SAPI_BASE)/external_peripherals

# Display ---------------------------------------------------------------------

INCLUDES += -I$(EXTERNAL_PERIPH_BASE)/display/fonts/inc
INCLUDES += -I$(EXTERNAL_PERIPH_BASE)/display/fonts/greek_chars_5x7/inc
INCLUDES += -I$(EXTERNAL_PERIPH_BASE)/display/lcd/inc
INCLUDES += -I$(EXTERNAL_PERIPH_BASE)/display/led_segments/7segment/inc

SRC += $(wildcard $(EXTERNAL_PERIPH_BASE)/display/fonts/greek_chars_5x7/src/*.c)
SRC += $(wildcard $(EXTERNAL_PERIPH_BASE)/display/lcd/src/*.c)
SRC += $(wildcard $(EXTERNAL_PERIPH_BASE)/display/led_segments/7segment/src/*.c)

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
