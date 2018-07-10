/* Copyright 2016, Pablo Ridolfi
 * All rights reserved.
 *
 * This file is part of Workspace.
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
 */

#include "board.h"

#include "math.h"
#include "ciaaUART.h"
#include <stdio.h>

volatile uint32_t * DWT_CTRL   = (uint32_t *)0xE0001000;
volatile uint32_t * DWT_CYCCNT = (uint32_t *)0xE0001004;

typedef struct
{
	const int * x;
	int nx;
	const int * h;
	int nh;
	int * y;
}convData_t;

extern void conv_asm(convData_t * datos);

int y[612];
int y_c[612];

const int x1k[300] = {
		0	,
		4616	,
		8858	,
		12382	,
		14903	,
		16217	,
		16217	,
		14903	,
		12382	,
		8858	,
		4616	,
		0	,
		-4616	,
		-8858	,
		-12382	,
		-14903	,
		-16217	,
		-16217	,
		-14903	,
		-12382	,
		-8858	,
		-4616	,
		0	,
		4616	,
		8858	,
		12382	,
		14903	,
		16217	,
		16217	,
		14903	,
		12382	,
		8858	,
		4616	,
		0	,
		-4616	,
		-8858	,
		-12382	,
		-14903	,
		-16217	,
		-16217	,
		-14903	,
		-12382	,
		-8858	,
		-4616	,
		0	,
		4616	,
		8858	,
		12382	,
		14903	,
		16217	,
		16217	,
		14903	,
		12382	,
		8858	,
		4616	,
		0	,
		-4616	,
		-8858	,
		-12382	,
		-14903	,
		-16217	,
		-16217	,
		-14903	,
		-12382	,
		-8858	,
		-4616	,
		0	,
		4616	,
		8858	,
		12382	,
		14903	,
		16217	,
		16217	,
		14903	,
		12382	,
		8858	,
		4616	,
		0	,
		-4616	,
		-8858	,
		-12382	,
		-14903	,
		-16217	,
		-16217	,
		-14903	,
		-12382	,
		-8858	,
		-4616	,
		0	,
		4616	,
		8858	,
		12382	,
		14903	,
		16217	,
		16217	,
		14903	,
		12382	,
		8858	,
		4616	,
		0	,
		-4616	,
		-8858	,
		-12382	,
		-14903	,
		-16217	,
		-16217	,
		-14903	,
		-12382	,
		-8858	,
		-4616	,
		0	,
		4616	,
		8858	,
		12382	,
		14903	,
		16217	,
		16217	,
		14903	,
		12382	,
		8858	,
		4616	,
		0	,
		-4616	,
		-8858	,
		-12382	,
		-14903	,
		-16217	,
		-16217	,
		-14903	,
		-12382	,
		-8858	,
		-4616	,
		0	,
		4616	,
		8858	,
		12382	,
		14903	,
		16217	,
		16217	,
		14903	,
		12382	,
		8858	,
		4616	,
		0	,
		-4616	,
		-8858	,
		-12382	,
		-14903	,
		-16217	,
		-16217	,
		-14903	,
		-12382	,
		-8858	,
		-4616	,
		0	,
		4616	,
		8858	,
		12382	,
		14903	,
		16217	,
		16217	,
		14903	,
		12382	,
		8858	,
		4616	,
		0	,
		-4616	,
		-8858	,
		-12382	,
		-14903	,
		-16217	,
		-16217	,
		-14903	,
		-12382	,
		-8858	,
		-4616	,
		0	,
		4616	,
		8858	,
		12382	,
		14903	,
		16217	,
		16217	,
		14903	,
		12382	,
		8858	,
		4616	,
		0	,
		-4616	,
		-8858	,
		-12382	,
		-14903	,
		-16217	,
		-16217	,
		-14903	,
		-12382	,
		-8858	,
		-4616	,
		0	,
		4616	,
		8858	,
		12382	,
		14903	,
		16217	,
		16217	,
		14903	,
		12382	,
		8858	,
		4616	,
		0	,
		-4616	,
		-8858	,
		-12382	,
		-14903	,
		-16217	,
		-16217	,
		-14903	,
		-12382	,
		-8858	,
		-4616	,
		0	,
		4616	,
		8858	,
		12382	,
		14903	,
		16217	,
		16217	,
		14903	,
		12382	,
		8858	,
		4616	,
		0	,
		-4616	,
		-8858	,
		-12382	,
		-14903	,
		-16217	,
		-16217	,
		-14903	,
		-12382	,
		-8858	,
		-4616	,
		0	,
		4616	,
		8858	,
		12382	,
		14903	,
		16217	,
		16217	,
		14903	,
		12382	,
		8858	,
		4616	,
		0	,
		-4616	,
		-8858	,
		-12382	,
		-14903	,
		-16217	,
		-16217	,
		-14903	,
		-12382	,
		-8858	,
		-4616	,
		0	,
		4616	,
		8858	,
		12382	,
		14903	,
		16217	,
		16217	,
		14903	,
		12382	,
		8858	,
		4616	,
		0	,
		-4616	,
		-8858	,
		-12382	,
		-14903	,
		-16217	,
		-16217	,
		-14903	,
		-12382	,
		-8858	,
		-4616	,
		0	,
		4616	,
		8858	,
		12382	,
		14903	,
		16217	,
		16217	,
		14903	,
		12382	,
		8858	,
		4616	,
		0	,
		-4616	,
		-8858
};

