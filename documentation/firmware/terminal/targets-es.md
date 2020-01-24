# Ejecutar *targets* de *makefile* con la Terminal

### Abrir una terminal

- En Linux presionar `Ctrl+Alt+T` para abrir una terminal del sistema.
- En Windows abrir la terminal `busybox.exe`. 

**Nota:**  Si instaló el paquete de software provisto ([CIAA Software](https://github.com/epernia/software/)) ambas terminales están disponibles como atajos desde el *launcher*.

### Cambiar ruta (*path*) a firmware_v3

Dentro de la terminal se debe cambiar la ruta a la de la carpeta firmware_v3 mediante el comando `cd`.

Ejemplo en Linux:

```
cd myuser/CIAA/firmware_v3
```

Ejemplo en Windows:

```
cd c:/CIAA/firmware_v3
```

### Ejecutar *target* de *Makefile* de firmware_v3

Ejecutar en la terminal el *target* de Makefile deseado mediante el comando `make` de la forma:

```
make nombre_del_target
```

Por ejemplo:

```
make all
```



[Volver al README](../../readme/readme-es.md).