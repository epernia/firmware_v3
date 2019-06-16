ifeq ($(USE_FREERTOS),y)

BOARD ?= edu_ciaa_nxp

FREERTOS_BASE=libs/freertos
SRC+=$(FREERTOS_BASE)/MemMang/heap_$(FREERTOS_HEAP_TYPE).c

INCLUDES += -I$(FREERTOS_BASE)/include/private
INCLUDES += -I$(FREERTOS_BASE)/include

DEFINES+=USE_FREERTOS
DEFINES+=TICK_OVER_RTOS

SRC+=$(wildcard $(FREERTOS_BASE)/source/*.c)
SRC+=$(wildcard $(FREERTOS_BASE)/source/portable/*.c)

ifeq ($(USE_FPU),y)
INCLUDES += -I$(FREERTOS_BASE)/source/portable/ARM_CM4F
SRC+=$(FREERTOS_BASE)/source/portable/ARM_CM4F/port.c
else
INCLUDES += -I$(FREERTOS_BASE)/source/portable/ARM_CM3
SRC+=$(FREERTOS_BASE)/source/portable/ARM_CM3/port.c
endif


ifeq ($(BOARD),ciaa_nxp)
INCLUDES += -I$(FREERTOS_BASE)/source/portable/lpc4337
INCLUDES += -I$(FREERTOS_BASE)/source/portable/ARM_CM
SRC+=$(wildcard $(FREERTOS_BASE)/source/portable/ARM_CM/*.c)
endif

ifeq ($(BOARD),edu_ciaa_nxp)
INCLUDES += -I$(FREERTOS_BASE)/source/portable/ARM_CM
INCLUDES += -I$(FREERTOS_BASE)/source/portable/lpc4337
SRC+=$(wildcard $(FREERTOS_BASE)/source/portable/ARM_CM/*.c)
endif

ifeq ($(BOARD),lpcxpresso1769)
INCLUDES += -I$(FREERTOS_BASE)/source/portable/lpc1769
INCLUDES += -I$(FREERTOS_BASE)/source/portable/ARM_CM
SRC+=$(wildcard $(FREERTOS_BASE)/source/portable/ARM_CM/*.c)
endif

endif
