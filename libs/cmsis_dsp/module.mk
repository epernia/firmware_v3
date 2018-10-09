ifeq ($(USE_CMSIS_DSP),y)

CMSIS_DSP_BASE=libs/cmsis_dsp

DEFINES+=USE_CMSIS_DSP

INCLUDES += -I$(CMSIS_DSP_BASE)/Include

SRC += $(wildcard $(CMSIS_DSP_BASE)/Source/BasicMathFunctions/*.c)
SRC += $(wildcard $(CMSIS_DSP_BASE)/Source/CommonTables/*.c)
SRC += $(wildcard $(CMSIS_DSP_BASE)/Source/ComplexMathFunctions/*.c)
SRC += $(wildcard $(CMSIS_DSP_BASE)/Source/ControllerFunctions/*.c)
SRC += $(wildcard $(CMSIS_DSP_BASE)/Source/FastMathFunctions/*.c)
SRC += $(wildcard $(CMSIS_DSP_BASE)/Source/FilteringFunctions/*.c)
SRC += $(wildcard $(CMSIS_DSP_BASE)/Source/MatrixFunctions/*.c)
SRC += $(wildcard $(CMSIS_DSP_BASE)/Source/StatisticsFunctions/*.c)
SRC += $(wildcard $(CMSIS_DSP_BASE)/Source/SupportFunctions/*.c)
SRC += $(wildcard $(CMSIS_DSP_BASE)/Source/TransformFunctions/*.c)

#ASRC += $(wildcard $(CMSIS_DSP_BASE)/Source/TransformFunctions/*.S)
# Borre este archivo porque no compilaba.

endif
