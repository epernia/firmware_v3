# API de la biblioteca sAPI

## MÛdulos

### DataTypes

Define las siguientes constantes:

Estados lÛgicos

- ``FALSE = 0``
- ``TRUE  = !FALSE``

Estados funcionales

- ``ON  = 1``
- ``OFF = 0``

Estados elÈctricos

- ``HIGH = 1``
- ``LOW  = 0``

Estados de habilitaciÛn

- ``ENABLE  = 1``
- ``DISABLE = 0``

Adem·s define los tipos de datos:

- **Booleano** ``bool_t``
- **Enteros sin signo** ``uint8_t, uint16_t, uint32_t, uint64_t``
- **Enteros con signo** ``int8_t, int16_t, int32_t, int64_t``
- **Flotantes** ``float32_t, float64_t``

El tipo de datos para el conteo de tiempo en la unidad Tick

```c
typedef uint64_t tick_t;
```

Un tipo de datos para puntero a funciÛn:

```c
typedef bool_t (*sapiFuncPtr_t)(void *);
```
- Par·metro: ``void *`` Para poder pasar cualquier argumento.
- Retorna: ``bool_t`` Para reportar errores (TRUE si todo est· bien).

Utilizando este tipo de datos define la finciÛn Nula que no hace nada y Retorna
siempre TRUE, esta se utiliza para evitar errores de NULL POINTER.

```c
bool_t sapiNullFuncPtr(void *);
```
- Par·metro: ``void *`` No usado.
- Retorna: ``bool_t``Retorna siempre TRUE.


### Peripheral Map

Contiene el mapa de perifÈricos.

**GPIO Map**

EDU-CIAA-NXP:

- P1 header

    ``T_FIL0, T_FIL1, T_FIL2, T_FIL3,``
    ``T_COL0,  T_COL1, T_COL2,``
    ``CAN_TD, CAN_RD,``
    ``RS232_TXD, RS232_RXD``

    ``GPIO25, GPIO26, GPIO27, GPIO28,``
    ``GPIO29, GPIO30, GPIO31,``
    ``GPIO32, GPIO33,``
    ``GPIO34, GPIO35``

- P2 header

    ``GPIO0, GPIO1, GPIO2, GPIO3, GPIO4, GPIO5, GPIO6, GPIO7, GPIO8``
    ``LCD1, LCD2, LCD3, LCD4, LCDEN, LCDRS,``
    ``SPI_MISO, SPI_MOSI,``
    ``ENET_TXD0, ENET_TXD1, ENET_TXEN, ENET_RXD0, ENET_RXD1, ENET_MDIO, ENET_CRS_DV, ENET_MDC,``

    ``GPIO0, GPIO1, GPIO2, GPIO3, GPIO4, GPIO5, GPIO6, GPIO7, GPIO8``
    ``GPIO9, GPIO10, GPIO11, GPIO12, GPIO13, GPIO14,``
    ``GPIO15, GPIO16,``
    ``GPIO17, GPIO18, GPIO19, GPIO20, GPIO21, GPIO22, GPIO23, GPIO24,``

- Switches

    ``TEC1,  TEC2,  TEC3,  TEC4``

- Leds

    ``LED1,  LED2,  LED3,  LEDR,  LEDG,  LEDB``


CIAA-NXP:
    ``DI0,   DI1,   DI2,   DI3,   DI4,   DI5,   DI6,   DI7,``
    ``DO0,   DO1,   DO2,   DO3,   DO4,   DO5,   DO6,   DO7``

**ADC Map**

EDU-CIAA-NXP: ``CH1, CH2, CH3``

EDU-CIAA-NXP: ``ADC0CH0, ADC0CH1, ADC0CH2``

CIAA-NXP: ``AI0, AI1, AI2, AI3``

**DAC Map**

EDU-CIAA-NXP: ``DAC``

CIAA-NXP: ``AO``

**Uart Map**

``UART_USB, UART_232, UART_485``
``UART0, UART1, UART2``

**Timer Map**

``TIMER0, TIMER1, TIMER2, TIMER3, TIMER4``

**Pwm Map**

``PWM0, PWM1, PWM2, PWM3, PWM4, PWM5, PWM6, PWM7, PWM8, PWM9, PWM10``

**Servo Map**

``SERVO0, SERVO1, SERVO2, SERVO3, SERVO4, SERVO5, SERVO6, SERVO7, SERVO8``

**I2C Map**

``I2C0``



### GPIO

Manejo de Entradas y Salidas (booleanas) de propÛsito general.

**ConfiguraciÛn inicial y modo de una entrada o salida**

```c
bool_t gpioInit( gpioMap_t pin, gpioConfig_t config );
```
- Par·metro: ``gpioMap_t pin`` pin a configurar (ver GPIO Map).
- Par·metro: ``gpioConfig_t config`` configuraciÛn.
- Retorna: ``bool_t`` TRUE si la configuraciÛn es correcta.

Posibles configuraciones:

