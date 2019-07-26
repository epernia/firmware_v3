#include "sapi.h"       // <= sAPI header
#include "ff.h"         // <= Biblioteca FAT FS
#include "fssdc.h"      // API de bajo nivel para unidad "SDC:" en FAT FS


// FUNCION que se ejecuta cada vezque ocurre un Tick
void diskTickHook( void *ptr );


char buf[100];

void showDateAndTime( rtc_t *rtc ) {
   sprintf( buf, "%04d-%02d-%02d %02d:%02d:%02d\r\n",
      (*rtc).year,
      rtc->month,
      rtc->mday,
      rtc->hour,
      rtc->min,
      rtc->sec
   );
   uartWriteString( UART_USB, buf );
}

static FATFS fs;           // <-- FatFs work area needed for each volume
static FIL fp;             // <-- File object needed for each open file
static rtc_t rtc = {
   2018,
   8,
   16,
   4,
   20,
   10,
   0
};

int main(void){
   boardConfig();
   uartConfig( UART_USB, 115200 );
   
   // SPI configuration
   spiConfig( SPI0 );
   
   // Inicializar el conteo de Ticks con resolucion de 10ms,
   // con tickHook diskTickHook
   tickConfig( 10 );
   tickCallbackSet( diskTickHook, NULL );
   
   printf("Inicializando\n");
   FSSDC_InitSPI ();
   if( f_mount( &fs, "SDC:", 0 ) != FR_OK ){
      while (1) {
         gpioToggle( LEDR );
         printf("SD no disponible\n");
         delay(1000);
      }
   }
   /*
   // Leer archivo
   if( f_open( &fp, "SDC:/log.txt", FA_READ ) == FR_OK ){
      while (!f_eof( &fp ) ) {
         int nbytes;
         f_read ( &fp , buf, 100, &nbytes);
         printf("%s", buf );
      }
   } else
      printf(" Error al abrir archivo\n");
   fflush(stdout);
   while (1)
      ;
   */

   printf("Inicializamos RTC\n");
   rtcInit(); // Inicializar RTC 
   rtcWrite( &rtc ); // Establecer fecha y hora
   delay(2000);
   printf("RTC listo\n");

   while (1) {
      rtcRead( &rtc );
      showDateAndTime( &rtc );
      if( f_open( &fp, "SDC:/log.txt", FA_WRITE | FA_OPEN_APPEND ) == FR_OK ){
         int nbytes;
         int n = sprintf( buf, "Hola mundo %04d-%02d-%02d %02d:%02d:%02d\r\n",
            rtc.year,
            rtc.month,
            rtc.mday,
            rtc.hour,
            rtc.min,
            rtc.sec
         );
         f_write( &fp, buf, n, &nbytes );

         f_close(&fp);

         if( nbytes == n ){
            printf("Escribio correctamente\n");
            gpioWrite( LEDG, ON );
         } else {
            gpioWrite( LEDR, ON );
            printf("Escribio %d bytes\n", nbytes);
         }
      } else{
         printf("Error al abrir el archivo\n");
         gpioWrite( LEDR, ON );
      }
      delay(1000);
   }
}


// FUNCION que se ejecuta cada vezque ocurre un Tick
void diskTickHook( void *ptr )
{
   disk_timerproc();   // Disk timer process
}
