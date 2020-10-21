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

#include "mcp23017.h"
#include "sapi.h"

bool_t 	MCP23017_init(mcp23017_t *mcp, mcpSpeed_t speed){
	bool_t state_init;
	uint8_t init_pins[13] = {MCP23017_IODIRA, // sub address IODIRA + IODIRB
			0xFF, // inicia pines como entradas
			0xFF, // inicia pines como entradas
			0x00,
			0x00,
			0x00,
			0x00,
			0x00,
			0x00,
			0x00,
			0x00,
			0x00,
			0x00
	};

	mcp->DIRportA = 0xFF;
	mcp->DIRportB = 0xFF;
	mcp->PUportA = 0x00;
	mcp->PUportB = 0x00;

	i2cInit( I2C0 , speed );

	state_init = i2cWrite(I2C0 , mcp->address, init_pins, 13, TRUE);

	return state_init;
}

// Funciones orientadas a pines
bool_t 	MCP23017_pinMode(mcp23017_t *mcp, mcpPinMap_t pin, mcpPinMode_t mode){
	bool_t state;
	uint8_t buffer1[2];
	uint8_t buffer2[2];

	switch(mode){
	case OUTPUT:
		if(pin < GPB0){ // port A
			mcp->DIRportA &= 0 << pin;
			mcp->PUportA &= 0 << pin;
		}
		else{			// port B
			mcp->DIRportB &= 0 << (pin-GPB0);
			mcp->PUportB &= 0 << (pin-GPB0);
		}
		break;
	case INPUT:
		if(pin < GPB0){ // port A
			mcp->DIRportA |= 1 << pin;
			mcp->PUportA &= 0 << pin;
		}
		else{			// port B
			mcp->DIRportB |= 1 << (pin-GPB0);
			mcp->PUportB &= 0 << (pin-GPB0);
		}
		break;
	case INPUT_PULLUP:
		if(pin < GPB0){ // port A
			mcp->DIRportA |= 1 << pin;
			mcp->PUportA |= 1 << pin;
		}
		else{			// port B
			mcp->DIRportB |= 1 << (pin-GPB0);
			mcp->PUportB |= 1 << (pin-GPB0);
		}
		break;
	}

	if(pin < GPB0){ // port A
		buffer1[0] = MCP23017_IODIRA;
		buffer1[1] = mcp->DIRportA;
		buffer2[0] = MCP23017_GPPUA;
		buffer2[1] = mcp->PUportA;
	}
	else{			// port B
		buffer1[0] = MCP23017_IODIRB;
		buffer1[1] = mcp->DIRportB;
		buffer2[0] = MCP23017_GPPUB;
		buffer2[1] = mcp->PUportB;
	}

	state = i2cWrite(I2C0 , mcp->address, buffer1, 2, TRUE);
	state &= i2cWrite(I2C0 , mcp->address, buffer2, 2, TRUE);

	return state;
}

bool_t 	MCP23017_pinWrite(mcp23017_t *mcp, mcpPinMap_t pin, bool_t pinState){
	bool_t state;

	if(pin < GPB0){ // port A
		mcp->STAportA = pinState == ON ? (mcp->STAportA |= 1 << pin) : (mcp->STAportA &= ~(1 << pin));
		state = MCP23017_portWrite(mcp, PORTA, mcp->STAportA);
	}
	else{			// port B
		mcp->STAportB = pinState == ON ? (mcp->STAportB |= 1 << (pin - GPB0)) : (mcp->STAportB &= ~(1 << (pin - GPB0) ));
		state = MCP23017_portWrite(mcp, PORTB, mcp->STAportB);
	}
	return state;
}

bool_t 	MCP23017_pinRead(mcp23017_t *mcp, mcpPinMap_t pin){
	bool_t state;
	uint8_t buffer1, subaddress;

	if(pin < GPB0) // port A
		state = ( MCP23017_portRead(mcp, PORTA) & 1 << pin );
	else			// port B
		state = ( MCP23017_portRead(mcp, PORTB) & 1 << (pin - GPB0) );
	return state;
}

// Funciones orientadas a puertos
// Configura el puerto como entrada o salida.
bool_t 	MCP23017_portMode(mcp23017_t *mcp, mcpPortMap_t port, uint8_t mode){
	bool_t state;
	uint8_t buffer[2];
	switch(port){
	case PORTA:
		buffer[0] = MCP23017_IODIRA;
		mcp->DIRportA = mode;
		buffer[1] = mcp->DIRportA;
		break;
	case PORTB:
		buffer[0] = MCP23017_IODIRB;
		mcp->DIRportB = mode;
		buffer[1] = mcp->DIRportB;
		break;
	}

	state = i2cWrite(I2C0 , mcp->address, buffer, 2, TRUE);

	return state;
}

bool_t 	MCP23017_portWrite(mcp23017_t *mcp, mcpPortMap_t port, uint8_t state_port){
	bool_t state;
	uint8_t buffer[2];

	buffer[1] = state_port;

	switch(port){
	case PORTA:
		buffer[0] = MCP23017_OLATA;
		state = i2cWrite(I2C0 , mcp->address, buffer, 2, TRUE);
		break;
	case PORTB:
		buffer[0] = MCP23017_OLATB;
		state = i2cWrite(I2C0 , mcp->address, buffer, 2, TRUE);
		break;
	}
	return state;
}

uint8_t MCP23017_portRead(mcp23017_t *mcp, mcpPortMap_t port){
	bool_t state;
	uint8_t buffer, subaddress;

	switch(port){
	case PORTA:
		subaddress = MCP23017_GPIOA;
		i2cWriteRead(I2C0, mcp->address, &subaddress, 1, TRUE, &buffer, 1, TRUE);
		break;
	case PORTB:
		subaddress = MCP23017_GPIOB;
		i2cWriteRead(I2C0, mcp->address, &subaddress, 1, TRUE, &buffer, 1, TRUE);
		break;
	}
	return buffer;
}