- ``GPIO_ENABLE`` Habilita las entradas y salidas de porpÛsito general.
- ``GPIO_INPUT, GPIO_INPUT_PULLUP, GPIO_INPUT_PULLDOWN, GPIO_INPUT_PULLUP_PULLDOWN`` Pin configurado como entrada en sus distintas variantes.
- ``GPIO_OUTPUT`` Pin configurado como salida.

**Lectura de Entrada**

```c
bool_t gpioRead( gpioMap_t pin );
```
- Par·metro: ``gpioMap_t pin`` pin a leer (ver GPIO Map).
- Retorna: ``bool_t`` valor de la entrada.


**Escritura de Salida**

```c
bool_t gpioWrite( gpioMap_t pin, bool_t value );
```
- Par·metro: ``gpioMap_t pin`` pin a escribir (ver GPIO Map).
- Par·metro: ``bool_t value`` valor a escribir en el pin.
- Retorna: ``bool_t`` FALSE en caso de errores.


### Tick

**ConfiguraciÛn de interrupciÛn periÛdica**

```c
bool_t tickInit(tick_t tickRateMSvalue, sAPI_FuncPtr_t tickHook );
```
- Par·metro: ``tick_t tickRateMSvalue`` cada cuantos ms ocurre un tick.
- Par·metro: ``sAPI_FuncPtr_t tickHook`` funciÛn a ejecutar en cada tick.
- Retorna: ``bool_t`` TRUE en en caso correcto o FALSE en caso de errores.

Configura una interrupciÛn periÛdica de temporizador cada tickRateMSvalue
milisegundos para utilizar de base de tiempo del sistema. Una vez ejecutada
esta funciÛn se dice que ocurre un tick del sistema cada tickRateMSvalue
milisegundos.

La tasa de ticks en ms, tickRateMS, es un par·metro con rango de 1 a 50 ms.

Adem·s de aumentar el conteo de ticks en cada interrupciÛn, la funciÛn
tickConfig ejecuta la funciÛn pasada como par·mero cada vez que ocurre un tick.
Si no se desea ejecutar ninguna funciÛn debe poner en cero este par·metro.

**Leer la variable del conteo actual de ticks**

```c
tick_t tickRead( void );
```
- Par·metro: ``void`` sin par·metro.
- Retorna: ``tick_t`` el valor actual del contador de ticks.

La variable del conteo actual de ticks se incrementa en 1 cada tickRateMSvalue
milisegundos.

**Escribir la variable del conteo actual de ticks**

```c
void tickWrite( tick_t ticks );
```
- Par·metro: ``tick_t ticks`` el nuevo valor a setear del contador de ticks.
- Retorna: ``void``

Se utiliza si se necesita cambiar el valor del contador de ticks, por ejemplo,
para resetearlo.

En la implementaciÛn para la CIAA utiliza internamente el peifÈrico temporizador
Systick para configurar una interrupciÛn periÛdica.


### Delay

Para utilizar los retardos (con excepciÛn del retardo inexacto) se debe
configurar el Tick ya que utiliza estas interrupciones como base de tiempo.

Todos los tiempos de par·metros est·n en milisegundos.

Define la constante ``#define INACCURATE_TO_MS 20400`` y contiene las funciones:

**Retardos inexactos bloqueantes**

Utilizan un bloque for bloqueante que tiene una constante calculada para perder muchos ciclos de reloj y lograr hacer un retado.

```c
void delayInaccurateMs( tick_t delay_ms );
```
- Par·metros: ``tick_t delay_ms`` tiempo de duraciÛn del retardo en ms.
- Retorna: ``void``

```c
void delayInaccurateUs( tick_t delay_us );
```
- Par·metros: ``tick_t delay_us`` tiempo de duraciÛn del retardo en us.
- Retorna: ``void``

```c
void delayInaccurateNs( tick_t delay_ns );
```
- Par·metros: ``tick_t delay_ns`` tiempo de duraciÛn del retardo en ns (debe ser m˙ltiplo de 5 en la CIAA-NXP o EDU-CIAA-NXP).
- Retorna: ``void``

**Retardo bloqueante**
```c
void delay ( tick_t time );
```
- Par·metros: ``tick_t time``
- Retorna: ``void``

Utiliza el conteo de ticks para determinar el tiempo transcurrido resultando en
un retardo exacto. Es bloqueante pues se queda en un bucle while hasta que se
cuentan los ticks necesarios para lograr el tiempo especificado.

**Retardo no bloqueante**

Este tipo de retardo permite realizar otras tareas mientras se ejecuta ya que
simplemente se chequea si el tiempo de retardo se ha arribado en lugar de
quedarse bloqueado esperando a que se complete el tiempo como en los casos
anteriores.

Define el tipo de datos estructurado ``delay_t``

Contiene las funciones:

```c
void delayInit( delay_t* delay, tick_t duration );
```
- Par·metro: ``delay_t * delay`` direcciÛn de memoria de una variable del tipo delay_t.
- Par·metro: ``tick_t duration`` tiempo de duraciÛn del retardo en ms.
- Retorna: ``void``

