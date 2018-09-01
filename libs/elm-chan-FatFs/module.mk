ifeq ($(USE_FATFS_SSP),y)
FATFS_BASE=libs/elm-chan-FatFs
DEFINES+=USE_FATFS_SSP
INCLUDES += -I$(FATFS_BASE)/fatfs_ssp/inc
SRC+=$(wildcard $(FATFS_BASE)/fatfs_ssp/src/*.c)
endif

ifeq ($(USE_FATFS_USB),y)
FATFS_BASE=libs/elm-chan-FatFs
DEFINES+=USE_FATFS_USB
INCLUDES += -I$(FATFS_BASE)/fatfs_usb/inc
SRC+=$(wildcard $(FATFS_BASE)/fatfs_usb/src/*.c)
endif