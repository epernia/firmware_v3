/*=============================================================================
 * Copyright (c) 2020, Francisco Timez <franciscotimez@gmail.com>
 * All rights reserved.
 * Date: 2020/09/26
 *
 * This file is part sAPI library for microcontrollers.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * 3. Neither the name of the copyright holder nor the names of its
 *    contributors may be used to endorse or promote products derived from this
 *    software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 *===========================================================================*/

/*=====[Inclusions of function dependencies]=================================*/

#include "sapi.h"

//INICIO del testeo (Eliminar para convertir el archivo en una libreria)

// Tipo de datos que contiene una copia de registros importantes del Expansor
mcp23017_t expander1;

// Arreglo de pines utilizado en el ejemplo de testeo.
mcpPinMap_t PINS[16] = {
		GPA0,
		GPA1,
		GPA2,
		GPA3,
		GPA4,
		GPA5,
		GPA6,
		GPA7,
		GPB0,
		GPB1,
		GPB2,
		GPB3,
		GPB4,
		GPB5,
		GPB6,
		GPB7
};

/*=====[Main function, program entry point after power on or reset]==========*/

int main( void )
{
	// ----- Setup -----------------------------------
	boardInit();

	// Configuro la direccion del expansor. (ver MCP23017.h)
	expander1.address = MCP_ADDRESS1;

	// Inicializo el mcp23017
	MCP23017_init(&expander1, I2C_100KHZ);

	MCP23017_portMode(&expander1, PORTA, 0x00);	// Puerto A como salida

	MCP23017_portMode(&expander1, PORTB, 0x00);	// Puerto B como salida

	MCP23017_pinMode(&expander1, GPB0, INPUT);	// Pines como entrada
	MCP23017_pinMode(&expander1, GPB1, INPUT);
	MCP23017_pinMode(&expander1, GPB2, INPUT);
	MCP23017_pinMode(&expander1, GPB3, INPUT);

	delay(1);

	// ----- Repeat for ever -------------------------
	while( true ) {
		gpioToggle(LEDB);
		for(int i = 0 ; i < 8 ; i++){
			MCP23017_pinWrite(&expander1, PINS[i], ON);	// Enciendo los pines del puerto A 0 al 7

			if(MCP23017_pinRead(&expander1, GPB0))		// Si esta en ALTO el PIN 0  PUERTO B
				MCP23017_pinWrite(&expander1, GPB4, ON);	// pongo el alto el PIN 4
			else
				MCP23017_pinWrite(&expander1, GPB4, OFF);

			if(MCP23017_pinRead(&expander1, GPB1))		// Si esta en ALTO el PIN 1  PUERTO B
				MCP23017_pinWrite(&expander1, GPB5, ON);	// pongo el alto el PIN 5
			else
				MCP23017_pinWrite(&expander1, GPB5, OFF);

			if(MCP23017_pinRead(&expander1, GPB2))		// Si esta en ALTO el PIN 2  PUERTO B
				MCP23017_pinWrite(&expander1, GPB6, ON);	// pongo el alto el PIN 6
			else
				MCP23017_pinWrite(&expander1, GPB6, OFF);

			if(MCP23017_pinRead(&expander1, GPB3))		// Si esta en ALTO el PIN 3  PUERTO B
				MCP23017_pinWrite(&expander1, GPB7, ON);	// pongo el alto el PIN 7
			else
				MCP23017_pinWrite(&expander1, GPB7, OFF);

			delay(300);
			MCP23017_pinWrite(&expander1, PINS[i], OFF); // Enciendo los pines del puerto A 0 al 7
			//delay(500);
		}

	}

	// YOU NEVER REACH HERE, because this program runs directly or on a
	// microcontroller and is not called by any Operating System, as in the
	// case of a PC program.
	return 0;
}
//FIN del testeo