```c
bool_t delayRead( delay_t* delay );
```
- Par·metro: ``delay_t* delay`` direcciÛn de memoria de una variable del tipo delay_t.
- Retorna: ``bool_t`` TRUE cuando el delay se cumpliÛ, FALSE en caso contrario.

```c
void delayWrite( delay_t* delay, tick_t duration );
```
- Par·metro: ``delay_t* delay`` direcciÛn de memoria de una variable del tipo delay_t.
- Par·metro: ``tick_t duration`` tiempo de duraciÛn del retardo en ms.
- Retorna: ``void``


Uso:

Se utiliza declarando una variable de estructura del tipo delay_t, por ejemplo:

```c
delay_t myDelay;
```

Luego, se configura inicialmente pasando como par·metro la variable reciÈn
declarada

```c
delayInit( &myDelay, 500 );
```

Se detecta con un bloque if si se cumpliÛ el tiempo del delay leyÈndolo con

```c
if( delayRead( &myDelay ) ) {
   // Si se cumplio el tiempo hacer algo
}
```

La primera vez que se ejecuta delayRead activa el mismo. delayRead devuelve TRUE
cuando se completo y se vuelve a relanzar autom·ticamente.

Con ``delayWrite( &myDelay, 1000 );`` se puede cambiar la duraciÛn de un delay
en tiempo de ejecuciÛn.



### ADC

Manejo de conversor analÛgico-digital.

**ConfiguraciÛn inicial de conversor analÛgico-digital**

```c
void adcInit( adcConfig_t config );
```
- Par·metro: ``adcConfig_t config`` configuraciÛn.
- Retorna: ``void``.

Posibles configuraciones:

- ``ADC_ENABLE`` Habilita el perifÈrico ADC.
- ``ADC_DISABLE`` Deshabilita el perifÈrico ADC.

**Lectura de Entrada analÛgica**

```c
uint16_t adcRead( adcMap_t analogInput );
```
- Par·metro: ``adcMap_t analogInput`` pin a leer (ver ADC Map).
- Retorna: ``uint16_t`` el valor actual de la entrada analÛgica.


### DAC

Manejo de conversor digital-analÛgico.

**ConfiguraciÛn inicial de conversor digital-analÛgico**

```c
void dacInit( dacConfig_t config );
```
- Par·metro: ``dacConfig_t config`` configuraciÛn.
- Retorna: ``void``.

Posibles configuraciones:

- ``DAC_ENABLE`` Habilita el perifÈrico DAC.
- ``DAC_DISABLE`` Deshabilita el perifÈrico DAC.

**Escritura de Salida analÛgica**

```c
void dacWrite( dacMap_t analogOutput, uint16_t value );
```
- Par·metro: ``dacMap_t analogOutput`` pin a escribir (ver DAC Map).
- Par·metro: ``uint16_t value`` valor del pin a escribir.
- Retorna: ``void``.


### UART

Manejo del perifÈrico de comunicaciÛn UART (puerto serie asincrÛnico).

**ConfiguraciÛn**

```c
void uartInit( uartMap_t uart, uint32_t baudRate )
```
- Par·metro: ``uartMap_t uart`` UART a configurar (ver UART MAP).
- Par·metro: ``uint32_t baudRate`` tasa de  bits.
- Retorna: ``void``.

Posibles configuraciones de baudRate: ``9600, 57600, 115200, etc.``

**Recibir Byte**

```c
bool_t uartReadByte( uartMap_t uart, uint8_t* receivedByte );
```
- Par·metro: ``uartMap_t uart`` UART a configurar (ver UART Map).
- Par·metro: ``uint8_t* byte`` DirecciÛn de memoria a donde se escribir· el dato recibido en caso de que haya un dato para recibir.
- Retorna: ``bool_t`` TRUE si recibiÛ un dato, FALSE en caso contrario.

**Enviar Byte**

```c
void uartWriteByte( uartMap_t uart, uint8_t byte );
```
- Par·metro: ``uartMap_t uart`` UART a configurar (ver UART Map).
- Par·metro: ``uint8_t byte`` Byte a enviar.
- Retorna: ``void``.

**Enviar String**

```c
void uartWriteString( uartMap_t uart, char* str );
```
- Par·metro: ``uartMap_t uart`` UART a configurar (ver UART Map).
- Par·metro: ``char* str`` String a enviar, puede ser un literal, por ejemplo "hola", o un vector de uint8_t terminado en 0 o '\0' (caracter NULL).
- Retorna: ``void``.


### I2C

Manejo del perifÈrico bus comunicaciÛn I2C (Inter Integrated Circuits).

**ConfiguraciÛn**

```c
bool_t i2cInit( i2cMap_t i2cNumber, uint32_t clockRateHz );
```
- Par·metro: ``i2cMap_t i2cNumber`` I2C a configurar (ver I2C Map).
- Par·metro: ``uint32_t clockRateHz`` configuraciÛn de velocidad del bus I2C.
- Retorna: ``bool_t`` TRUE si la configuraciÛn es correcta.

Posibles configuraciones de clockRateHz: 100000, etc.

**Lectura**

