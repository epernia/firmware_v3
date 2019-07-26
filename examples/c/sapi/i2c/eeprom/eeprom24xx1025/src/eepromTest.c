/*=============================================================================
 * Copyright (c) 2019, Eric Pernia <ericpernia@gmail.com>
 * All rights reserved.
 * License: bsd-3-clause (see LICENSE file)
 * Date: 2019/06/26
 *===========================================================================*/

/*=====[Inclusions of function dependencies]=================================*/

#include "sapi.h"

/*=====[Definition macros of private constants]==============================*/

/*=====[Definitions of extern global variables]==============================*/

/*=====[Definitions of public global variables]==============================*/

/*=====[Definitions of private global variables]=============================*/

/*=====[Main function, program entry point after power on or reset]==========*/

int main( void )
{
   // ----- Setup -----------------------------------
   boardInit();

   // EEPROM Test - Performs a test on the 24xx1025 EEPROM memory.

   //                        0                             3132                            6364                            9596                          127
   uint8_t testBytes[128] = "EEPROM TEST #0123456789 --------abcdefghijklmnopqrstuvwxyz -----012345678901234567890123456789--deadbeef deadbeef deadbeef dead";

   // EEPROM Test Write individual bytes
   eeprom24xx1025TestWriteBytes( 0, testBytes, 24 );

   // EEPROM Test Write a hole page
   eeprom24xx1025TestWritePage( 1, testBytes, 128 );

   // EEPROM Test Sequential read
   eeprom24xx1025TestReadSequential( 128, testBytes, 128 );

   // EEPROM Test Current Address Read
   uint8_t read = 0;
   eeprom24xx1025TestReadCurrentAddress( 12, &read ); // address = 12 =>'#', Result => #0123456789

   // ----- Repeat for ever -------------------------
   while( true );

   // YOU NEVER REACH HERE, because this program runs directly or on a
   // microcontroller and is not called by any Operating System, as in the
   // case of a PC program.
   return 0;
}
