## Table of contents

<div>
<ul style="list-style:none;">
   <li><a style="text-decoration: none;" href="#framework-for-embedded-systems-firmware-development-in-cc">Framework for Embedded Systems Firmware development in C/C++</a></li>
   <ul style="list-style:none;">
      <li><a style="text-decoration: none;" href="#important">IMPORTANT</a></li>
      <li><a style="text-decoration: none;" href="#about-firmware_v3">About firmware_v3</a></li>
      <li><a style="text-decoration: none;" href="#supported-boards">Supported boards</a></li>
      <li><a style="text-decoration: none;" href="#examples">Examples</a></li>
      <li><a style="text-decoration: none;" href="#supported-toolchains">Supported toolchains</a></li>
      <li><a style="text-decoration: none;" href="#available-libraries">Available libraries</a></li>
      <li><a style="text-decoration: none;" href="#usage">Usage</a></li>
   </ul>
</ul>
</div>

# Framework for Embedded Systems Firmware development in C/C++

## IMPORTANT

**This environment is under construction!!**

**Always use the [released versions](../../../../../releases) because in these all examples are tested and the API documentation is consistent. The master branch may contain inconsistencies because this environment is currently under development. See the Travis CI tag above to know the status of this repository.**

## About firmware_v3

**firmware_v3** is a makefile-based project that act as a framework for Embedded Systems firmware development in C/C++ language. It support toolchain, libraries and examples for several platforms.

## Supported boards

- [CIAA-NXP (LPC4337)](../../CIAA_Boards/NXP_LPC4337/CIAA-NXP/CIAA-NXP%20v1.0%20Board%20-%202019-01-04%20v3r0.pdf).
- [EDU-CIAA-NXP (LPC4337)](../../CIAA_Boards/NXP_LPC4337/EDU-CIAA-NXP/EDU-CIAA-NXP%20v1.1%20Board%20-%202019-01-03%20v5r0.pdf).

## Examples

[See examples](../examples/examples-en.md).

## Supported toolchains

- gcc-arm-none-eabi 

## Available libraries

- CMSIS 5.4.0 (Core and DSP). [CMSIS_5 Repository](https://github.com/ARM-software/CMSIS_5).
- LPCOpen v3.02 [LPCOpen Software Development Platform LPC43XX NXP website](https://www.nxp.com/design/microcontrollers-developer-resources/lpcopen-libraries-and-examples/lpcopen-software-development-platform-lpc43xx:LPCOPEN-SOFTWARE-FOR-LPC43XX).
- LPCUSBlib. [LPCUSBlib documentation](http://67.222.144.123/lpcopen/v1.03/group___l_p_c_u_s_blib.html)
- sAPI v0.5.2. [sAPI API Reference (spanish)](../../../libs/sapi/documentation/api_reference_es.md).
- Arduino (wiring). [Language Reference](https://www.arduino.cc/reference/en/).
- Elm-Chan FatFS R0.13b. [FatFS documentation](http://elm-chan.org/fsw/ff/00index_e.html). Generic FAT Filesystem module with support for SD Card (SSP) and Pendrive (USB MSD) disks.
- FreeRTOS Kernel V10.0.1. [FreeRTOS_Reference_Manual_V10.0.0.pdf](../../../examples/c/freertos_book/FreeRTOS_Reference_Manual_V10.0.0.pdf).
- RKH 3.2.3 [RKH Reference Manual](https://vortexmakes.com/rkh/).

## Usage

[firmware_v3 usage](../usage/usage-en.md).



## More information

[Back to main README](../../../README.md).