```c
bool_t i2cRead( i2cMap_t  i2cNumber,
                uint8_t  i2cSlaveAddress,
                uint8_t* dataToReadBuffer,
                uint16_t dataToReadBufferSize,
                bool_t   sendWriteStop,
                uint8_t* receiveDataBuffer,
                uint16_t receiveDataBufferSize,
                bool_t   sendReadStop );
```
- Par·metro: ``i2cMap_t i2cNumber`` I2C a leer (ver I2C Map).
- Par·metro: ``uint8_t i2cSlaveAddress`` DirecciÛn del sensor conectado por I2C a leer.
- Par·metro: ``uint8_t* dataToReadBuffer`` puntero al buffer con los bytes a escribir para indicar que se debe leer.
- Par·metro: ``uint16_t dataToReadBufferSize`` tamaÒo del buffer con los bytes a escribir.
- Par·metro: ``bool_t sendWriteStop`` setear en 1 para enviar stop al finalizar el comando de escritura, con 0 no se envÌ≠a. Algunos perifÈricos pueden no necesitar el stop.
- Par·metro: ``uint8_t* receiveDataBuffer`` puntero al buffer donde se almacenar·n los datos leÌ≠dos.
- Par·metro: ``uint16_t receiveDataBufferSize`` tamaÒo del buffer donde se almacenar·n los datos leÌ≠dos.
- Par·metro: ``bool_t sendReadStop`` setear en 1 para enviar stop al finalizar el comando de lectura, con 0 no se envÌ≠a. Algunos perifÈricos pueden no necesitar el stop.
- Retorna: ``bool_t`` TRUE si se pudo leer correctamente.

**Escritura**

```c
bool_t i2cWrite( i2cMap_t  i2cNumber,
                 uint8_t  i2cSlaveAddress,
                 uint8_t* transmitDataBuffer,
                 uint16_t transmitDataBufferSize,
                 bool_t   sendWriteStop );
```
- Par·metro: ``i2cMap_t i2cNumber`` ID de perifÈrico I2C a escribir (ver I2C Map). Actualmente funciona Ì∫nicamente el I2C0.
- Par·metro: ``uint8_t i2cSlaveAddress`` DirecciÛn del sensor conectado por I2C a escribir.
- Par·metro: ``uint8_t* transmitDataBuffer`` puntero al buffer donde se encuentran los datos a escribir.
- Par·metro: ``uint16_t transmitDataBufferSize`` tamaÒo del buffer donde se encuentran los datos a escribir.
- Par·metro: ``bool_t sendWriteStop`` setear en 1 para enviar stop al finalizar el comando de escritura, con 0 no se envÌ≠a. Algunos perifÈricos pueden no necesitar el stop.
- Retorna: ``bool_t`` TRUE si se pudo escribir correctamente.

### Sleep

Manejo de modos de bajo consumo del microcontrolador.

**Dormir hasta que ocurra la prÛxima interrupciÛn**

```c
void sleepUntilNextInterrupt( void );
```
- Par·metro: ninguno.
- Retorna: nada.


### RTC

Manejo del perifÈrico RTC (reloj de tiempo real).

**ConfiguraciÛn**

```c
bool_t rtcInit( void );
```
- Retorna: ``bool_t`` TRUE si la configuraciÛn es correcta.

**Establecer la fecha y hora**

```c
bool_t rtcWrite( rtc_t* rtc );
```
- Par·metro: ``rtc_t* rtc`` Puntero a estructura del tipo rtc_t con la fecha y hora a establecer.
- Retorna: ``bool_t`` TRUE.

La estructura del tipo ``rtc_t`` contiene los par·metros:

- ``uint16_t year`` aÒo, con valores desde 1 a 4095.
- ``uint8_t month`` mes, con valores desde 1 a 12.
- ``uint8_t mday`` dÌ≠a, con valores desde 1 a 31.
- ``uint8_t wday`` dÌ≠a de la semana, con valores desde 1 a 7.
- ``uint8_t hour`` horas, con valores desde 0 a 23.
- ``uint8_t min`` minutos, con valores desde 0 a 59.
- ``uint8_t sec`` segundos, con valores desde 0 a 59.

**Lectura de fecha y hora**

```c
bool_t rtcRead( rtc_t* rtc );
```
- Par·metro: ``rtc_t * rtc`` Puntero a estructura del tipo RTC_t donde se guarda la fecha y hora.
- Retorna: ``bool_t`` TRUE.

### PWM

Manejo de salidas PWM (modulaciÛn por ancho de pulso). 

En la EDU-CIAA-NXP se utiliza internamente el perifÈrico SCT para generar los PWM.

**ConfiguraciÛn**

```c
bool_t pwmInit( pwmMap_t pwmNumber, pwmConfig_t config);
```
- Par·metro: ``pwmMap_t pwmNumber`` pin a configurar como salida PWM (ver PWM Map).
- Par·metro: ``uint8_t config`` configuraciÛn.
- Retorna: ``bool_t`` TRUE si la configuraciÛn es correcta.

Posibles configuraciones:

- ``PWM_ENABLE`` habilita el o los Timers en modo PWM.
- ``PWM_DISABLES`` deshabilita el o los Timers en modo PWM.
- ``PWM_ENABLE_OUTPUT`` habilita la salida PWM particular.
- ``PWM_DISABLE_OUTPUT`` deshabilita la salida PWM particular.

Ejemplo:

```c
// Configurar el (o los) Timer(s) para generar las seÒales PWM
pwmInit( 0, PWM_ENABLE );    
        
// Configura el pin como salida de seÒal PWM 
// Se debe realizar para cada salida PWM a activar en particular
pwmInit( PWM7 , PWM_ENABLE_OUTPUT ); 
```

**Lectura del ciclo de trabajo (duty cycle) de la salida PWM**

```c
uint8_t pwmRead( pwmMap_t pwmNumber );
```
- Par·metro: ``pwmMap_t pwmNumber`` salida PWM a leer el ciclo de trabajo.
- Retorna: ``uint8_t`` el ciclo de trabajo de la salida PWM.

 **Establecer el ciclo de trabajo de la salida PWM**

```c
bool_t pwmWrite( pwmMap_t pwmNumber, uint8_t percent );
```
- Par·metro: ``pwmMap_t pwmNumber`` salida PWM a leer el ciclo de trabajo.
- Par·metro: ``uint8_t percent`` valor de ciclo de trabajo a setear en la salida PWM.
- Retorna: ``bool_t`` TRUE.


### Servo

Manejo de salidas para Servomortores angulares (usan modulaciÛn por ancho de pulso). En la EDU-CIAA-NXP se utilizan internamente los perifÈricos TIMER para
generar estas salidas.

**ConfiguraciÛn**

```c
bool_t servoInit( servoMap_t servoNumber, servoConfig_t config );
```
- Par·metro: ``servoMap_t servoNumber`` pin a configurar como salida Servo (ver Servo Map).
- Par·metro: ``servoConfig_t config`` configuraciÛn.
- Retorna: ``bool_t`` TRUE si la configuraciÛn es correcta.

Posibles configuraciones:

- ``SERVO_ENABLE`` habilita el o los Timers en modo PWM para Servo.
- ``SERVO_DISABLE`` deshabilita el o los Timers en modo PWM para Servo.
- ``SERVO_ENABLE_OUTPUT`` habilita la salida PWM particular.
- ``SERVO_DISABLE_OUTPUT`` deshabilita la salida PWM particular.

Ejemplo:

```c
// Configurar el (o los) Timer(s) para generar las seÒales de control de Servomotor
servoInit( 0, PWM_ENABLE );    
        
// Configura el pin como salida de seÒal de control de Servomotor
// Se debe realizar para cada salida Servo a activar en particular
servoInit( SERVO0 , SERVO_ENABLE_OUTPUT ); 
```

**Lectura del valor angular actual de la salida Servo**

```c
uint16_t servoRead( servoMap_t servoNumber);
```
- Par·metro: ``servoMap_t servoNumber`` pin como salida Servo a leer.
- Retorna: ``uint16_t`` el valor angular actual de la salida Servo (de 0 a 180¬∞).

**Establecer el valor angular de la salida Servo**

```c
bool_t servoWrite( servoMap_t servoNumber, uint16_t angle );
```
- Par·metro: ``servoMap_t servoNumber`` pin como salida Servo a escribir.
- Par·metro: ``uint16_t angle`` valor angular a establecer en la salida Servo (de 0 a 180¬∞).
- Retorna: ``bool_t`` TRUE.


### Display 7 segmentos

**ConfiguraciÛn**

Configurar los pines conectados al display 7 segmentos como salida:

```c
void display7SegmentPinInit( gpioMap_t* display7SegmentPins );
```
- Par·metro: ``gpioMap_t* display7SegmentPins`` puntero a un vector con los 8 pines conectados al display 7 segmentos, deben estar en el orden *a* a *h*.
- Retorna: nada.

![Imagen "7-segment-display.png" no encontrada](assets/img/7-segment-display.png "Display 7 segmentos")

Ejemplo:

```c
uint8_t display7Segment[8] = {
   GPIO5, // Segment 'a'
   GPIO7, // Segment 'b'
   GPIO6, // Segment 'c'
   GPIO1, // Segment 'd'
   GPIO2, // Segment 'e'
   GPIO4, // Segment 'f'
   GPIO3, // Segment 'g'
   GPIO8  // Segment 'h' or 'dp'
 };
```

Testear en que orden est·n conectados los pines a los segmentos *a* a *h*:

```c
void display7SegmentTestPins( gpioMap_t* display7SegmentPins, gpioMap_t pin );
```
- Par·metro: ``gpioMap_t* display7SegmentPins`` puntero a un vector con los 8 pines conectados al display 7 segmentos, deben estar en el orden *a* a *h*.
- Par·metro: ``gpioMap_t pin`` un pin aparte que indica cuando escribe el segmento *a* para iniciar la secuencia de la *a* a *h*. Por ejemplo, LEDB.
- Retorna: nada.

