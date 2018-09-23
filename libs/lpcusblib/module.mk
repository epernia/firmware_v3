ifeq ($(USE_LPCUSBLIB),y)

LPCUSBLIB_BASE=libs/lpcusblib

# sgermino: necesario para resolver el mal orden (automatico) de precedencia en
# la inclusion de librerias!
DEFINES+=__LPC43XX__

# sgermino: no se incluye codigo de device classes de esta libreria. para device
# seria mejor usar USBD.
DEFINES+=USB_HOST_ONLY

SRC+=$(wildcard $(LPCUSBLIB_BASE)/Drivers/USB/Class/Common/*.c)
SRC+=$(wildcard $(LPCUSBLIB_BASE)/Drivers/USB/Class/Host/*.c)
SRC+=$(wildcard $(LPCUSBLIB_BASE)/Drivers/USB/Core/*.c)
SRC+=$(wildcard $(LPCUSBLIB_BASE)/Drivers/USB/Core/HAL/LPC18XX/*.c)
SRC+=$(wildcard $(LPCUSBLIB_BASE)/Drivers/USB/Core/HCD/*.c)
SRC+=$(wildcard $(LPCUSBLIB_BASE)/Drivers/USB/Core/HCD/EHCI/*.c)

INCLUDES+=-I$(LPCUSBLIB_BASE)
INCLUDES+=-I$(LPCUSBLIB_BASE)/Common
INCLUDES+=-I$(LPCUSBLIB_BASE)/Drivers/USB
INCLUDES+=-I$(LPCUSBLIB_BASE)/Drivers/USB/Class
INCLUDES+=-I$(LPCUSBLIB_BASE)/Drivers/USB/Class/Common
INCLUDES+=-I$(LPCUSBLIB_BASE)/Drivers/USB/Class/Host
INCLUDES+=-I$(LPCUSBLIB_BASE)/Drivers/USB/Core
INCLUDES+=-I$(LPCUSBLIB_BASE)/Drivers/USB/Core/HAL
INCLUDES+=-I$(LPCUSBLIB_BASE)/Drivers/USB/Core/HAL/LPC18XX
INCLUDES+=-I$(LPCUSBLIB_BASE)/Drivers/USB/Core/HCD
INCLUDES+=-I$(LPCUSBLIB_BASE)/Drivers/USB/Core/HCD/EHCI

endif