const int x3k[300] = {
		0	,
		24764	,
		32434	,
		17716	,
		-9232	,
		-29807	,
		-29807	,
		-9232	,
		17716	,
		32434	,
		24764	,
		0	,
		-24764	,
		-32434	,
		-17716	,
		9232	,
		29807	,
		29807	,
		9232	,
		-17716	,
		-32434	,
		-24764	,
		0	,
		24764	,
		32434	,
		17716	,
		-9232	,
		-29807	,
		-29807	,
		-9232	,
		17716	,
		32434	,
		24764	,
		0	,
		-24764	,
		-32434	,
		-17716	,
		9232	,
		29807	,
		29807	,
		9232	,
		-17716	,
		-32434	,
		-24764	,
		0	,
		24764	,
		32434	,
		17716	,
		-9232	,
		-29807	,
		-29807	,
		-9232	,
		17716	,
		32434	,
		24764	,
		0	,
		-24764	,
		-32434	,
		-17716	,
		9232	,
		29807	,
		29807	,
		9232	,
		-17716	,
		-32434	,
		-24764	,
		0	,
		24764	,
		32434	,
		17716	,
		-9232	,
		-29807	,
		-29807	,
		-9232	,
		17716	,
		32434	,
		24764	,
		0	,
		-24764	,
		-32434	,
		-17716	,
		9232	,
		29807	,
		29807	,
		9232	,
		-17716	,
		-32434	,
		-24764	,
		0	,
		24764	,
		32434	,
		17716	,
		-9232	,
		-29807	,
		-29807	,
		-9232	,
		17716	,
		32434	,
		24764	,
		0	,
		-24764	,
		-32434	,
		-17716	,
		9232	,
		29807	,
		29807	,
		9232	,
		-17716	,
		-32434	,
		-24764	,
		0	,
		24764	,
		32434	,
		17716	,
		-9232	,
		-29807	,
		-29807	,
		-9232	,
		17716	,
		32434	,
		24764	,
		0	,
		-24764	,
		-32434	,
		-17716	,
		9232	,
		29807	,
		29807	,
		9232	,
		-17716	,
		-32434	,
		-24764	,
		0	,
		24764	,
		32434	,
		17716	,
		-9232	,
		-29807	,
		-29807	,
		-9232	,
		17716	,
		32434	,
		24764	,
		0	,
		-24764	,
		-32434	,
		-17716	,
		9232	,
		29807	,
		29807	,
		9232	,
		-17716	,
		-32434	,
		-24764	,
		0	,
		24764	,
		32434	,
		17716	,
		-9232	,
		-29807	,
		-29807	,
		-9232	,
		17716	,
		32434	,
		24764	,
		0	,
		-24764	,
		-32434	,
		-17716	,
		9232	,
		29807	,
		29807	,
		9232	,
		-17716	,
		-32434	,
		-24764	,
		0	,
		24764	,
		32434	,
		17716	,
		-9232	,
		-29807	,
		-29807	,
		-9232	,
		17716	,
		32434	,
		24764	,
		0	,
		-24764	,
		-32434	,
		-17716	,
		9232	,
		29807	,
		29807	,
		9232	,
		-17716	,
		-32434	,
		-24764	,
		0	,
		24764	,
		32434	,
		17716	,
		-9232	,
		-29807	,
		-29807	,
		-9232	,
		17716	,
		32434	,
		24764	,
		0	,
		-24764	,
		-32434	,
		-17716	,
		9232	,
		29807	,
		29807	,
		9232	,
		-17716	,
		-32434	,
		-24764	,
		0	,
		24764	,
		32434	,
		17716	,
		-9232	,
		-29807	,
		-29807	,
		-9232	,
		17716	,
		32434	,
		24764	,
		0	,
		-24764	,
		-32434	,
		-17716	,
		9232	,
		29807	,
		29807	,
		9232	,
		-17716	,
		-32434	,
		-24764	,
		0	,
		24764	,
		32434	,
		17716	,
		-9232	,
		-29807	,
		-29807	,
		-9232	,
		17716	,
		32434	,
		24764	,
		0	,
		-24764	,
		-32434	,
		-17716	,
		9232	,
		29807	,
		29807	,
		9232	,
		-17716	,
		-32434	,
		-24764	,
		0	,
		24764	,
		32434	,
		17716	,
		-9232	,
		-29807	,
		-29807	,
		-9232	,
		17716	,
		32434	,
		24764	,
		0	,
		-24764	,
		-32434	,
		-17716	,
		9232	,
		29807	,
		29807	,
		9232	,
		-17716	,
		-32434	,
		-24764	,
		0	,
		24764	,
		32434	,
		17716	,
		-9232	,
		-29807	,
		-29807	,
		-9232	,
		17716	,
		32434	,
		24764	,
		0	,
		-24764	,
		-32434	,
};

