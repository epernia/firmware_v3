ifeq ($(USE_TINYPRINTF),y)

TINYPRINTF_BASE=libs/tinyprintf
INCLUDES += -I$(TINYPRINTF_BASE)/include
SRC += $(wildcard $(TINYPRINTF_BASE)/*.c)
DEFINES += USE_TINYPRINTF

endif
