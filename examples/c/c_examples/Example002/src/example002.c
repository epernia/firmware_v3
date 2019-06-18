/*****************************************************************************
2. Operadores.

a. Indique el resultado de las siguientes expresiones:

(1 >2) || (1<2)
(1>2) && 3
1 + 2 / 3 * 4 – 5

b. Considerando: int a=1, b=2, c=3 diga cuanto valen cada una de las siguientes
   expresiones:

a<b && c<b
a<b && c
a<=c || b>c
a!=b-1 || b>2 || a+c>2


******************************************************************************/

/*
    (1 >2) || (1<2)     V
    (1>2) && 3          F
    -4                  V   
    
    int a=1, b=2, c=3;
    
    1<2 && 3<2               F      
    1<2 && 3                 V
    1<=3 || 2>3              V
    1!=2-1 || 2>2 || 1+3>2   V
*/    
    
#include <stdio.h>
#include <stdint.h>

int main()
{
   uint16_t x[21], x2[21], x3[21];

   printf("a)\r\n");
   
   // Uso el "If rapido" de C, ver https://biengeek.wordpress.com/2009/09/16/el-operador-interrogacion/
   printf("(1 >2) || (1<2) ==> %s", ((1 >2) || (1<2))?"Verdadero\r\n":"Falso\r\n" ); 
   printf("(1>2) && 3      ==> %s", ((1>2) && 3)?"Verdadero\r\n":"Falso\r\n" );
   printf("-4              ==> %s", (-4)?"Verdadero\r\n":"Falso\r\n" );

   printf("\r\nb)\r\n");
   
   int a=1, b=2, c=3;
   
   printf("a<b && c<b             ==> %s", (a<b && c<b)?"Verdadero\r\n":"Falso\r\n" ); 
   printf("a<b && c               ==> %s", (a<b && c)?"Verdadero\r\n":"Falso\r\n" );
   printf("a<=c || b>c            ==> %s", (a<=c || b>c)?"Verdadero\r\n":"Falso\r\n" );
   printf("a!=b-1 || b>2 || a+c>2 ==> %s", (a!=b-1 || b>2 || a+c>2)?"Verdadero\r\n":"Falso\r\n" );

   return 0;
}
    