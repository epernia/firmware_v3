#include <conio.h>

#include <sapi.h>

/* Board specifics. */
#if (BOARD == ciaa_nxp)
   #define CONIO_KEYS 8
   static const gpioMap_t keys[] = { DI0, DI1, DI2, DI3, DI4, DI5, DI6, DI7 };
#elif (BOARD == edu_ciaa_nxp)
   #define CONIO_KEYS 4
   static const gpioMap_t keys[] = { TEC1, TEC2, TEC3, TEC4 };
#elif (BOARD == lpcxpresso1769)
   #define CONIO_KEYS 1
   static const gpioMap_t keys[] = { 0 }; // Not supported yet
#endif

static int keyIdx;

int _kbhit()
{
   for ( int i=0; i<CONIO_KEYS; i++ )
      if ( gpioRead(keys[i]) == 0 ) {
         keyIdx = i;
         return 1;
      }
   return 0;
}

int _getch()
{
   return keys[keyIdx];
}
