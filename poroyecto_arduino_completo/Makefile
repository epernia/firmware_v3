PROJECT := blinking
VERBOSE := n
USE_NANO := y
LIBS:=nosys
OPT_LEVEL := g
DEBUG_LEVEL := 3

LIBSFOLDER:=cores
LIBSPATH:=$(LIBSFOLDER)/variants/LPC4337/linker_scripts
SRC_DIRS:=sketch $(LIBSFOLDER)/LPC4337/lpcopen $(LIBSFOLDER)/LPC4337/sapi $(LIBSFOLDER)/LPC4337/arduino 
INCLUDES:=$(SRC_DIRS)
LD_SCRIPTS:=lpc4337_m4_lib lpc4337_m4_mem lpc4337_m4
#OOCD_CFG:=$(LIBSFOLDER)/variants/LPC4337/openocd/lpc4337_cd.cfg
OOCD_CFG:=$(LIBSFOLDER)/variants/LPC4337/openocd/lpc4337_fv3.cfg

C_SRC = $(foreach m, $(SRC_DIRS), $(wildcard $(m)/*.c))
A_SRC = $(foreach m, $(MODULES), $(wildcard $(m)/src/*.S))
CXX_SRC = $(foreach m, $(SRC_DIRS), $(wildcard $(m)/*.cpp))
INO_SRC = $(foreach m, $(SRC_DIRS), $(wildcard $(m)/*.ino))
OUT = out
OBJS = $(addprefix $(OUT)/, $(INO_SRC:%.ino=%.o) $(CXX_SRC:%.cpp=%.o) $(C_SRC:%.c=%.o) $(A_SRC:%.S=%.o))
TARGET := $(OUT)/$(PROJECT).elf
TARGET_MAP := $(OUT)/$(PROJECT).map
TARGET_HEX := $(OUT)/$(PROJECT).hex

CROSS = arm-none-eabi-
CC = $(CROSS)gcc
CXX = $(CROSS)g++
LD = $(CROSS)g++
MD = mkdir -p

OPT=-O$(OPT_LEVEL)
DBG=-g$(DEBUG_LEVEL)
DEFINES:=CORE_M4 __USE_LPCOPEN MULTICORE __LPC43XX__ __CODE_RED LPC43_MULTICORE_M0APP __MULTICORE_MASTER __MULTICORE_MASTER_SLAVE_M0APP ARCH=cortexM4 CPUTYPE=lpc43xx CPU=lpc4337 BOARD=CIAA_NXP lpc4337_m4

INCLUDES_FLAGS:=$(foreach i, $(INCLUDES), -I$(i))
DEFINES_FLAGS:=$(foreach d, $(DEFINES), -D$(d))
LIBS_FLAGS:=$(foreach L, $(LIBS), -l$(L))
LIBPATHS_FLAGS:=$(foreach L, $(LIBSPATH), -L$(L))
LD_SCRIPTS_FLAGS:=$(foreach S, $(LD_SCRIPTS), -T$(S).ld)

ARCH:=-mcpu=cortex-m4 -mfpu=fpv4-sp-d16 -mfloat-abi=softfp -mthumb
COMMON_FLAGS:=-c -MMD $(ARCH) $(DEBUG) $(OPT) -ffunction-sections -fdata-sections -nostdlib --param max-inline-insns-single=500 $(DEFINES_FLAGS) $(INCLUDES_FLAGS)
CFLAGS:=$(COMMON_FLAGS) -std=gnu99
CXXFLAGS:=$(COMMON_FLAGS) -std=gnu++11 -fno-rtti -fno-exceptions
INOFLAGS:=-x c++ -include Arduino.h $(CXXFLAGS)
LDFLAGS:=-Wl,--gc-sections $(ARCH) -Xlinker -nostdlib -fno-builtin -Wl,-Map=$(TARGET_MAP) $(LIBPATHS_FLAGS) $(LIBS_FLAGS) $(LD_SCRIPTS_FLAGS)

ifeq ($(VERBOSE),y)
V=
else
V=@
endif

$(info using optimization level $(OPT_LEVEL))

ifeq ($(USE_NANO),y)
$(info using newlib nano)
LDFLAGS+=--specs=nano.specs
endif

ifeq ($(SEMIHOST),y)
$(info using semihosting)
LDFLAGS+=--specs=rdimon.specs
endif

all: $(TARGET) $(TARGET_HEX) size

$(OUT)/%.o: %.c
	@echo CC $<
	@$(MD) $(dir $@)
	$(V)$(CC) $(CFLAGS) $< -o $@

$(OUT)/%.o: %.cpp
	@echo CXX $<
	@$(MD) $(dir $@)
	$(V)$(CXX) $(CXXFLAGS) $< -o $@

$(OUT)/%.o: %.ino
	@echo ARDUINO CXX $<
	@$(MD) $(dir $@)
	$(V)$(CXX) $(INOFLAGS) $< -o $@

$(OUT)/%.o: %.S
	@echo AS $<
	@$(MD) $(dir $@)
	$(V)$(CC) -c $(CFLAGS) $< -o $@

$(TARGET): $(OBJS) $(LD_SCRIPT)
	@echo LINK $(notdir $@)
	@$(MD) $(dir $@)
	$(V)$(LD) $(LDFLAGS) -o $@ $(OBJS)

$(TARGET_HEX): $(TARGET)
	@echo GEN $(notdir $@)
	$(V)$(CROSS)objcopy -O ihex $< $@

size: $(TARGET)
	@echo SIZEOF $(notdir $<)...
	$(V)$(CROSS)size $<

download: $(TARGET_HEX)
	@echo DOWNLOAD $(notdir $<)
	$(V)openocd -f $(OOCD_CFG) \
		-c "init" \
		-c "halt 0" \
		-c "flash write_image erase $< 0x1A000000 bin" \
		-c "reset run" \
		-c "shutdown" 2>&1

erase_flash:
	@echo ERASE FW
	$(V)openocd -f $(OOCD_CFG) \
		-c "init" \
		-c "halt 0" \
		-c "flash erase_sector 0 0 last" \
		-c "shutdown" 2>&1		
	@echo
	@echo Listo.
	@echo Por favor resetee la placa.

clean:
	@echo CLEAN
	$(V)rm -fr $(OUT)

debug: $(TARGET_ELF)
	@echo START DEBUG
	$(V)openocd -f $(OOCD_CFG) 

.PHONY: all clean size download erase_flash debug
