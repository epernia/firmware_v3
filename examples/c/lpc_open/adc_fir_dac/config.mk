# Compile options ---------------------------------------------
VERBOSE=n
OPT=g
USE_NANO=n
SEMIHOST=n
USE_FPU=y

# Libraries ---------------------------------------------------

USE_LPCOPEN=y

# Extra defines -----------------------------------------------

# Comentar con # para LPC1769
DEFINES+=lpc4337_m4

# Elegir uno de los 2 filtros
DEFINES+=FILTRO_PASA_BANDA
#DEFINES+=FILTRO_PASA_BAJOS

# Elgir usar cálculo en C o en asembler
#DEFINES+=USAR_FUNCIONES_ASSEMBLER
