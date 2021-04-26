/*=============================================================================
 * Copyright (c) 2021, Eric Pernia <ericpernia@gmail.com>
 * All rights reserved.
 * License: bsd-3-clause
 * Date: 2021/04/24
 * Version: 1.0
 *===========================================================================*/

/*=====[Setup function, program entry point after power on or reset]=========*/

void setup() {
   pinMode( LED1, OUTPUT );
   pinMode( TEC1, INPUT );
}

/*=====[loop function, program repeat for ever this function]================*/

void loop() {       
   // !(TEC1) --> LED1
   // Negado (!) porque el pulsador esta en 1 por defecto y al presionar da 0
   bool value = digitalRead( TEC1 );
   digitalWrite( LED1, !value );
}
