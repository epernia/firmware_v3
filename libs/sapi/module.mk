ifeq ($(USE_SAPI),y)

SAPI_BASE=libs/sapi
DEFINES+=USE_SAPI
INCLUDES += -I$(SAPI_BASE)/sapi_r0.5.0/inc
SRC+=$(wildcard $(SAPI_BASE)/sapi_r0.5.0/src/*.c)

endif
