/*
    SDCard/USB Mass Storage Wrappers - Test and usage example
    Copyright 2018 Santiago Germino (royconejo@gmail.com)
    
    Redistribution and use in source and binary forms, with or without
    modification, are permitted provided that the following conditions are met:

    1.  Redistributions of source code must retain the above copyright notice,
        this list of conditions and the following disclaimer.

    2.  Redistributions in binary form must reproduce the above copyright
        notice, this list of conditions and the following disclaimer in the
        documentation and/or other materials provided with the distribution.

    3.  Neither the name of the copyright holder nor the names of its
        contributors may be used to endorse or promote products derived from
        this software without specific prior written permission.

    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
    AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
    IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
    ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
    LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
    CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
    SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
    INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
    CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
    ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
    POSSIBILITY OF SUCH DAMAGE.
    
    ----------------------------------------------------------------------------
    sgermino 28/08/2018:
    
    Utilizar los wrappers "sdcard" y "usbms" es solo una forma conveniente de 
    usar los dispositivos de FatFs que actualmente soporta este firmware. La 
    ventaja es que permiten tener nocion del estado actual del dispositivo y 
    se realiza un montaje automatico (f_mount) apenas es posible y un desmontaje
    automatico de la unidad cuando esta se extrae.
    
    La segunda opcion, utilizar las APIs FSSDC y FSUSB (las que usa internamente
    este firmware para el driver de los dispositivos en FatFs) requiere 
    configurar un callback para notificarse de esos estados y realizar el 
    mount/unmount de forma manual.
    
    La tercera opcion es utilizar FatFs directamente. Al realizar un f_mount()
    sobre USB: o SDC:, el driver de FatFs intentara inicializar el dispositivo
    usando las APIs FSUSB o FSSDC. El problema (y la razon de la existencia de 
    los wrappers) es que al llamar a f_mount no se conoce el estado actual del 
    dispositivo. Lo mas probable es que el montaje falle sin conocer la
    verdadera causa, que puede no ser un error. Por ejemplo, en el caso de USB,
    el pendrive puede insertarse o removerse en cualquier momento.
    Y existe una demora importante entre que se inserta y esta disposible para
    ser montado. Del mismo modo, se debe desmontar el dispositivo en FatFs 
    apenas este se extraiga.
    
    En este ejemplo, comentar la definicion de EXAMPLE_TEST_{SDCARD/USBMS}_
    WRAPPER para activar o desactivar el uso y testeo del wrapper elegido.
    ----------------------------------------------------------------------------
    ----------------------------------------------------------------------------
    Pasos para ejecutar este ejemplo:
    
    1) Preparar un pendrive formateado en FAT16, FAT32 o exFAT.
    2) Idem tarjeta de memoria. 
    (Todos los pendrives y tarjetas deberian funcionar directamente a menos que
     se los haya formateado en un sistema de archivos raro)
    
    3) Conectar la Edu-CIAA, compilar y flashear este programa.
    4) Desconectar la Edu-CIAA!
    5) Conectar el SD Card reader como se muestar en la figura "SD SPI.png".
    
    6) Reconectar la Edu-CIAA.
        Las luces LED_1/2/3 indican progreso:
    
        LED_1: supero inicializacion.
        LED_2: dispositivos montados.
        LED_3: test finalizado.
        
    Finalmente, el led RGB Verde indica que el test fue exitoso. El ledo RGB
    Rojo indica que hubo un error.
        
    6) Este programa envia mucha informacion de estado por el puerto serie.
       Conectar una terminal al puerto serie USB de la Edu-CIAA (gralmente 
       /dev/ttyUSB1 en linux)
    ----------------------------------------------------------------------------
    ----------------------------------------------------------------------------
    FAQ:
        Q: Por que no se muestran todos los estados USB o SDCARD?
        A: Varios estados ocurren en el INIT del wrapper, antes del bucle en
           donde se pollea al wrapper. Si realmente fuese necesario mostrar
           (enterarse) de los estados apenas ocurren, lo mejor seria no usar los
           wrappers, sino directamente las APIs FSSDC y FSUSB seteando nuestros 
           propios callbacks:
           
           FSSDC_SetStatusUpdateCallback()
           FSUSB_SetStatusUpdateCallback()
           
        Q: En la consola veo mensajes de "FSUSB:", de donde salen? Como los
           desactivo?
        A: Son mensajes de debug de la API FSUSB, usando DEBUGSTR() definido en
           board_api.h. Para desactivarlos hay que compilar sin definir
           "DEBUG_ENABLE" (ver el config.mk de este proyecto).
    ----------------------------------------------------------------------------
*/

