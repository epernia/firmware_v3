ifeq ($(USE_LPCOPEN),y)

    ifeq ($(USE_FATFS),y)

        LPCOPEN_FATFS_DISKS_BASE=libs/lpc_fatfs_disks
        SAPI_FATFS=$(LPCOPEN_FATFS_DISKS_BASE)/sapi

        INCLUDES+=-I$(LPCOPEN_FATFS_DISKS_BASE)/source \
                  -I$(SAPI_FATFS)/inc

        SRC+=$(LPCOPEN_FATFS_DISKS_BASE)/source/fssdc.c \
             $(LPCOPEN_FATFS_DISKS_BASE)/source/ffdisks.c \
             $(SAPI_FATFS)/src/sapi_sdcard.c

        ifeq ($(USE_LPCUSBLIB),y)
            DEFINES+=LPCUSBLIB_HOST_MASS_STORAGE
            SRC+=$(LPCOPEN_FATFS_DISKS_BASE)/source/fsusb.c \
                 $(SAPI_FATFS)/src/sapi_usbms.c
        endif

    endif

endif