Mediante esta funciÛn se puede completar la siguiente tabla que indica a quÈ pines se conecto el display 7 segmentos:

| Segmento encendido     | Valor Binario | Valor Hexadecimal | Pin de salida |
|:----------------------:|:-------------:|:-----------------:|:-------------:|
| Segmento 'a' encendido | 0b00000001    |   0x20            | (a completar) |
| Segmento 'b' encendido | 0b00000010    |   0x80            |               |
| Segmento 'c' encendido | 0b00000100    |   0x40            |               |
| Segmento 'd' encendido | 0b00001000    |   0x02            |               |
| Segmento 'e' encendido | 0b00010000    |   0x04            |               |
| Segmento 'f' encendido | 0b00100000    |   0x10            |               |
| Segmento 'g' encendido | 0b01000000    |   0x08            |               |
| Segmento 'h' encendido | 0b10000000    |   0x80            |               |


**Escribir sÌ≠mbolo en display 7 segmentos**

Escribe un sÌ≠mbolo en el display 7 segmentos.

```c
void display7SegmentWrite( gpioMap_t* display7SegmentPins, uint8_t symbolIndex );
```
- Par·metro: ``gpioMap_t* display7SegmentPins`` puntero a un vector con los 8 pines conectados al display 7 segmentos, deben estar en el orden *a* a *h*.
- Par·metro: ``uint8_t symbolIndex`` Ì≠ndice del valor a mostrar de la tabla de sÌ≠mbolos.
- Retorna: nada.

Tabla de sÌ≠mbolos:

```c
 0, 1, 2, 3, 4, 5, 6, 7, 8, 9,
 a, b, c, d, e, f,
 A, C, E, H, J, L, P, U
 ., DISPLAY_OFF
```

### Teclado matricial

**ConfiguraciÛn**

```c
bool_t keypadInit( keypad_t* keypad,
                     gpioMap_t* keypadRowPins, uint8_t keypadRowSize,
                     gpioMap_t* keypadColPins, uint8_t keypadColSize );
```
- Par·metro: ``keypad_t* keypad`` puntero a estructura del teclado matricial donde se almacenan todos los par·metros necesarios del mismo.
- Par·metro: ``gpioMap_t* keypadRowPins`` puntero a vector que contiene la lista de pines conectados a las filas del teclado matricial.
- Par·metro: ``uint8_t keypadRowSize`` cantidad de filas del teclado matricial.
- Par·metro: ``gpioMap_t* keypadColPins`` puntero a vector que contiene la lista de pines conectados a las columnas del teclado matricial.
- Par·metro: ``uint8_t keypadColSize`` cantidad de columnas del teclado matricial.
- Retorna: ``bool_t`` TRUE si los par·metros de configuraciÛn son v·lidos.

![Imagen "keypad.png" no encontrada](assets/img/keypad.png "Teclado matricial 4x4")

Ejemplo:

- Variable del teclado

```c
keypad_t keypad;
```

- Filas a configuar como Salidas

```c
uint8_t keypadRowPins1[4] = {
   RS232_TXD, // Row 0
   CAN_RD,    // Row 1
   CAN_TD,    // Row 2
   T_COL1     // Row 3
};
```

- Columnas a configurar como Entradas con pull-up (MODO = GPIO_INPUT_PULLUP)

```c
uint8_t keypadColPins1[4] = {
   T_FIL0,    // Column 0
   T_FIL3,    // Column 1
   T_FIL2,    // Column 2
   T_COL0     // Column 3
};
```

- FunciÛn de inicializaciÛn

```c
keypadInit( &keypad, keypadRowPins1, 4, keypadColPins1, 4 );
```

**Lectura de teclado matricial**

```c
bool_t keypadRead( keypad_t* keypad, uint16_t* key );
```
- Par·metro: ``keypad_t* keypad`` puntero a estructura del teclado matricial.
- Par·metro: ``uint16_t* key`` puntero a variable donde escribe la tecla leida del teclado matricial.
- Retorna: ``bool_t`` TRUE si hay tecla leida o FALSE en caso contrario.


### sAPI HMC5883L

Manejo del sensor magnetÛmetro vectorial (x,y,z) HMC5883L de Honeywell.
Este sensor se conecta mediante I2C.

**ConfiguraciÛn**

```c
bool_t hmc5883lPrepareDefaultInit( HMC5883L_config_t* config );
```
- Par·metro: ``HMC5883L_config_t *config`` puntero a estructura del tipo HMC5883L_config_t a donde se cargar·n los valores por defecto de configuraciÛn.
- Retorna: ``bool_t`` TRUE.

```c
bool_t hmc5883lInit( HMC5883L_config_t config );
```
- Par·metro: ``HMC5883L_config_t *config`` estructura del tipo HMC5883L_config_t desde donde se cargar·n los valores de configuraciÛn.
- Retorna: ``bool_t`` TRUE si la configuraciÛn es correcta.

La estructura del tipo ``HMC5883L_config_t`` contiene:

