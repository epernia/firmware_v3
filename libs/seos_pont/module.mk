ifeq ($(USE_SAPI),y)

SEOS_PONT_BASE=libs/seos_pont/seos_pont_2014

INCLUDES += -I$(SEOS_PONT_BASE)/inc
SRC += $(wildcard $(SEOS_PONT_BASE)/src/*.c)

endif
