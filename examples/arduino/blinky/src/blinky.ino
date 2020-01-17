/*=============================================================================
 * Copyright (c) 2020, Eric Pernia <ericpernia@gmail.com>
 * All rights reserved.
 * License: bsd-3-clause
 * Date: 2020/01/17
 * Version: 1.0
 *===========================================================================*/

/*=====[Setup function, program entry point after power on or reset]=========*/

void setup()
{
   pinMode(LEDG, OUTPUT);
}

/*=====[loop function, program repeat for ever this function]================*/

void loop()
{
   digitalWrite(LEDG, HIGH);
   delay(250);
   digitalWrite(LEDG, LOW);
   delay(250);
}
