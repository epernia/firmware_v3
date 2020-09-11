ifeq ($(USE_LPCOPEN),y)

BOARD ?= edu_ciaa_nxp

DEFINES+=__USE_LPCOPEN
DEFINES+=CHIP_LPC43XX

ifeq ($(USE_M0),y)
DEFINES+=ARM_MATH_CM0
else
DEFINES+=ARM_MATH_CM4
endif

LPCOPEN_BASE=libs/lpc_open

SRC+=$(wildcard $(LPCOPEN_BASE)/boards/$(BOARD)/src/*.c)
INCLUDES+=-I$(LPCOPEN_BASE)/boards/$(BOARD)/inc
INCLUDES+=-I$(LPCOPEN_BASE)/boards/inc

SRC+=$(wildcard $(LPCOPEN_BASE)/lpc_chip_43xx/src/*.c)

INCLUDES+=-I$(LPCOPEN_BASE)/lpc_chip_43xx/inc
INCLUDES+=-I$(LPCOPEN_BASE)/lpc_chip_43xx/usbd_rom

endif

DEFINES+=__USE_NEWLIB

ARCH_FLAGS=-mcpu=cortex-m0 -mthumb

ifeq ($(USE_M0),y)
DEFINES+=CORE_M0
else
DEFINES+=CORE_M4
ARCH_FLAGS=-mcpu=cortex-m4 -mthumb

ifeq ($(USE_FPU),y)
DEFINES+=__FPU_PRESENT=1U
ARCH_FLAGS+=-mfloat-abi=hard -mfpu=fpv4-sp-d16
else
DEFINES+=__FPU_PRESENT=OU
endif

endif

ifeq ($(USE_M0),y)
LDSCRIPT=link_m0.ld
else
ifeq ($(LOAD_INRAM),y)
LDSCRIPT=flat.ld
else
LDSCRIPT=link.ld
endif
endif

SRC+=$(wildcard $(LPCOPEN_BASE)/lpc_startup/src/*.c) 
INCLUDES+=-I$(LPCOPEN_BASE)/lpc_startup/inc
