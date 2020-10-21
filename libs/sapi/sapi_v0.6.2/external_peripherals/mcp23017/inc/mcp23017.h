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

/*=====[Avoid multiple inclusion - begin]====================================*/

#ifndef __MCP23017_H__
#define __MCP23017_H__

/*=====[Inclusions of public function dependencies]==========================*/
#include "sapi_i2c.h"

/*=====[C++ - begin]=========================================================*/

#ifdef __cplusplus
extern "C" {
#endif

/*=====[Definition macros of public constants]===============================*/

// Direcciones de los registros del MCP23017
// Direcciones asociadas al puerto A
#define MCP23017_IODIRA 	0x00	// I/O direction register A
#define MCP23017_IPOLA 		0x02	// Input polarity port register A
#define MCP23017_GPINTENA 	0x04	// Interrupt-on-change pins A
#define MCP23017_DEFVALA 	0x06	// Default value register A
#define MCP23017_INTCONA 	0x08	// Interrupt-on-change control register A
#define MCP23017_IOCONA 	0x0A	// I/O expander configuration register A
#define MCP23017_GPPUA 		0x0C	// GPIO pull-up resistor register A
#define MCP23017_INTFA 		0x0E	// Interrupt flag register A
#define MCP23017_INTCAPA 	0x10	// Interrupt captured value for port register A
#define MCP23017_GPIOA 		0x12	// General purpose I/O port register A
#define MCP23017_OLATA 		0x14	// Output latch register 0 A

// Direcciones asociadas al puerto B
#define MCP23017_IODIRB 	0x01	// I/O direction register B
#define MCP23017_IPOLB 		0x03	// Input polarity port register B
#define MCP23017_GPINTENB 	0x05	// Interrupt-on-change pins B
#define MCP23017_DEFVALB 	0x07	// Default value register B
#define MCP23017_INTCONB 	0x09	// Interrupt-on-change control register B
#define MCP23017_IOCONB 	0x0B	// I/O expander configuration register B
#define MCP23017_GPPUB 		0x0D	// GPIO pull-up resistor register B
#define MCP23017_INTFB 		0x0F	// Interrupt flag register B
#define MCP23017_INTCAPB 	0x11	// Interrupt captured value for port register B
#define MCP23017_GPIOB 		0x13	// General purpose I/O port register B
#define MCP23017_OLATB 		0x15	// Output latch register 0 B

//Direcciones posibles para el MCP23017
//							pines  A2 | A1 | A0
#define MCP_ADDRESS0 0x20	//		0 |  0 |  0
#define MCP_ADDRESS1 0x21	//		0 |  0 |  1
#define MCP_ADDRESS2 0x22	//		0 |  1 |  0
#define MCP_ADDRESS3 0x23	//		0 |  1 |  1
#define MCP_ADDRESS4 0x24	//		1 |  0 |  0
#define MCP_ADDRESS5 0x25	//		1 |  0 |  1
#define MCP_ADDRESS6 0x26	//		1 |  1 |  0
#define MCP_ADDRESS7 0x27	//		1 |  1 |  1

/*=====[Public function-like macros]=========================================*/

/*=====[Definitions of public data types]====================================*/

typedef enum{
	PORTA,
	PORTB
}mcpPortMap_t;

typedef enum{
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
}mcpPinMap_t;

typedef enum{
	OUTPUT,
	INPUT,
	INPUT_PULLUP
}mcpPinMode_t;

typedef enum{
	I2C_100KHZ = 100000,	// Testeado en la edu-CIAA
	I2C_400KHZ = 400000,	// Testeado en la edu-CIAA
	I2C_1700KHZ = 1700000
}mcpSpeed_t;

typedef struct{
	uint8_t address;
	uint8_t STAportA;
	uint8_t STAportB;
	uint8_t DIRportA;
	uint8_t DIRportB;
	uint8_t PUportA;
	uint8_t PUportB;
}mcp23017_t;

/*=====[Prototypes (declarations) of public functions]=======================*/
bool_t 	MCP23017_init(mcp23017_t *mcp, mcpSpeed_t speed);

// Funciones orientadas a pines
bool_t 	MCP23017_pinMode(mcp23017_t *mcp, mcpPinMap_t pin, mcpPinMode_t mode);
bool_t 	MCP23017_pinWrite(mcp23017_t *mcp, mcpPinMap_t pin, bool_t state);
bool_t 	MCP23017_pinRead(mcp23017_t *mcp, mcpPinMap_t pin);

// Funciones orientadas a puertos
bool_t 	MCP23017_portMode(mcp23017_t *mcp, mcpPortMap_t port, uint8_t mode);
bool_t 	MCP23017_portWrite(mcp23017_t *mcp, mcpPortMap_t port, uint8_t state);
uint8_t MCP23017_portRead(mcp23017_t *mcp, mcpPortMap_t port);


/*=====[C++ - end]===========================================================*/

#ifdef __cplusplus
}
#endif

/*=====[Avoid multiple inclusion - end]======================================*/

#endif /* __MCP23017_H__ */
