/*=============================================================================
 * Copyright (c) 2019, Eric Pernia <ericpernia@gmail.com>
 * All rights reserved.
 * License: bsd-3-clause (see LICENSE.txt)
 * Date: 2019/08/24
 * Version: 1.0.0
 *===========================================================================*/

/*=====[Inclusions of function dependencies]=================================*/

#include "sapi.h"
#include "axis.h"

/*=====[Definition macros of private constants]==============================*/

// Send ----------------------------------

#define DATA_SIZE   128 // Cantidad de bytes a tranasimitir/recibir

static uint8_t tx_data[DATA_SIZE] = {
   0x01,
   0x02,
   0x04,
   0x08,
   0x10,
   0x20,
   0x40,
   0x80,
   
   0x00,
   0xAA,
   0x55,
   0xFF,
   0xA5,
   0x5A,
   0xCA,
   0xFE, 

   0x01,
   0x02,
   0x04,
   0x08,
   0x10,
   0x20,
   0x40,
   0x80,
   
   0x00,
   0xAA,
   0x55,
   0xFF,
   0xA5,
   0x5A,
   0xCA,
   0xFE,  // 32

   0x01,
   0x02,
   0x04,
   0x08,
   0x10,
   0x20,
   0x40,
   0x80,
   
   0x00,
   0xAA,
   0x55,
   0xFF,
   0xA5,
   0x5A,
   0xCA,
   0xFE, 

   0x01,
   0x02,
   0x04,
   0x08,
   0x10,
   0x20,
   0x40,
   0x80,
   
   0x00,
   0xAA,
   0x55,
   0xFF,
   0xA5,
   0x5A,
   0xCA,
   0xFE,  // 64


   0x01,
   0x02,
   0x04,
   0x08,
   0x10,
   0x20,
   0x40,
   0x80,
   
   0x00,
   0xAA,
   0x55,
   0xFF,
   0xA5,
   0x5A,
   0xCA,
   0xFE, 

   0x01,
   0x02,
   0x04,
   0x08,
   0x10,
   0x20,
   0x40,
   0x80,
   
   0x00,
   0xAA,
   0x55,
   0xFF,
   0xA5,
   0x5A,
   0xCA,
   0xFE, // 96

   0x01,
   0x02,
   0x04,
   0x08,
   0x10,
   0x20,
   0x40,
   0x80,
   
   0x00,
   0xAA,
   0x55,
   0xFF,
   0xA5,
   0x5A,
   0xCA,
   0xFE, 

   0x01,
   0x02,
   0x04,
   0x08,
   0x10,
   0x20,
   0x40,
   0x80,
   
   0x00,
   0xAA,
   0x55,
   0xFF,
   0xA5,
   0x5A,
   0xCA,
   0xFE, // 128
};

/*
#define DATA_SIZE   6 // Cantidad de datos a tranasimitir/recibir

static uint8_t tx_data[DATA_SIZE] = {
   0b00000101,
   0b00001010,
   0b00010100,
   0b00101000,
   0b01010000,
   0b10100000,
};
*/

// Receive -------------------------------

static uint8_t rx_data[DATA_SIZE];

void keysUpdate( void );

/*=====[Main function, program entry point after power on or reset]==========*/

int main( void )
{
   // ----- Setup -----------------------------------
   boardInit();   
   axisInit( tx_data, DATA_SIZE, 
             rx_data, DATA_SIZE ); 
   printf( "Hola\r\n\r\n" );
   // ----- Repeat for ever -------------------------
   while( true ) {      
      axisUpdate();
      keysUpdate();
   }
   return 0;
}
// ----------------------------------------------------------------

void keysUpdate( void )
{
   // Comenzar a enviar datos
   if( !gpioRead(TEC1) ) {   
      gpioWrite( LEDB, ON );
      delay(500);
      gpioWrite( LEDB, OFF );
      axisStartToSendData();
   }
   // Parar envio de datos y reiniciar
   if( !gpioRead(TEC2) ) {
      gpioWrite( LED1, ON );
      delay(500);
      gpioWrite( LED1, OFF );
      axisStopSendingDataAndReset();
   }

   // Comenzar a recibir datos
   if( !gpioRead(TEC3) ) {    
      gpioWrite( LED2, ON );
      delay(500);
      gpioWrite( LED2, OFF );
      axisStartToReceiveData();
   }
   // Parar recepcion de datos y reiniciar
   if( !gpioRead(TEC4) ) {
      gpioWrite( LED3, ON );
      delay(500);
      gpioWrite( LED3, OFF );
      axisStopReceiveDataAndReset();
   }
}