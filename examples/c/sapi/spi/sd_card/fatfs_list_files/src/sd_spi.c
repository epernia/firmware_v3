#include "sd_spi.h"   // <= own header (optional)
#include "sapi.h"     // <= sAPI header

#include "ff.h"       // <= Biblioteca FAT FS
#include "fssdc.h"    // API de bajo nivel para unidad SD en FAT FS

#include <string.h>

// FUNCION que se ejecuta cada vezque ocurre un Tick
void diskTickHook( void *ptr );

FATFS fs;
FRESULT res;
char buff[256];

FRESULT scan_files (char* path)
{
   FRESULT res;
   DIR dir;
   UINT i;
   static FILINFO fno;

   res = f_opendir(&dir, path);                       /* Open the directory */
   if (res == FR_OK) {
      for (;;) {
         res = f_readdir(&dir, &fno);                   /* Read a directory item */
         if (res != FR_OK || fno.fname[0] == 0) break;  /* Break on error or end of dir */
         if (fno.fattrib & AM_DIR) {                    /* It is a directory */
            i = strlen(path);
            sprintf(&path[i], "/%s\r\n", fno.fname);
            res = scan_files(path);                    /* Enter the directory */
            if (res != FR_OK) break;
            path[i] = 0;
         } else {                                       /* It is a file. */
            printf("%s/%s\r\n", path, fno.fname);
         }
      }
      f_closedir(&dir);
   }

   return res;
}


int main (void)
{
   spiConfig( SPI0 );
   // Inicializar el conteo de Ticks con resolucion de 10ms,
   // con tickHook diskTickHook
   tickConfig( 10 );
   tickCallbackSet( diskTickHook, NULL );   
   
   FSSDC_InitSPI ();
   strcpy (buff, "SDC:");   
   res = f_mount(&fs, "SDC:", 1);
   if (res == FR_OK) {
      strcpy(buff, "/");
      res = scan_files(buff);
   }

   return res;
}


// FUNCION que se ejecuta cada vezque ocurre un Tick
void diskTickHook( void *ptr )
{
   disk_timerproc();   // Disk timer process
}
