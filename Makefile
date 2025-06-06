########################################################################
# BSD 3-Clause License
# Copyright (c) 2017, Martin Ribelotta and Eric Pernia
# All rights reserved.
#
# Do not edit this file!!!
########################################################################

# Board ----------------------------------------------------------------

# Board default value
BOARD = edu_ciaa_nxp

# Board from an external board.mk file
-include board.mk

# Program path and name ------------------------------------------------

# Program path and name default values
PROGRAM_PATH = examples/c
PROGRAM_NAME = app

# Program path and name from an external program.mk file
-include program.mk

# Compose program path and name
ifeq ($(PROGRAM_PATH),)
PROGRAM_PATH_AND_NAME=$(PROGRAM_NAME)
else
PROGRAM_PATH_AND_NAME=$(PROGRAM_PATH)/$(PROGRAM_NAME)
endif

# Compilation and libraries configurations -----------------------------

# Compilation and libraries default values
# Compile options
VERBOSE=n
OPT=g
USE_NANO=y
USE_LTO=n
SEMIHOST=n
USE_FPU=y
ENFORCE_NOGPL=n
M0_APP=
# Libraries
USE_LPCOPEN=y
USE_SAPI=y
USE_M0=n

# Include config.mk file from program
-include $(PROGRAM_PATH_AND_NAME)/config.mk

# ----------------------------------------------------------------------

