ifeq ($(USE_ARDUINO),y)

ARDUINO_BASE=libs/arduino

DEFINES+=USE_ARDUINO

# Arduino core libraies

INCLUDES += -I$(ARDUINO_BASE)/arduino
INCLUDES += -I$(ARDUINO_BASE)/arduino/avr
INCLUDES += -I$(ARDUINO_BASE)/arduino/avr
INCLUDES += -I$(ARDUINO_BASE)/SPI/inc

SRC += $(wildcard $(ARDUINO_BASE)/arduino/*.c)
SRC += $(wildcard $(ARDUINO_BASE)/arduino/avr/*.c)

CXXSRC += $(wildcard $(ARDUINO_BASE)/arduino/*.cpp)
CXXSRC += $(wildcard $(ARDUINO_BASE)/SPI/src/*.cpp)
#CXXSRC += $(wildcard $(ARDUINO_BASE)/arduino/avr/*.cpp)

# Other Arduino libs
# TODO:

endif