/*

FIR filter designed with
 http://t-filter.appspot.com

sampling frequency: 22000 Hz

fixed point precision: 31 bits

* 0 Hz - 1500 Hz
  gain = 1
  desired ripple = 5 dB
  actual ripple = n/a

* 2000 Hz - 11000 Hz
  gain = 0
  desired attenuation = -40 dB
  actual attenuation = n/a

*/
const int h[49] = {
		-8858824,
		  6003323,
		  12563243,
		  22554988,
		  34445997,
		  46183080,
		  55268898,
		  59164851,
		  55788691,
		  44095689,
		  24536209,
		  -703795,
		  -27799499,
		  -51730180,
		  -67033220,
		  -68753208,
		  -53419173,
		  -19871629,
		  30257820,
		  92516531,
		  160137980,
		  224962013,
		  278706724,
		  314171785,
		  326566858,
		  314171785,
		  278706724,
		  224962013,
		  160137980,
		  92516531,
		  30257820,
		  -19871629,
		  -53419173,
		  -68753208,
		  -67033220,
		  -51730180,
		  -27799499,
		  -703795,
		  24536209,
		  44095689,
		  55788691,
		  59164851,
		  55268898,
		  46183080,
		  34445997,
		  22554988,
		  12563243,
		  6003323,
		  -8858824
};

void conv_c(convData_t * datos)
{
	int i,j;
	long long acum;

	for(i=0; i<(datos->nx+datos->nh); i++)
		datos->y[i] = 0;

	for(i=0; i<datos->nx; i++)
	{
		acum = 0;
		for(j=0; j<datos->nh; j++)
		{
			acum = (long long)datos->y[i+j] << 32;
			acum += (long long)datos->h[j] * (long long)datos->x[i];
			datos->y[i+j] = acum >> 32;
		}

	}
}


int main(void)
{
	convData_t datos;

	Board_Init();
	SystemCoreClockUpdate();
	ciaaUARTInit();

	datos.x = x3k;
	datos.nx = 300;
	datos.h = h;
	datos.nh = 49;

	*DWT_CTRL |= 1;


	datos.y = y_c;
	*DWT_CYCCNT = 0;
	conv_c(&datos);
	printf("conv_c 3k: %lu ciclos\r\n", *DWT_CYCCNT);


	datos.y = y;
	*DWT_CYCCNT = 0;
	conv_asm(&datos);
	printf("conv_asm 3k: %lu ciclos\r\n", *DWT_CYCCNT);

	int i;

	printf("i,y[i],y_c[i]\r\n");
	for(i=0; i<349; i++)
	{
		printf("%d,%d,%d\r\n", i, y[i], y_c[i]);
	}

	datos.x = x1k;
	datos.y = y_c;
	*DWT_CYCCNT = 0;
	conv_c(&datos);
	printf("conv_c 1k: %lu ciclos\r\n", *DWT_CYCCNT);


	datos.y = y;
	*DWT_CYCCNT = 0;
	conv_asm(&datos);
	printf("conv_asm 1k: %lu ciclos\r\n", *DWT_CYCCNT);

	printf("i,y[i],y_c[i]\r\n");
	for(i=0; i<349; i++)
	{
		printf("%d,%d,%d\r\n", i, y[i], y_c[i]);
	}

	return 0;
}