MODULES=$(sort $(dir $(wildcard libs/*/)))
SRC+=$(wildcard $(PROGRAM_PATH_AND_NAME)/src/*.c)
SRC+=$(foreach m, $(MODULES), $(wildcard $(m)/src/*.c))

CXXSRC+=$(wildcard $(PROGRAM_PATH_AND_NAME)/src/*.cpp)
CXXSRC+=$(foreach m, $(MODULES), $(wildcard $(m)/src/*.cpp))

# Arduino
INOSRC+=$(wildcard $(PROGRAM_PATH_AND_NAME)/src/*.ino)
INOSRC+=$(foreach m, $(MODULES), $(wildcard $(m)/src/*.ino))

ASRC+=$(wildcard $(PROGRAM_PATH_AND_NAME)/src/*.s)
ASRC+=$(foreach m, $(MODULES), $(wildcard $(m)/src/*.s))

ASSRC+=$(wildcard $(PROGRAM_PATH_AND_NAME)/src/*.sx)
ASSRC+=$(foreach m, $(MODULES), $(wildcard $(m)/src/*.sx))

ASSSRC+=$(wildcard $(PROGRAM_PATH_AND_NAME)/src/*.S)
ASSSRC+=$(foreach m, $(MODULES), $(wildcard $(m)/src/*.S))

OUT=$(PROGRAM_PATH_AND_NAME)/out

OBJECTS=$(INOSRC:%.ino=$(OUT)/%.o)
OBJECTS+=$(CXXSRC:%.cpp=$(OUT)/%.o)
OBJECTS+=$(SRC:%.c=$(OUT)/%.o)
OBJECTS+=$(ASRC:%.s=$(OUT)/%.o)
OBJECTS+=$(ASSRC:%.sx=$(OUT)/%.o)
OBJECTS+=$(ASSSRC:%.S=$(OUT)/%.o)

$(info $(OBJECTS))

DEPS=$(OBJECTS:%.o=%.d)

TARGET=$(OUT)/$(PROGRAM_NAME).elf
TARGET_BIN=$(basename $(TARGET)).bin
TARGET_HEX=$(basename $(TARGET)).hex
TARGET_LST=$(basename $(TARGET)).lst
TARGET_MAP=$(basename $(TARGET)).map
TARGET_NM=$(basename $(TARGET)).names.csv
TARGET_ELF=$(basename $(TARGET)).elf
TARGET_AXF=$(basename $(TARGET)).axf

INCLUDE_FLAGS=$(foreach m, $(MODULES), -I$(m)/inc) -I$(PROGRAM_PATH_AND_NAME)/inc $(INCLUDES)
DEFINES_FLAGS=$(foreach m, $(DEFINES), -D$(m))
OPT_FLAGS=-ggdb3 -O$(OPT) -ffunction-sections -fdata-sections
LIBSDEPS=$(addprefix $(OUT)/, $(addsuffix .a, $(basename $(foreach l, $(LIBS), $(foreach m, $(MODULES), $(wildcard $(m)/lib/lib$(l).hexlib) ) ))))

COMMON_FLAGS=$(ARCH_FLAGS) $(DEFINES_FLAGS) $(INCLUDE_FLAGS) $(OPT_FLAGS) -DBOARD=$(BOARD)

CFLAGS=$(COMMON_FLAGS) -std=c99
CXXFLAGS=$(COMMON_FLAGS) -fno-rtti -fno-exceptions -std=c++11
# Arduino
INOFLAGS=$(CXXFLAGS) -x c++ -include Arduino.h

LDFLAGS=$(ARCH_FLAGS)
LDFLAGS+=$(addprefix -L, $(foreach m, $(MODULES), $(wildcard $(m)/lib)))
LDFLAGS+=$(addprefix -L, $(wildcard $(dir $(LIBSDEPS))))
LDFLAGS+=$(addprefix -l, $(LIBS))
LDFLAGS+=-T$(LDSCRIPT)
LDFLAGS+=-nostartfiles -Wl,-gc-sections -Wl,-Map=$(TARGET_MAP) -Wl,--cref

#$(info Using optimization level $(OPT))
#$(info Using debug level $(DEBUG_LEVEL))

ifeq ($(USE_NANO),y)
#$(info Using newlib nano. No printf with floats supported)
LDFLAGS+=--specs=nano.specs
else
#$(info Using newlib)
endif

ifeq ($(USE_LTO),y)
$(info Using LTO)
ifeq ($(OPT),g)
$(warning "Using LTO in debug may cause inconsistences in debug")
endif
COMMON_FLAGS+=-flto
LDFLAGS+=-flto
endif

ifeq ($(SEMIHOST),y)
$(info Using semihosting)
DEFINES+=USE_SEMIHOST
LDFLAGS+=--specs=rdimon.specs
endif

CROSS=arm-none-eabi-
CC=$(CROSS)gcc
CXX=$(CROSS)g++
ifeq ($(CXXSRC),)
LD=$(CROSS)gcc
else
LD=$(CROSS)g++
endif
SIZE=$(CROSS)size
LIST=$(CROSS)objdump -xdS
OBJCOPY=$(CROSS)objcopy
NM=$(CROSS)nm
GDB=$(CROSS)gdb

# TODO: Create vervose level Q, QQ, QQQ
ifeq ($(VERBOSE),y)
Q=
else
Q=@
endif

# Build program --------------------------------------------------------

all: .m0app $(OUT) .try_enforce_no_gpl $(TARGET) $(TARGET_BIN) $(TARGET_HEX) $(TARGET_LST) $(TARGET_NM) size
	@echo 
	@echo Selected program: $(PROGRAM_PATH_AND_NAME)
	@echo Selected board: $(BOARD)

ifeq ($(M0_APP),)
.m0app: ;
.m0app_clean: ;
else
.m0app:
	@echo M0 APP: PROGRAM_PATH=$(dir $(M0_APP)) PROGRAM_NAME=$(notdir $(M0_APP))
	@$(MAKE) PROGRAM_PATH=$(dir $(M0_APP)) PROGRAM_NAME=$(notdir $(M0_APP)) USE_M0=y all

.m0app_clean:
	@echo M0 APP CLEAN: PROGRAM_PATH=$(dir $(M0_APP)) PROGRAM_NAME=$(notdir $(M0_APP))
	@$(MAKE) PROGRAM_PATH=$(dir $(M0_APP)) PROGRAM_NAME=$(notdir $(M0_APP)) USE_M0=y clean

endif

-include $(foreach m, $(MODULES), $(wildcard $(m)/module.mk))

-include $(DEPS)

$(OUT):
	@mkdir -p $@

$(OUT)/%.o: %.c
	@echo CC $(notdir $<)
	@mkdir -p $(dir $@)
	$(Q)$(CC) -MMD $(CFLAGS) -c -o $@ $<

$(OUT)/%.o: %.cpp
	@echo CXX $(notdir $<)
	@mkdir -p $(dir $@)
	$(Q)$(CXX) -MMD $(CXXFLAGS) -c -o $@ $<

$(OUT)/%.o: %.ino
	@echo ARDUINO CXX $(notdir $<)
	@mkdir -p $(dir $@)
	$(Q)$(CXX) -MMD $(INOFLAGS) -c -o $@ $<

$(OUT)/%.o: %.s
	@echo AS $(notdir $<)
	@mkdir -p $(dir $@)
	$(Q)$(CC) -MMD $(CFLAGS) -c -o $@ $<

$(OUT)/%.o: %.sx
	@echo AS $(notdir $<)
	@mkdir -p $(dir $@)
	$(Q)$(CC) -MMD $(CFLAGS) -c -o $@ $<

$(OUT)/%.o: %.S
	@echo AS $<
	@mkdir -p $(dir $@)
	$(Q)$(CC) -MMD $(CFLAGS) -c -o $@ $<

$(OUT)/%.a: %.hexlib
	@echo DEBLOB $(notdir $<)
	@mkdir -p $(dir $@)
	$(Q)$(OBJCOPY) -I ihex -O binary $< $@

$(OUT)/linker-params: $(OBJECTS) $(LIBSDEPS) Makefile
	@echo LD params
	@mkdir -p $(dir $@)
	$(Q)echo "-Wl,-( $(OBJECTS) -Wl,-) $(LDFLAGS)" > $@

$(TARGET): $(OUT)/linker-params
	@echo LD $@...
	$(Q)$(LD) -o $@ @$(OUT)/linker-params

$(TARGET_BIN): $(TARGET)
	@echo COPY $(notdir $<) TO $(notdir $@)
	@mkdir -p $(dir $@)
	$(Q)$(OBJCOPY) -O binary $< $@

$(TARGET_HEX): $(TARGET)
	@echo COPY $(notdir $<) TO $(notdir $@)
	@mkdir -p $(dir $@)
	$(Q)$(OBJCOPY) -O ihex $< $@

$(TARGET_LST): $(TARGET)
	@echo LIST
	$(Q)$(LIST) $< > $@

# If you have sed
#$(TARGET_NM): $(TARGET)
#	@echo NAME
#	$(Q)$(NM) -nAsSCp $< \
#		| sed -r 's/(.+?\:[^ ]+) ([a-zA-Z\?] [a-zA-Z_].*)/\1 00000000 \2/' \
#		| sed -r 's/(.+?)\:([a-fA-F0-9]+) ([a-fA-F0-9]+) ([a-zA-Z\?]) (.*)/\1\t0x\2\t0x\3\t\4\t\5/' \
#		> $@

# If you doesn't have sed
$(TARGET_NM): $(TARGET)
	@echo NAME
	$(Q)$(NM) -nAsSCp $< > $@

# Build program size
size: $(TARGET)
	@echo SIZEOF $(notdir $<)...
	$(Q)$(SIZE) $<

# Information
.info:
	LANG=C $(MAKE) -B -p  -r -n

# Convert axf to bin
.axf_to_bin:
	@echo Create $(PROGRAM_NAME).bin from $(PROGRAM_NAME).axf
	arm-none-eabi-objcopy -O binary $(TARGET_AXF) $(TARGET_BIN)

# Convert elf to bin
.elf_to_bin:
	@echo Create $(PROGRAM_NAME).bin from $(PROGRAM_NAME).elf
	arm-none-eabi-objcopy -O binary $(TARGET_ELF) $(TARGET_BIN)

# OpenOCD and GDB operations -------------------------------------------

# OpenOCD executable name
OOCD=openocd

# OpenOCD configuration script for board
OOCD_SCRIPT=scripts/openocd/lpc4337.cfg

ifeq ($(M0_APP),)
.download_flash_m0: ;
else
# Download program into flash memory of board
.download_flash_m0: $(TARGET_BIN) .download_flash_m0
	@echo DOWNLOAD to M0 FLASH
	$(Q)$(OOCD) -f $(OOCD_SCRIPT) \
		-c "init" \
		-c "halt 0" \
		-c "flash write_image erase $< 0x1B000000 bin" \
		-c "reset run" \
		-c "shutdown" 2>&1
endif

# Download program into flash memory of board
.download_flash: $(TARGET_BIN) .download_flash_m0
	@echo DOWNLOAD to FLASH
	$(Q)$(OOCD) -f $(OOCD_SCRIPT) \
		-c "init" \
		-c "halt 0" \
		-c "flash write_image erase $< 0x1A000000 bin" \
		-c "reset run" \
		-c "shutdown" 2>&1

# Download program into ram memory of board
.download_ram: $(TARGET_BIN)
	@echo DOWNLOAD to RAM
	$(Q)$(OOCD) -f $(OOCD_SCRIPT) \
			 -c "init" \
			 -c "halt 0" \
			 -c "load_image $< 0x20000000 bin" \
			 -c "reset run" \
			 -c "shutdown" 2>&1

# Download program into board (depends of config.mk file of program)
ifeq ($(LOAD_INRAM),y)
download: .download_ram
else
download: .download_flash
endif

# Erase Flash memory of board
erase:
	@echo ERASE FLASH
	$(Q)$(OOCD) -f $(OOCD_SCRIPT) \
		-c "init" \
		-c "halt 0" \
		-c "flash erase_sector 0 0 last" \
		-c "shutdown" 2>&1
	@echo
	@echo Done.
	@echo Please reset your Board.

# DEBUG with Embedded IDE (debug)
.debug:
	@echo DEBUG
	$(Q)$(OOCD) -f $(OOCD_SCRIPT) 2>&1

# DEBUG with Embedded IDE (run)
.run_gdb: $(TARGET)
	$(Q)socketwaiter :3333 && gdbfront $(TARGET) --start \
	--gdb arm-none-eabi-gdb \
	--gdbcmd="target remote :3333" \
	--gdbcmd="monitor reset halt" \
	--gdbcmd="load" \
	--gdbcmd="break main" \
	--gdbcmd="continue"

debug:
	@$(MAKE) -j 2 .debug .run_gdb

# TEST: Run hardware tests
.hardware_test: $(TARGET)
	$(Q)$(OOCD) -f $(OOCD_SCRIPT) > $(TARGET).log &
	$(Q)sleep 3 && arm-none-eabi-gdb -batch $(TARGET) -x scripts/openocd/gdbinit
	$(Q)cat $(TARGET).log
	$(Q)cat $(TARGET).log | grep FAIL -o >/dev/null && exit 1 || exit 0

# Remove compilation generated files -----------------------------------

# Clean current selected program
clean: .m0app_clean
	@echo CLEAN
	$(Q)rm -fR $(OBJECTS) $(TARGET) $(TARGET_BIN) $(TARGET_LST) $(DEPS) $(OUT)
	@echo 
	@echo Clean program: $(PROGRAM_PATH_AND_NAME)
	@echo Board: $(BOARD)

# Clean all programas inside this folder
clean_all:
	@echo CLEAN ALL
	@sh scripts/program/clean_all.sh

# Utilities ------------------------------------------------------------

# New program generator
new_program:
	@sh scripts/program/new_program.sh

# Select program to compile
select_program:
	@sh scripts/program/select_program.sh

# Select target board to compile
select_board:
	@sh scripts/board/select_board.sh

# TEST: Build all programs
.test_build_all:
	@sh scripts/test/test-build-all.sh

$(OUT)/gpl_check.txt:
	@grep -lE 'terms of the GNU (Lesser )?General Public License' $(CXXSRC) $(ASRC) $(SRC) > $@

.enforce_no_gpl: $(OUT)/gpl_check.txt
	@echo "CHECKING (L)GPL code in your project... "
	@[[ $(shell < $< wc -l) -ne 0 ]] && \
		echo "POSITIVE: GPL code in your project. You can see afected files in $<" || \
		echo "NEGATIVE: No GPL code in your project"

ifeq ($(ENFORCE_NOGPL),y)
.try_enforce_no_gpl: .enforce_no_gpl
else
.try_enforce_no_gpl:
endif

.print_eclipse_cfg:
	@echo $(CC) $(CFLAGS) -E -P -v -dD \"$$\{INPUTS\}\"

.gen_eclipse_cfg: Makefile board.mk program.mk $(PROGRAM_PATH_AND_NAME)/config.mk
	@mkdir -p .settings
	@echo "$(CC) $(CFLAGS) -E -P -v -dD &quot;${INPUTS}&quot;" > .settings/language.settings.xml
	@echo Settings writed to .settings/language.settings.xml

# ----------------------------------------------------------------------

.PHONY: all size download erase clean new_program select_program select_board
