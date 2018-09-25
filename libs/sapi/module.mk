ifeq ($(USE_SAPI),y)

SAPI_BASE=libs/sapi/sapi_v0.5.2

DEFINES+=USE_SAPI

INCLUDES += -I$(SAPI_BASE)/base/inc
INCLUDES += -I$(SAPI_BASE)/soc/core/inc
INCLUDES += -I$(SAPI_BASE)/soc/peripherals/inc
INCLUDES += -I$(SAPI_BASE)/soc/peripherals/usb/device/inc
INCLUDES += -I$(SAPI_BASE)/soc/peripherals/usb/host/inc
INCLUDES += -I$(SAPI_BASE)/board/inc
INCLUDES += -I$(SAPI_BASE)/abstract_modules/inc
INCLUDES += -I$(SAPI_BASE)/external_peripherals/inc

SRC += $(wildcard $(SAPI_BASE)/base/src/*.c)
SRC += $(wildcard $(SAPI_BASE)/soc/core/src/*.c)
SRC += $(wildcard $(SAPI_BASE)/soc/peripherals/src/*.c)
SRC += $(wildcard $(SAPI_BASE)/soc/peripherals/usb/device/src/*.c)
SRC += $(wildcard $(SAPI_BASE)/soc/peripherals/usb/host/src/*.c)
SRC += $(wildcard $(SAPI_BASE)/board/src/*.c)
SRC += $(wildcard $(SAPI_BASE)/abstract_modules/src/*.c)
SRC += $(wildcard $(SAPI_BASE)/external_peripherals/src/*.c)

endif
