# SD card write via SPI

## Description
This example runs on the
[EDU-CIAA](http://www.proyecto-ciaa.com.ar/devwiki/doku.php?id=desarrollo:edu-ciaa:edu-ciaa-nxp)
board and shows how to write a file to an SD card using the
[FatFs module](http://elm-chan.org/fsw/ff/00index_e.html)
and the SSP API functions provided by the
[LPCOpen Platform](https://www.lpcware.com/lpcopen).

The minimum setup to communicate to an SD card via SPI interface is:
![](http://elm-chan.org/docs/mmc/gfx1/spicon.png)

Where:

`GPIO -> GPIO0    (P6_1)`

`SCLK -> SPI_SCK  (PF_4)`

`MOSI -> SPI_MOSI (P1_4)`

`MISO -> SPI_SCK  (P1_3)`

After starting the debugging session, a LED should blink when the write
operation is successfully completed.

*Note: several features (file read, timestamp, debug messages, etc.) were not
implemented just to stick to the
[KISS principle](https://en.wikipedia.org/wiki/KISS_principle).*