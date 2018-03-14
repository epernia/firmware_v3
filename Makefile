include project.mk
ifeq ($(PROJECT_PATH),)
PROJECT_PATH_AND_NAME=$(PROJECT_NAME)
else
PROJECT_PATH_AND_NAME=$(PROJECT_PATH)/$(PROJECT_NAME)
endif

include $(PROJECT_PATH_AND_NAME)/config.mk

MODULES=$(sort $(dir $(wildcard libs/*/)))
SRC=$(wildcard $(PROJECT_PATH_AND_NAME)/src/*.c)
SRC+=$(foreach m, $(MODULES), $(wildcard $(m)/src/*.c))

ASRC=$(wildcard $(PROJECT_PATH_AND_NAME)/src/*.s)
ASCR+=$(foreach m, $(MODULES), $(wildcard $(m)/src/*.s))

OUT=$(PROJECT_PATH_AND_NAME)/out
OBJECTS=$(ASRC:%.s=$(OUT)/%.o) $(SRC:%.c=$(OUT)/%.o)
DEPS=$(OBJECTS:%.o=%.d)

OOCD_SCRIPT=scripts/openocd.cfg

TARGET=$(OUT)/$(PROJECT_NAME).elf
TARGET_BIN=$(basename $(TARGET)).bin
TARGET_LST=$(basename $(TARGET)).lst
TARGET_MAP=$(basename $(TARGET)).map
TARGET_NM=$(basename $(TARGET)).names.csv

CFLAGS=$(ARCH_FLAGS)
CFLAGS+=$(foreach m, $(MODULES), -I$(m)/inc) -I$(PROJECT_PATH_AND_NAME)/inc $(INCLUDES)
CFLAGS+=$(foreach m, $(DEFINES), -D$(m))
CFLAGS+=-ggdb3 -O$(OPT)
CLFAGS+=-ffunction-sections -fdata-sections

LDFLAGS=$(ARCH_FLAGS)
LDFLAGS+=$(foreach m, $(MODULES), -L$(m)/lib)
LDFLAGS+=-Tlink.ld
LDFLAGS+=-nostartfiles -Wl,-gc-sections -Wl,-Map=$(TARGET_MAP) -Wl,--cref

ifeq ($(USE_NANO),y)
LDFLAGS+=--specs=nano.specs
endif

ifeq ($(SEMIHOST),y)
LDFLAGS+=--specs=rdimon.specs
endif

CROSS=arm-none-eabi-
CC=$(CROSS)gcc
LD=$(CROSS)gcc
SIZE=$(CROSS)size
LIST=$(CROSS)objdump -xdS
OBJCOPY=$(CROSS)objcopy
NM=$(CROSS)nm
GDB=$(CROSS)gdb
OOCD=openocd

ifeq ($(VERBOSE),y)
Q=
else
Q=@
endif

all: $(TARGET) $(TARGET_BIN) $(TARGET_LST) $(TARGET_NM) size

-include $(foreach m, $(MODULES), $(wildcard $(m)/module.mk))

-include $(DEPS)

$(OUT)/%.o: %.c
	@echo CC $(notdir $<)
	@mkdir -p $(dir $@)
	$(Q)$(CC) -MMD $(CFLAGS) -c -o $@ $<

$(OUT)/%.o: %.s
	@echo AS $(notdir $<)
	@mkdir -p $(dir $@)
	$(Q)$(CC) -MMD $(CFLAGS) -c -o $@ $<

$(TARGET): $(OBJECTS)
	@echo LD $@...
	$(Q)$(LD) $(LDFLAGS) -o $@ $(OBJECTS)

$(TARGET_BIN): $(TARGET)
	$(Q)$(OBJCOPY) -v -O binary $< $@

$(TARGET_LST): $(TARGET)
	@echo LIST
	$(Q)$(LIST) $< > $@

$(TARGET_NM): $(TARGET)
	@echo NAME
	$(Q)$(NM) -nAsSCp $< \
		| sed -r 's/(.+?\:[^ ]+) ([a-zA-Z\?] [a-zA-Z_].*)/\1 00000000 \2/' \
		| sed -r 's/(.+?)\:([a-fA-F0-9]+) ([a-fA-F0-9]+) ([a-zA-Z\?]) (.*)/\1\t0x\2\t0x\3\t\4\t\5/' \
		> $@

size: $(TARGET)
	$(Q)$(SIZE) $<

download: $(TARGET_BIN)
	@echo DOWNLOAD
	$(Q)$(OOCD) -f $(OOCD_SCRIPT) \
		-c "init" \
		-c "halt 0" \
		-c "flash write_image erase unlock $< 0x1A000000 bin" \
		-c "reset run" \
		-c "shutdown" 2>&1

erase:
	@echo ERASE
	$(Q)$(OOCD) -f $(OOCD_SCRIPT) \
		-c "init" \
		-c "halt 0" \
		-c "flash erase_sector 0 0 last" \
		-c "shutdown" 2>&1

debug:
	@echo DEBUG
	$(Q)$(OOCD) -f $(OOCD_SCRIPT) 2>&1

clean:
	@echo CLEAN
	$(Q)rm -fR $(OBJECTS) $(TARGET) $(TARGET_BIN) $(TARGET_LST) $(DEPS) $(OUT)

.PHONY: all size download erase debug clean
