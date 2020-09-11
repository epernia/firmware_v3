# Compilar y correr en dual core

El flujo de trabajo para dual core es construir dos proyectos, uno para el maestro M4 y otro para el esclavo M0 y seleccionar como proyecto actual, el proyecto del maestro M4. El proyecto del M0 es construido automaticamente como un prerequsito.

En el archivo `config.mk` del proyecto maestro M4:

```makefile
M0_APP=examples/c/app_m0
```

En el archivo `config.mk` del proyecto esclavo M0:

```makefile
# This configure the app to build for M0
USE_M0=y
```
