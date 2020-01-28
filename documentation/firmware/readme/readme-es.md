## Índice

<div>
<ul style="list-style:none;">
   <li><a style="text-decoration: none;" href="#framework-para-desarrollo-de-firmware-de-sistemas-embebidos-en-cc">Framework para desarrollo de Firmware de Sistemas Embebidos en C/C++3</a></li>
   <ul style="list-style:none;">
      <li><a style="text-decoration: none;" href="#importante">IMPORTANTE</a></li>
      <li><a style="text-decoration: none;" href="#acerca-de-firmware_v3">Acerca de firmware_v3</a></li>
      <li><a style="text-decoration: none;" href="#plataformas-de-hardware-soportadas">Plataformas de hardware soportadas</a></li>
      <li><a style="text-decoration: none;" href="#ejemplos">Ejemplos</a></li>
      <li><a style="text-decoration: none;" href="#toolchains-soportados">Toolchains soportados</a></li>
      <li><a style="text-decoration: none;" href="#bibliotecas-disponibles">Bibliotecas disponibles</a></li>
      <li><a style="text-decoration: none;" href="#utilización-de-firmware_v3">Utilización de firmware_v3</a></li>
   </ul>
</ul>
</div>

# Framework para desarrollo de Firmware de Sistemas Embebidos en C/C++

## IMPORTANTE

**¡Este entorno está en construcción!**

**Utilice siempre las versiones [release](../../../../../releases) debido a que todos los ejemplos están testeados sobre las plataformas de hardware y la documentación de la API es consistente. La rama master puede contener inconsistencias debido a que este entorno esta continuamente en desarrollo. Vea el estado de Travis CI en la etiqueta de arriba para conocer el estado actual del repositorio.**

## Acerca de firmware_v3

**firmware_v3** es un proyecto en base a *makefile* que actúa como framework para el desarrollo de *firm*ware para sistemas embebidos en lenguajes C/C++. Soporta *toolchain*, bibliotecas y ejemplos para múltiples plataformas de *hardware*.

## Plataformas de hardware soportadas

- [CIAA-NXP (LPC4337)](../../CIAA_Boards/NXP_LPC4337/CIAA-NXP/CIAA-NXP%20v1.0%20Board%20-%202019-01-04%20v3r0.pdf).
- [EDU-CIAA-NXP (LPC4337)](../../CIAA_Boards/NXP_LPC4337/EDU-CIAA-NXP/EDU-CIAA-NXP%20v1.1%20Board%20-%202019-01-03%20v5r0.pdf).

## Ejemplos

[Ver ejemplos](../examples/examples-es.md).

## Toolchains soportados

- gcc-arm-none-eabi 

## Bibliotecas disponibles

- CMSIS 5.4.0 (Core y DSP). [Repositorio de CMSIS_5](https://github.com/ARM-software/CMSIS_5) (en inglés).
- LPCOpen v3.02 [Sitio web de NXP de la plataforma de desarrollo de software LPCOpen para LPC43XX NXP](https://www.nxp.com/design/microcontrollers-developer-resources/lpcopen-libraries-and-examples/lpcopen-software-development-platform-lpc43xx:LPCOPEN-SOFTWARE-FOR-LPC43XX) (en inglés).
- LPCUSBlib. [Documnetación de LPCUSBlib](http://67.222.144.123/lpcopen/v1.03/group___l_p_c_u_s_blib.html) (en inglés).
- sAPI v0.5.2. [Referencia de la API de la biblioteca sAPI](../../../libs/sapi/documentation/api_reference_es.md).
- Arduino (wiring). [Guía de Referencia de Arduino](https://www.arduino.cc/reference/es/).
- Elm-Chan FatFS R0.13b. [FatFS documentation](http://elm-chan.org/fsw/ff/00index_e.html) (en inglés). Módulo de sistema de archivos FAT genérico con soporte para los discos: tarjeta SD (SSP) y Pendrive (USB MSD).
- FreeRTOS Kernel V10.0.1. [FreeRTOS_Reference_Manual_V10.0.0.pdf](../../../examples/c/freertos_book/FreeRTOS_Reference_Manual_V10.0.0.pdf) (en inglés).
- RKH 3.2.3 [RKH Reference Manual](https://vortexmakes.com/rkh/) (en inglés).

## Utilización de firmware_v3

- [Utilización básica de firmware_v3](../usage/usage-es.md).
- [Ejecutar *targets* de *makefile* con la Terminal](../terminal/targets-es.md).
- [Utilización de firmware_v3 con Embedded IDE](../embedded_ide/usage-es.md).
  
- [Utilización de firmware_v3 con Eclipse](../eclipse/usage-es.md).



## Más información

[Volver al README principal](../../../README.md).