- ``HMC5883L_samples_t samples`` Numero de muestras que promedia para calcular la salida de la mediciÛn. Valores admitidos:
    - HMC5883L_1_sample
    - HMC5883L_2_sample
    - HMC5883L_4_sample
    - HMC5883L_8_sample
    - HMC5883L_DEFAULT_sample = HMC5883L_1_sample
- ``HMC5883L_rate_t rate`` Bits de tasa de datos de salida. Estos bits establecen la tasa de escritura de los 3 registros de datos de salida del sensor. Valores admitidos:
    - HMC5883L_0_75_Hz
    - HMC5883L_1_50_Hz
    - HMC5883L_3_Hz
    - HMC5883L_7_50_Hz
    - HMC5883L_15_Hz
    - HMC5883L_30_Hz
    - HMC5883L_75_Hz
    - HMC5883L_DEFAULT_rate = HMC5883L_15_Hz
- ``HMC5883L_messurement_t meassurement`` Bits de configuraciÛn de mediciÛn. Estos bits definen el flujo de mediciÛn del sensor. EspecÌ≠ficamente si se aplica, o no, un bias a la mediciÛn. Valores admitidos:
    - HMC5883L_normal
    - HMC5883L_positive
    - HMC5883L_regative
    - HMC5883L_DEFAULT_messurement = HMC5883L_normal
- ``HMC5883L_gain_t gain`` Bits de configuraciÛn de ganancia. Estos bits configuran la ganancia del sensor. Esta configuraciÛn se aplica a todos los canales. Valores admitidos:
    - HMC5883L_1370 para ¬± 0.88 Ga
    - HMC5883L_1090 para ¬± 1.3 Ga
    - HMC5883L_820 para ¬± 1.9 Ga
    - HMC5883L_660 para ¬± 2.5 Ga
    - HMC5883L_440 para ¬± 4.0 Ga
    - HMC5883L_390 para ¬± 4.7 Ga
    - HMC5883L_330 para ¬± 5.6 Ga
    - HMC5883L_230 para ¬± 8.1 Ga
    - HMC5883L_DEFAULT_gain = HMC5883L_1090
- ``HMC5883L_mode_t mode``. Modo de mediciÛn. Valores admitidos:
    - HMC5883L_continuous_measurement
    - HMC5883L_single_measurement
    - HMC5883L_idle
    - HMC5883L_DEFAULT_mode = HMC5883L_single_measurement

**Prueba de lectura del magnetÛmetro**

```c
bool_t hmc5883lIsAlive( void );
```
- Par·metro: ``void`` ninguno.
- Retorna: ``bool_t`` TRUE si puede comunicarse con el sensor.

**Lectura del valor del magnetÛmetro en (x,y,z)**

```c
bool_t hmc5883lRead( int16_t* x, int16_t* y, int16_t* z );
```
- Par·metro: ``int16_t * x`` puntero entero de 16 bits con signo donde se guardar· el valor leÌ≠do del sensor HMC5883L en la componente x.
- Par·metro: ``int16_t * y`` puntero entero de 16 bits con signo donde se guardar· el valor leÌ≠do del sensor HMC5883L en la componente y.
- Par·metro: ``int16_t * z`` puntero entero de 16 bits con signo donde se guardar· el valor leÌ≠do del sensor HMC5883L en la componente z.
- Retorna: ``bool_t`` TRUE si puede leer correctamente el sensor magnetÛmetro.

### sAPI Circular Buffer

CreaciÛn y manejo de Buffers Ciculares.

**Crear el buffer (macro)**

```c
circularBufferNew( buffName, elementSize, amountOfElements );
```
- Par·metro: ``buffName`` nombre del buffer.
- Par·metro: ``elementSize`` TamaÒo en bytes de cada elemento que guardar· el buffer.
- Par·metro: ``amountOfElements`` cantidad de elementos que podra almacenar el buffer.

Esta macro realiza lo mismo que se puede declarar de forma manual:

```c
circularBuffer_t buffName;
uint8_t buffName_BufferMemory[ (amountOfElements + 1)*elementSize ];
```

Ejemplo:

```c
circularBufferNew( nombreBuffer,
                   tamaÒoEnBytesDeCadaElemento,
                   CantidadMaximaDeElementosQuePuedeAlmacenar );
```

**Estructura circularBuffer_t**

``circularBuffer_t`` es una estructura que contiene:
- ``uint8_t* memoryAddress;`` Puntero al vector donde guarda los datos el buffer.
- ``uint32_t amountOfElements;`` Cantidad m·xima de elementos en el Buffer.
- ``uint32_t elementSize;`` TamaÒo en bytes de cada elemento en el Buffer.
- ``uint32_t readIndex;`` Ìçndice de lectura del Buffer.
- ``uint32_t writeIndex;`` Ìçndice de escritura del Buffer.
- ``circularBufferStatus_t status;``  Estado del Buffer.
- ``callBackFuncPtr_t emptyBufferCallback;`` Puntero a funciÛn que se ejecuta cuando Buffer vacÌ≠o.
- ``callBackFuncPtr_t fullBufferCalback;`` Puntero a funciÛn que se ejecuta cuando Buffer lleno.

