# Display ---------------------------------------------------------------------

DISPLAY_BASE=$(EXTERNAL_PERIPH_BASE)/display

# Los includes son paths
# Los src son archivos

INCLUDES += -I$(DISPLAY_BASE)/drivers/LED_Segments/7segment
INCLUDES += -I$(DISPLAY_BASE)/drivers/LCD/HD44780/GPIOs

INCLUDES += -I$(DISPLAY_BASE)/fonts
INCLUDES += -I$(DISPLAY_BASE)/fonts/chars_5x8px

SRC += $(wildcard $(DISPLAY_BASE)/drivers/LCD/HD44780/GPIOs/*.c)
SRC += $(wildcard $(DISPLAY_BASE)/drivers/LED_Segments/7segment/*.c)

SRC += $(wildcard $(DISPLAY_BASE)/fonts/chars_5x8px/*.c)
