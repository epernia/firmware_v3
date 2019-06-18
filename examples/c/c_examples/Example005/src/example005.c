/*****************************************************************************
5. Realice nuevamente el ejercicio 3 pero esta vez utilice los arreglos 
x[], x2[] y x3[] para almacenar primero los valores correspondientes de 
x, x al cuadrado y x al cubo, respectivamente.
Luego repita la tabla a partir de los arreglos anteriores.

*****************************************************************************/

#include <stdio.h>
#include <stdint.h>

int main()
{
   uint16_t x[21], x2[21], x3[21];

   printf("x \tx al cuadrado \tx al cubo\r\n");

   uint8_t i;

   for( i=0; i<=20; i++ ){
      x[i] = i;
      x2[i] = i*i;
      x3[i] = i*i*i;
   }

   for( i=0; i<=20; i++ ){
      printf("%d \t%d \t\t%d\r\n", x[i], x2[i], x3[i] );
   }

   printf("\r\n\n%d \t%d \t\t%d\r\n", x[5], x2[5], x3[5] );

   return 0;
}