#include "sapi_sdcard.h"
#include "sapi_usbms.h"
#include "sapi.h"

#include "ff.h"

#ifndef LPCUSBLIB_HOST_MASS_STORAGE
    #error No olvidar USE_LPCUSBLIB=y en config.mk!
#endif

#define EXAMPLE_TEST_SDCARD_WRAPPER
#define EXAMPLE_TEST_USBMS_WRAPPER

#if !defined(EXAMPLE_TEST_SDCARD_WRAPPER) && !defined(EXAMPLE_TEST_USBMS_WRAPPER)
    #error Se debe probar al menos un wrapper!
#endif

// Wrappers para manejar dispositivos USB Mass Storage y tarjeta SD
sdcard_t sdcard;
usbms_t usbms;

// Se guarda el ultimo estado para enviar un mensaje solo cuando el estado 
// cambie.
sdcardStatus_t sdcardUltimoEstado = -1;
usbmsStatus_t usbmsUltimoEstado = -1;


void diskTickHook ( void *ptr );


static void mostrarEstadoTarjetaSD( void )
{
    // El estado actual es distinto al ultimo mostrado?
    if ( sdcardUltimoEstado == sdcardStatus() )
    {
        // Es igual, no hay nada que hacer aca
        return;
    }
    
    // Es diferente, guardo y envio estado
    sdcardUltimoEstado = sdcardStatus();
    
    switch( sdcardUltimoEstado )
    {
        case SDCARD_Status_Removed:
            uartWriteString( UART_USB, "STATUS: Por favor inserte Tarjeta SD.\r\n" );
            break;
        
        case SDCARD_Status_Inserted:
            uartWriteString( UART_USB, "STATUS: Tarjeta SD insertada.\r\n" );
            break;
    
        case SDCARD_Status_NativeMode:
            uartWriteString( UART_USB, "STATUS: Configurando tarjeta SD.\r\n" );
            break;
            
        case SDCARD_Status_Initializing:
            uartWriteString( UART_USB, "STATUS: Iniciando tarjeta SD.\r\n" );
            break;
        
        case SDCARD_Status_ReadyUnmounted:
            uartWriteString( UART_USB, "STATUS: Tarjeta SD lista pero desmontada.\r\n" );
            break;

        case SDCARD_Status_ReadyMounted:
            uartWriteString( UART_USB, "STATUS: Tarjeta SD lista y montada.\r\n" );
            break;
        
        case SDCARD_Status_Error:
            uartWriteString( UART_USB, "STATUS: Tarjeta SD en estado de Error.\r\n" );
            break;
    }
}


