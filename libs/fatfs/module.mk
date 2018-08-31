ifeq ($(USE_FATFS),y)

    FATFS_BASE=libs/fatfs

    INCLUDES+=-I$(FATFS_BASE)/source
    SRC+=$(FATFS_BASE)/source/ff.c \
         $(FATFS_BASE)/source/ffunicode.c

    # the disk(s) implementation must provide ffconf.h
    # (see libs/lpc_fatfs_disks)

endif

