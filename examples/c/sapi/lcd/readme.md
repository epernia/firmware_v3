# Ejemplos de display LCD

### Hoja de datos

Se inincluye la hoja de datos del controlador Hitachi
HD44780 HD44780.pdf".

### Crear caracteres personalizados

https://maxpromer.github.io/LCD-Character-Creator/

### Uso de display LCD vía I2C

Para usar el display por I2C mediante el I/O expander
I2C PCF8574T, se debe añadir en el config.mk:

``` makefile
DEFINES+=LCD_HD44780_I2C_PCF8574T
```

Si no se añade el display funciona através de GPIOs.

### Tipografía

Además la fuente tipográfica "HD44780.ttf" para diseñar
pantallas.