static void mostrarEstadoUSBMassStorage( void )
{
    // El estado actual es distinto al ultimo mostrado?
    if ( usbmsUltimoEstado == usbmsStatus() )
    {
        // Es igual, no hay nada que hacer aca
        return;
    }
    
    // Es diferente, guardo y envio estado
    usbmsUltimoEstado = usbmsStatus();
    
    switch( usbmsUltimoEstado )
    {
        case USBMS_Status_StorageDetached:
            uartWriteString( UART_USB, "STATUS: Por favor inserte USB Mass Storage (pendrive).\r\n" );
            break;
        
        case USBMS_Status_StorageAttached:
            uartWriteString( UART_USB, "STATUS: Configurando USB Mass Storage.\r\n" );
            break;
    
        case USBMS_Status_StorageConfigured:
            uartWriteString( UART_USB, "STATUS: USB Mass Storage configurado.\r\n" );
            break;
            
        case USBMS_Status_StorageEnumerated:
            uartWriteString( UART_USB, "STATUS: USB Mass Storage enumerado.\r\n" );
            break;
        
        case USBMS_Status_StorageReadyUnmounted:
            uartWriteString( UART_USB, "STATUS: UBS Mass Storage listo pero desmontado.\r\n" );
            break;

        case USBMS_Status_StorageReadyMounted:
            uartWriteString( UART_USB, "STATUS: USB Mass Storage listo y montado.\r\n" );
            break;
        
        case USBMS_Status_StorageError:
            uartWriteString( UART_USB, "STATUS: USB Mass Storage en estado de Error.\r\n" );
            break;
    }   
}


static void fatFsTestStart( const char* test )
{
    char msg[256];
    sprintf( msg, "TEST: Ejecutando '%s'...", test );
    uartWriteString( UART_USB, msg );
}


static void fatFsTestOK( void )
{
    uartWriteString( UART_USB, "OK!\r\n" );
}


static void fatFsTestERROR( int error )
{
    char msg[256];
    sprintf( msg, "ERROR %i.\r\n", error );
    uartWriteString( UART_USB, msg );
}


static bool fatFsTest( const char *unidad )
{
    char buf[1024];
    char filename[64];    
    FIL file;
    FRESULT fr;
    int r;
    
    sprintf( filename, "%s/TEST.TXT", unidad );

    uartWriteString( UART_USB, "\r\n-------------------------------------------\r\n" );   
    sprintf( buf, "TEST sobre archivo '%s'.\r\n", filename);
    uartWriteString( UART_USB, buf);
    uartWriteString( UART_USB, "-------------------------------------------\r\n" ); 
    
    // Ver http://elm-chan.org/fsw/ff/00index_e.html para una referencia de la
    // API de FatFs
    
    // Abre un archivo. Si no existe lo crea, si existe, lo sobreescribe.
    fatFsTestStart( "f_open( WRITE )" );
    fr = f_open( &file, filename, FA_CREATE_ALWAYS | FA_WRITE);
    if( fr != FR_OK )
    {
        fatFsTestERROR( fr );
        return false;
    }
    fatFsTestOK( );
 
    // Prueba de f_putc
    fatFsTestStart( "f_putc" );
    sprintf (buf, "La unidad bajo prueba es '%s'\r\n"
                  "Lista de caracteres ASCII:\r\n",
                  unidad);
    // Escribe mensaje
    for (uint32_t i = 0; i < strlen(buf); ++i)
    {
        r = f_putc( buf[i], &file );
        if (r < 1)
        {
            fatFsTestERROR( r );
            f_close( &file );
            return false;
        }
    }
    
    // Escribe todos los caracteres UTF-8 que overlapean ASCII
    // (sin combinaciones multibyte)    
    for (uint32_t i = 32; i < 127; ++i)
    {
        r = f_putc( (TCHAR)i, &file );
        if (r < 1)
        {
            fatFsTestERROR( r );
            f_close( &file );
            return false;
        }
    }
    fatFsTestOK( );
    
    // Prueba f_puts
    fatFsTestStart( "f_puts");  
    sprintf (buf, "\r\n"
                  "Fecha y hora de compilacion del programa: %s %s\r\n"
                  "Estado de pulsadores TEC 1 a 4 al momento de la prueba: %i%i%i%i\r\n",
                  __DATE__, __TIME__,
                  gpioRead( TEC1 ), gpioRead( TEC2 ), gpioRead( TEC3 ), gpioRead( TEC4 ));
    
    r = f_puts( buf, &file );
    if (r < 1)
    {
       fatFsTestERROR( r );
       f_close( &file );
       return false;
    }
    fatFsTestOK( );
    
    // Cierra el archivo y vuelve a abrirlo como LECTURA
    f_close( &file );
    
    fatFsTestStart( "f_open( READ )" );
    fr = f_open( &file, filename, FA_READ );
    if( fr != FR_OK )
    {
       fatFsTestERROR( fr );
       return false;
    }
    fatFsTestOK( );
    
    // Borro contenido del buffer, para que no haya dudas de que el contenido
    // se leyo desde el archivo
    memset( buf, 0, sizeof(buf) );
    
    // Carga el contenido del archivo
    UINT bytesLeidos = 0;
     fatFsTestStart( "f_read" );
    fr = f_read( &file, buf, sizeof(buf), &bytesLeidos );
    if (fr != FR_OK)
    {
       fatFsTestERROR( fr );
       return false;
    }
    fatFsTestOK( );
    
    f_close( &file );

    uartWriteString( UART_USB, "\r\n");        
    uartWriteString( UART_USB, ">>>> INICIO CONTENIDO DEL ARCHIVO LEIDO >>>>\r\n");    
    uartWriteString( UART_USB, buf );
    uartWriteString( UART_USB, "<<<< FIN CONTENIDO DEL ARCHIVO LEIDO <<<<\r\n");    
    return true;   
}


