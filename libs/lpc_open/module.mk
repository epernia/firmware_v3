ifeq ($(USE_LPCOPEN),y)

DEFINES+=__USE_LPCOPEN

LPCOPEN_BASE=libs/lpc_open

SRC+=$(wildcard $(LPCOPEN_BASE)/lpc_board_edu_ciaa_nxp/src/*.c)
INCLUDES+=-I$(LPCOPEN_BASE)/lpc_board_edu_ciaa_nxp/inc

SRC+=$(wildcard $(LPCOPEN_BASE)/lpc_chip_43xx/core/common/src/*.c)
SRC+=$(wildcard $(LPCOPEN_BASE)/lpc_chip_43xx/core/core_m0/src/*.c)
SRC+=$(wildcard $(LPCOPEN_BASE)/lpc_chip_43xx/core/core_m4/src/*.c)

INCLUDES+=-I$(LPCOPEN_BASE)/lpc_chip_43xx/core/common/inc
INCLUDES+=-I$(LPCOPEN_BASE)/lpc_chip_43xx/core/core_m0/inc
INCLUDES+=-I$(LPCOPEN_BASE)/lpc_chip_43xx/core/core_m4/inc

SRC+=$(wildcard $(LPCOPEN_BASE)/lpc_chip_43xx/peripherals/src/*.c)

INCLUDES+=-I$(LPCOPEN_BASE)/lpc_chip_43xx/peripherals/inc
INCLUDES+=-I$(LPCOPEN_BASE)/lpc_chip_43xx/peripherals/usbd_rom

endif

DEFINES+=CORE_M4 __USE_NEWLIB
ARCH_FLAGS=-mcpu=cortex-m4 -mthumb

ifeq ($(USE_FPU),y)
ARCH_FLAGS+=-mfloat-abi=hard -mfpu=fpv4-sp-d16
endif

LDSCRIPT=lpc4337.ld

SRC+=$(wildcard $(LPCOPEN_BASE)/lpc_startup/src/*.c) 
INCLUDES+=-I$(LPCOPEN_BASE)/lpc_startup/inc

