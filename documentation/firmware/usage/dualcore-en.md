# Dual core build and run

The workflow for dual core is make two projects, one for master M4 and other for slave M0 and select as a current project the M4 project. The M0 project is build automatically as a prerrequisite.

In the file `config.mk` from master M4 project:

```makefile
M0_APP=examples/c/app_m0
```

In the file `config.mk` from slave M0 project:

```makefile
# This configure the app to build for M0
USE_M0=y
```
