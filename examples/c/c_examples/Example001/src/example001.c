/*****************************************************************************
1. Suponga que una variable declarada como

uint16_t temperatura_sensor;

contiene la salida de un sensor de temperatura con una resolucion de 10 bits
y un rango de 0 C a 800 C.Esto es, el valor temperatura_sensor = 0
correspondera a la medida de 0 C mientras que temperatura_sensor = 1023
correspondera a la medida de 800 C.

Sería deseable disponer de una nueva variable: temperatura_grados que almacene
la medida directamente en grados para poder procesarla y mostrarla por UART 
directamente.

a. Escriba el programa que transforma el valor entregado por el sensor a 
grados Celcius y lo almacena en temperatura_grados.

b. Pruebe distintos valores de temperatura_sensor y conviértalos a 
temperatura_grados.

Imprima los valores por UART. Por ejemplo
temperatura_sensor = 0    --> temperatura_grados = 0 C
temperatura_sensor = 128  --> temperatura_grados = 100 C
temperatura_sensor = 512  --> temperatura_grados = 400 C
temperatura_sensor = 1023 --> temperatura_grados = 800 C

c. Sabiendo que el rango de la variable temperatura_sensor admite valores 
menores que 0 y mayores que 1023, busque una solucion para detectar estos 
casos e indicarle al usuario de que hay una falla en el sensor.

******************************************************************************/

#include <stdio.h>
#include <stdint.h>

void convertTempC( int16_t );
void convertTempCfloat( float );

int main()
{
    convertTempC(0);
    convertTempC(128);
    convertTempC(512);
    convertTempC(1023);
    
    convertTempCfloat(0);
    convertTempCfloat(128);
    convertTempCfloat(512);
    convertTempCfloat(1023);

    return 0;
}

void convertTempCfloat( float temperatura_sensor )
{
    float temperatura_grados = 0;
    
    /*
                    1023 --- 800
      temperatura_sensor --- temperatura_grados = (temperatura_sensor * 800) / 1023
    */
    temperatura_grados = (temperatura_sensor * 800.0) / 1023.0;
    
    printf( "temperatura_sensor = %f --> temperatura_grados = %f C\r\n", 
            temperatura_sensor, temperatura_grados );
}

void convertTempC( int16_t temperatura_sensor )
{
    int16_t temperatura_grados = 0;
    
    /*
                    1023 --- 800
      temperatura_sensor --- temperatura_grados = (temperatura_sensor * 800) / 1023
    */
    temperatura_grados = (temperatura_sensor * 800) / 1023;
    
    printf( "temperatura_sensor = %d --> temperatura_grados = %d C\r\n", 
            temperatura_sensor, temperatura_grados );
}
