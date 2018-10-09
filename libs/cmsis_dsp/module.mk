CMSIS_DSP_BASE=libs/cmsis_dsp
SRC += $(wildcard $(CMSIS_DSP_BASE)/src/*/*.c)

ifeq ($(USE_FPU),y)
LIBS += arm_cortexM4lf_math
else
LIBS += arm_cortexM4l_math
endif
