/*****************************************************************************
3. Bucles 1

Realice una tabla utilizando printf() en la UART con el siguiente encabezado:

x      	x al cuadrado     	x al cubo

a) complete con valores enteros de x entre 0 y 20 utilizando un bucle while.
b) complete con valores enteros de x entre 0 y 20 utilizando un bucle for.
c) complete con valores reales de x entre 0 y 10, con incrementos de 0.5 
utilizando un bucle while.
d) complete con valores reales de x entre 0 y 10, con incrementos de 0.5 
utilizando un bucle for.

Nota: utilice el caracter '\t' que realiza la funcion de tab para ordenar 
correctamente las tablas en la Terminal. Revisar si lo soporta la terminal 
serie que utiliza.

*****************************************************************************/

#include <stdio.h>
#include <stdint.h>

int main()
{
   uint16_t x = 0;
   printf("x \tx al cuadrado \tx al cubo\r\n");

   uint8_t i; 

   for( i=0; i<=20; i++ ){
      printf("%d \t%d \t\t%d\r\n", x, x*x, x*x*x );
      x++;
   }

   printf("------------------------------------------\r\n");

   printf("x \tx al cuadrado \tx al cubo\r\n");
   i=0;
   x=0;
   while(i<=20){
      printf("%d \t%d \t\t%d\r\n", x, x*x, x*x*x );
      x++;
      i++;
   }

   printf("------------------------------------------\r\n");
   printf("x \t\tx al cuadrado \tx al cubo\r\n");

   float j = 0.0;
   for( j=0.0; j<=20.0; j=j+0.5 ){
      printf("%f \t%f \t%f\r\n", j, j*j, j*j*j );
   }

   return 0;
}
