#Allowed modifiers in config.mk
#USE_RKH: y/n  y: allows the rkh framework to be compiled
#Allowed modifiers in config.mk
#USE_FREERTOS: y/n y: uses the FreeRTOS with Dyn Memory Allocation

ifeq ($(USE_RKH),y)

RKH_BASE = libs/rkh
DEFINES += USE_RKH

#INCLUDES PATHS
INCLUDES += -I$(RKH_BASE)/fwk/inc
INCLUDES += -I$(RKH_BASE)/trc/inc
INCLUDES += -I$(RKH_BASE)/queue/inc
INCLUDES += -I$(RKH_BASE)/mempool/inc
INCLUDES += -I$(RKH_BASE)/sma/inc
INCLUDES += -I$(RKH_BASE)/sm/inc
INCLUDES += -I$(RKH_BASE)/tmr/inc

#SOURCE FILES
SRC+=$(wildcard $(RKH_BASE)/src/*.c)
SRC+=$(wildcard $(RKH_BASE)/fwk/src/*.c)
SRC+=$(wildcard $(RKH_BASE)/mempool/src/*.c)
SRC+=$(wildcard $(RKH_BASE)/queue/src/*.c)
SRC+=$(wildcard $(RKH_BASE)/sm/src/*.c)
SRC+=$(wildcard $(RKH_BASE)/sma/src/*.c)
SRC+=$(wildcard $(RKH_BASE)/trc/src/*.c)
SRC+=$(wildcard $(RKH_BASE)/tmr/src/*.c)


ifeq ($(USE_FREERTOS),y)
INCLUDES += -I$(RKH_BASE)/portable/freertos/V10_0_1
SRC+=$(wildcard $(RKH_BASE)/portable/freertos/V10_0_1/*.c)
endif

ifndef USE_FREERTOS
USE_FREERTOS=n
endif

ifeq ($(USE_FREERTOS),n)
INCLUDES += -I$(RKH_BASE)/portable/arm-cortex/rkhs/arm_cm4f/educiaa
SRC+=$(wildcard $(RKH_BASE)/portable/arm-cortex/rkhs/arm_cm4f/educiaa/*.c)
endif

endif
