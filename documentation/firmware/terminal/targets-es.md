## Índice

<div>
<ul style="list-style:none;">
   <li><a style="text-decoration: none;" href="#ejecutar-targets-de-makefile-con-la-terminal">Ejecutar <em>targets</em> de <em>makefile</em> con la Terminal</a></li>
   <ul style="list-style:none;">
      <li><a style="text-decoration: none;" href="#1-abrir-una-terminal">1. Abrir una terminal</a></li>
      <li><a style="text-decoration: none;" href="#2-cambiar-ruta-path-a-firmware_v3">2. Cambiar ruta (<em>path</em>) a firmware_v3</a></li>
      <li><a style="text-decoration: none;" href="#3-ejecutar-target-de-makefile-de-firmware_v3">3. Ejecutar <em>target</em> de <em>Makefile</em> de firmware_v3</a></li>
   </ul>
</ul>
</div>

# Ejecutar *targets* de *makefile* con la Terminal

## 1. Abrir una terminal

- En Linux presionar `Ctrl+Alt+T` para abrir una terminal del sistema.
- En Windows abrir la terminal `busybox.exe`. 

**Nota:**  Si instaló el paquete de software provisto ([CIAA Software](https://github.com/epernia/software/)) ambas terminales están disponibles como atajos desde el *launcher*.

## 2. Cambiar ruta (*path*) a firmware_v3

Dentro de la terminal se debe cambiar la ruta a la de la carpeta firmware_v3 mediante el comando `cd`.

Ejemplo en Linux:

```
cd myuser/CIAA/firmware_v3
```

Ejemplo en Windows:

```
cd c:/CIAA/firmware_v3
```

## 3. Ejecutar *target* de *Makefile* de firmware_v3

Ejecutar en la terminal el *target* de Makefile deseado mediante el comando `make` de la forma:

```
make nombre_del_target
```

Por ejemplo:

```
make all
```



## Más información

[Volver al README](../readme/readme-es.md).