int main( void )
{
    char msg[256];
    
    // ---------- CONFIGURACIONES ------------------------------
    // Inicializar y configurar la plataforma
    boardConfig();
    
    // SPI configuration
    spiConfig( SPI0 );
    
    // Inicializar el conteo de Ticks con resolucion de 10ms,
    // con tickHook diskTickHook
    tickConfig( 10 );
    tickCallbackSet( diskTickHook, NULL );

    // Inicializar UART_USB a 115200 baudios
    uartConfig( UART_USB, 115200 );
        
    uartWriteString( UART_USB, "\r\n" );
    uartWriteString( UART_USB, "------------------------------------------------\r\n" );    
    uartWriteString( UART_USB, "Bienvenido a la prueba de wrappers sdcard/usbms!\r\n" );
    uartWriteString( UART_USB, "------------------------------------------------\r\n" ); 
    
    /*
        Inicializa los wrappers sdcard y usbms.
 
        Una vez iniciados los wrappers, sus respectivos dispositivos (Tarjeta
        SD y USB Mass Storage/Pendrive) seran iniciados y auto-montados apenas 
        se inserten o conecten.
        
        USB soporta attach y detach de dispositivos de manera dinamica. En el
        caso de la tarjeta SD, se requiere que el lector este totalmente 
        cableado y con una tarjeta SD insertada antes de llamar a sdcardInit!

        Esto ultimo es una limitacion de los kits de lectura de tarjeta SD, ya
        que casi ninguno viene con el switch de deteccion de presencia de 
        tarjeta en el lector.
    */
    
    #ifdef EXAMPLE_TEST_SDCARD_WRAPPER
    uartWriteString( UART_USB, "Iniciando sdcard con configuracion:\r\n" );
    sprintf( msg, "  velocidad inicial %i Hz.\r\n", FSSDC_GetSlowClock() );
    uartWriteString( UART_USB, msg );
    sprintf( msg, "  velocidad de trabajo %i Hz.\r\n", FSSDC_GetFastClock() );    
    uartWriteString( UART_USB, msg );  
    if (sdcardInit( &sdcard ) == false)
    {
        uartWriteString( UART_USB, "Inicio de sdcard FALLO.\r\n**FIN**\r\n" );
        while( 1 );
    }
    else {
        uartWriteString( UART_USB, "Inicio de sdcard OK! Unidad FatFs '" );
        uartWriteString( UART_USB, sdcardDriveName());
        uartWriteString( UART_USB, "'.\r\n" );
    }
    #else
    uartWriteString( UART_USB, "NO se probara sdcard.\r\n" );
    #endif
    
    #ifdef EXAMPLE_TEST_USBMS_WRAPPER
    uartWriteString( UART_USB, "Iniciando usbms.\r\n" ); 
    if (usbmsInit( &usbms ) == false)
    {
        uartWriteString( UART_USB, "Inicio de usbms FALLO.\r\n**FIN**\r\n" );
        while( 1 );        
    }
    else {
        uartWriteString( UART_USB, "Inicio de usbms OK! unidad FatFs '" );
        uartWriteString( UART_USB, usbmsDriveName());
        uartWriteString( UART_USB, "'.\r\n" );
    }
    #else
    uartWriteString( UART_USB, "NO se probara usbms.\r\n" );
    #endif
    
    // Se paso la primer etapa con exito, enciende LED1
    gpioWrite( LED1, ON );
    
    /*
        En el siguiente bucle consultaremos los estados de ambos dispositivos,
        pudiendo presenciar como transcurren sus estados desde que se conectan
        hasta que se inician y auto-montan.
        
        una vez iniciados y montados, podremos realizar un test estandar de
        lectura y escritura de archivos con la libreria FatFs.
    */
    uartWriteString( UART_USB, "Logueando STATUS de dipositivos...\r\n" ); 
    while( 1 )
    {        
        // Muestra el estado actual de los dispositivos
        #ifdef EXAMPLE_TEST_SDCARD_WRAPPER
        mostrarEstadoTarjetaSD();
        #else
        sdcardUltimoEstado = SDCARD_Status_ReadyMounted;
        #endif
        
        #ifdef EXAMPLE_TEST_USBMS_WRAPPER
        mostrarEstadoUSBMassStorage();
        #else
        usbmsUltimoEstado = USBMS_Status_StorageReadyMounted;
        #endif
   
        // Si ambos dispositivos ya estan iniciados y montados, salimos de este
        // bucle y comenzamos los testeos sobre archivos utilizando FatFs
        if( sdcardUltimoEstado == SDCARD_Status_ReadyMounted &&
            usbmsUltimoEstado == USBMS_Status_StorageReadyMounted)
        {
            break;            
        }
    }
   
    // Se paso la segunda etapa con exito, enciende LED2
    gpioWrite( LED2, ON );
    
    // Se ejecutan los testeos de archivos en el nombre de unidad
    // correspondiente a los dos dispositivos; "SDC:" para tarjeta SD y "USB:"
    // para Mass Storage.
    bool sdcardTestResult = true;
    bool usbmsTestResult = true;
    
    #ifdef EXAMPLE_TEST_SDCARD_WRAPPER
    sdcardTestResult = fatFsTest (sdcardDriveName());
    #endif
    
    #ifdef EXAMPLE_TEST_USBMS_WRAPPER
    usbmsTestResult  = fatFsTest (usbmsDriveName());
    #endif
    
    // Se paso la ultima etapa, enciende LED3
    gpioWrite( LED3, ON );
    
    // Si ambos resultados son positivos, se enciende led RGB verde, caso
    // contrario se enciende led RGB rojo.
    if (sdcardTestResult && usbmsTestResult)
    {
        gpioWrite( LEDG, ON );
        uartWriteString( UART_USB, "--------\r\n" );        
        uartWriteString( UART_USB, "TEST OK!\r\n" );
        uartWriteString( UART_USB, "--------\r\n" );                
    }
    else {
        gpioWrite( LEDR, ON );
        uartWriteString( UART_USB, "------------------------\r\n" );                
        uartWriteString( UART_USB, "TEST Finalizo con ERROR.\r\n" );        
        uartWriteString( UART_USB, "------------------------\r\n" );        
    }

    // Fin del programa
    uartWriteString( UART_USB, "**FIN**\r\n" );
    while( 1 );
    
    return 0;
}


// FUNCION que se ejecuta cada vez que ocurre un Tick
void diskTickHook( void *ptr )
{
    disk_timerproc(); 
}
