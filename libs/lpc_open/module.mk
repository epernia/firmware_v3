ifeq ($(USE_LPCOPEN),y)

BOARD?=edu_ciaa_nxp

DEFINES+=__USE_LPCOPEN
DEFINES+=CHIP_LPC43XX

LPCOPEN_BASE=libs/lpc_open

SRC+=$(wildcard $(LPCOPEN_BASE)/boards/src/*.c)
SRC+=$(wildcard $(LPCOPEN_BASE)/boards/$(BOARD)/src/*.c)
INCLUDES+=-I$(LPCOPEN_BASE)/boards/$(BOARD)/inc
INCLUDES+=-I$(LPCOPEN_BASE)/boards/inc

SRC+=$(wildcard $(LPCOPEN_BASE)/lpc_chip_43xx/src/*.c)

INCLUDES+=-I$(LPCOPEN_BASE)/lpc_chip_43xx/inc
INCLUDES+=-I$(LPCOPEN_BASE)/lpc_chip_43xx/usbd_rom

endif

DEFINES+=CORE_M4 __USE_NEWLIB
ARCH_FLAGS=-mcpu=cortex-m4 -mthumb

ifeq ($(USE_FPU),y)
ARCH_FLAGS+=-mfloat-abi=hard -mfpu=fpv4-sp-d16
endif

LDSCRIPT=lpc4337.ld

SRC+=$(wildcard $(LPCOPEN_BASE)/lpc_startup/src/*.c) 
INCLUDES+=-I$(LPCOPEN_BASE)/lpc_startup/inc
