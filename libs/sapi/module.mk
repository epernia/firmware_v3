ifeq ($(USE_SAPI),y)

SAPI_BASE=libs/sapi
DEFINES+=USE_SAPI
INCLUDES += -I$(SAPI_BASE)/sapi_v0.5.1/inc
SRC+=$(wildcard $(SAPI_BASE)/sapi_v0.5.1/src/*.c)

endif