**Inicializar el buffer (macro)**

```c
circularBufferInit( *buffName, elementSize, amountOfElements );
```
- Par·metro: ``buffName`` nombre del buffer pasado por referencia.
- Par·metro: ``elementSize`` TamaÒo en bytes de cada elemento que guardar· el buffer.
- Par·metro: ``amountOfElements`` cantidad de elementos que podra almacenar el buffer.

Ejemplo:

```c
circularBufferInit( &nombreBuffer,
                    tamaÒoEnBytesDeCadaElemento,
                    CantidadMaximaDeElementosQuePuedeAlmacenar );
```

**Usar un buffer declarado en otro archivo (macro)**

Esto es necesario si quiero usar el buffer y el mismo fue creado con ``circularBufferNew()``  de forma global.

```c
circularBufferUse( buffName );
```

Esta macro realiza lo mismo que se puede declarar de forma manual:

```c
extern circularBuffer_t buffName;
```

Ejemplo:

```c
circularBufferUse( nombreBuffer );
```

**Escribir un dato en el buffer**

```c
circularBufferStatus_t circularBufferWrite( circularBuffer_t* buffer, uint8_t *dataByte );
```
- Par·metro: ``circularBuffer_t* buffer`` Buffer a escribir pasado por referencia.
- Par·metro: ``uint8_t *dataByte`` Dato a escribir pasado por referencia.
- Retorna: ``circularBufferStatus_t`` Estado del buffer.

Ejemplo:

```c
uint8_t dato = 8;
circularBufferWrite( &nombreBuffer, &dato );
```

**Leer un dato en el buffer**

```c
circularBufferStatus_t circularBufferRead( circularBuffer_t* buffer, uint8_t *dataByte );
```
- Par·metro: ``circularBuffer_t* buffer`` Buffer a leer pasado por referencia.
- Par·metro: ``uint8_t *dataByte`` Dato donde guardar lo leido del Buffer pasado por referencia.
- Retorna: ``circularBufferStatus_t`` Estado del buffer.

Ejemplo:
```c
uint8_t dato = 0;
circularBufferRead( &nombreBuffer, &dato );
```

**Condiciones de buffer lleno y vacÌ≠o**

Las funciones:

```c
circularBufferWrite( &nombreBuffer, &dato );
circularBufferRead( &nombreBuffer, &dato );
```

Retornan el estado del buffer cuando se pide leer o escribir un dato:

- ``CIRCULAR_BUFFER_NORMAL`` EscribiÛ o leyo correctamente el dato.
- ``CIRCULAR_BUFFER_EMPTY`` No pudo leer el dato porque el buffer est· vacÌ≠o.
- ``CIRCULAR_BUFFER_FULL`` No pudo escribir el dato porque el buffer est· lleno.

De esta forma podemos chequear si se leyÛ o escribiÛ con un if():

```c
if( CIRCULAR_BUFFER_EMPTY == circularBufferRead( &nombreBuffer, &dato ) ){
   // No leyo nada... Informar error o ignorar
} else{
   // leyo, hacer algo con el dato leido
}
```

Alternativamente se puede ignorar el valor retornado y setear una funciÛn de callback en caso de errores. De esta forma si al llamar a estas funciones hay un error se llama al callback correspondiente. Para setear los callbacks se usan las funciones:

```c
circularBufferEmptyBufferCallbackSet( &nombreBuffer, cuandoBufferVacio );
circularBufferFullBufferCallbackSet( &nombreBuffer, cuandoBufferLleno );
```

Las funciones de callback tienen que retornar void y recibir como par·metro un puntero a void, ejemplo:

```c
void cuandoBufferLleno(void* unused){
    // ...
}

void cuandoBufferVacio(void* unused){
    // ...
}
```

## Archivos que componen la biblioteca

**src** (.c):

- sapi_7_segment_display.c
- sapi_adc.c
- sapi_board.c
- sapi_dac.c
- sapi_datatypes.c
- sapi_delay.c
- sapi_gpio.c
- sapi_hmc5883l.c
- sapi_i2c.c
- sapi_isr_vector.c
- sapi_keypad.c
- sapi_pwm.c
- sapi_rtc.c
- sapi_sct.c
- sapi_servo.c
- sapi_sleep.c
- sapi_spi.c
- sapi_tick.c
- sapi_timer.c
- sapi_uart.c

**inc** (.h):

- sapi_7_segment_display.h
- sapi_adc.h
- sapi_board.h
- sapi_dac.h
- sapi_datatypes.h
- sapi_delay.h
- sapi_gpio.h
- sapi_hmc5883l.h
- sapi_i2c.h
- sapi_isr_vector.h
- sapi_keypad.h
- sapi_peripheral_map.h
- sapi_pwm.h
- sapi_rtc.h
- sapi_sct.h
- sapi_servo.h
- sapi_sleep.h
- sapi_spi.h
- sapi_tick.h
- sapi_timer.h
- sapi_uart.h
- sapi.h
