#include <conio.h>

#include <sapi.h>

static const gpioMap_t keys[] = { TEC1, TEC2, TEC3, TEC4 };
static int keyIdx;

int _kbhit()
{
   for ( int i=0; i<4; i++ )
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
