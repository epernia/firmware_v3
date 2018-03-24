# Bootloader example
This example is intended to be used with the EDU-CIAA-NXP board. 

## Tutorial
The bootloader waits for an image (.bin file) that will be copied to a RAM section. The first 4 bytes of the file must be the image size.

1. Open a terminal application like GtkTerm.
1. Open USB Serial Port from EDU-CIAA (typically /dev/ttyUSB1)
1. Set variables in ```project.mk```:
```
PROJECT = examples/boot
TARGET = lpc4337_m4
BOARD = edu_ciaa_nxp
```
1. ```make```
1. ```make download```
1. See Terminal app. You should see this message: ```Waiting for binary. First 4 bytes must be image length.```
1. Set variables in ```project.mk```:
```
PROJECT = examples/blinky_ram
TARGET = lpc4337_m4
BOARD = edu_ciaa_nxp
```
1. ```make```
1. You should have a file called ```blinky_ram_image.bin``` in ```out/lpc4337_m4/```. Use "Send Raw File" option in GtkTerm to send the file to the board. Then the bootloader will do the rest :)
1. You should see this message: ```Image received. Booting...```. Then a LED should be blinking.

