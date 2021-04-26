ifeq ($(USE_SAPI),y)

IDE4PLC_BASE=libs/plc/ide4plc

INCLUDES += -I$(IDE4PLC_BASE)/inc
SRC += $(wildcard $(IDE4PLC_BASE)/src/*.c)

endif
