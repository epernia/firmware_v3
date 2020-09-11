/*****************************************************************************
4. Bucles 2

Realice los siguientes "dibujos" en la Terminal Serie parametrizados segun
el valor de N y M, segun corresponda (usar #define para N y M).

a)                b)                c)                d)
*                 *                 1                 1
*                 **                22                12
*                 ***               333               123
*                 ****              4444              1234
(N filas)         (N filas)         (N filas)         (N filas)

e)                f) El cuadrado:   g) El triángulo:
*******           *******           *******
*******           *     *            *   *
*******           *     *             * *
(N filas x        *******              *
M columnas)       (N filas x        (N filas, con
                  M columnas)       N impar)

*****************************************************************************/

#include <stdio.h>
#include <stdint.h>

int main( void )
{
   int f = 0;
   int c = 0;
   int N = 0;
   int M = 0;

   printf("Ingrese la cantidad de filas:");
   fflush(stdout);

   scanf("%d", &N);

   printf("\r\nIngrese la cantidad de columnas:");
   fflush(stdout);

   scanf("%d", &M);
   printf("\r\nIngreso %d filas y %d columnas\r\n\r\n", N, M);

   printf("Ej 4) a)\r\n");
   for( f=0; f<N; f++ ) {
      printf( "*\r\n" );
   }

   printf("\r\nEj 4) b)\r\n");
   for( f=0; f<N; f++ ) {
      for( c=0; c<=f; c++ ) {
         printf("*");
      }
      printf("\r\n");
   }

   printf("\r\nEj 4) c)\r\n");
   for( f=1; f<=N; f++ ) {
      for( c=0; c<f; c++ ) {
         printf("%d", f);
      }
      printf("\r\n");
   }

   printf("\r\nEj 4) d)\r\n");
   for( f=1; f<=N; f++ ) {
      for( c=1; c<=f; c++ ) {
         printf("%d", c);
      }
      printf("\r\n");
   }

   printf("\r\nEj 4) e)\r\n");
   for( f=1; f<=N; f++ ) {
      for( c=1; c<=M; c++ ) {
         printf("*");
      }
      printf("\r\n");
   }

   printf("\r\nEj 4) f)\r\n");
   for( f=1; f<=N; f++ ) {
      for( c=1; c<=M; c++ ) {
         if( f==1 || f==N || c==1 || c==M ) {
            printf("*");
         } else {
            printf(" ");
         }
      }
      printf("\r\n");
   }

   printf("\r\nEj 4) g)\r\n");
   for( f=1; f<=N; f++ ) {
      for( c=1; c<=(2*N-f); c++ ) {
         if( (c==f) || (c==(2*N-f)) || (f==1) )
            printf("*");
         else
            printf(" ");
      }
      printf("\r\n");
   }

   return 0;
}
