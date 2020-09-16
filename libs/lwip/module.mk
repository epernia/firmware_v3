ifeq ($(USE_LWIP),y)

LWIP_BASE=libs/lwip

DEFINES+=USE_LWIP

INCLUDES += -I$(LWIP_BASE)/inc
INCLUDES += -I$(LWIP_BASE)/inc/ipv4

SRC += $(wildcard $(LWIP_BASE)/src/api/*.c)
SRC += $(wildcard $(LWIP_BASE)/src/arch/*.c)
SRC += $(wildcard $(LWIP_BASE)/src/core/*.c)
SRC += $(wildcard $(LWIP_BASE)/src/core/ipv4/*.c)
SRC += $(wildcard $(LWIP_BASE)/src/netif/*.c)

endif
