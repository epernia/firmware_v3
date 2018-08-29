ifeq ($(USE_LPCOPEN),y)

    ifeq ($(USE_FATFS),y)

        LPCOPEN_FATFS_DISKS_BASE=libs/lpc_fatfs_disks

        INCLUDES+=-I$(LPCOPEN_FATFS_DISKS_BASE)/source
        SRC+=$(LPCOPEN_FATFS_DISKS_BASE)/source/fssdc.c \
             $(LPCOPEN_FATFS_DISKS_BASE)/source/ffdisks.c

        ifeq ($(USE_LPCUSBLIB),y)
            DEFINES+=LPCUSBLIB_HOST_MASS_STORAGE
            SRC+=$(LPCOPEN_FATFS_DISKS_BASE)/source/fsusb.c 
        endif

    endif

endif

