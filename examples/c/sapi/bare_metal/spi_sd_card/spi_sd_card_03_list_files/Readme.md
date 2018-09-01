# SD card SPI File System

## Compilacion

Recordar compilar con "make" desde adentro esta misma carpeta.

De manera similar al "Makefile.mine" del CIAA Firmware tenemos el archivo 
"project.mk" que permite seleccionar el proyecto y placa objetivo a compilar.

## Utilizacion

Este ejemplo escribe un archivo de texto en una tarjeta SD card mediante SPI.

Funciona sobre la EDU-CIAA-NXP. El conexionado utilizando el lector de SD es:

   SD/MicroSD Card Reader --> EDU-CIAA-NXP

                        + --> +3.3V
                       CS --> GPIO0
                       DI --> SPI_MOSI
                      CLK --> SPI_SCK
                       DO --> SPI_MISO
                        G --> GND

Utiliza el modulo FatFs (http://elm-chan.org/fsw/ff/00index_e.html) y las
funciones de la API para SSP de LPCOpen (https://www.lpcware.com/lpcopen).

Se debe conectar la SD antes de correr el ejemplo, si prende el LEDG significa
que pudo grabar el archivo correctamente en la tarjeta SD. En caso contrario
prende el LEDR.

No se encuentran implementados aun: FileRead, timestamp, debug messages, etc.
