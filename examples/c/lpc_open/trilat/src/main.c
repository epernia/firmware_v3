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
 
/** @brief trilateration algorithm using CMSIS-DSPLIB
 *
 */

/*==================[inclusions]=============================================*/

#include "board.h"
#include "arm_math.h"

/*==================[macros and definitions]=================================*/

typedef struct _p3d
{
	float32_t x;
	float32_t y;
	float32_t z;
}p3d;

/*==================[internal data declaration]==============================*/

/*==================[internal functions declaration]=========================*/

/** @brief hardware initialization function
 *	@return none
 */
static void initHardware(void);

/*==================[internal data definition]===============================*/

/*==================[external data definition]===============================*/

/*==================[internal functions definition]==========================*/

static void initHardware(void)
{
	Board_Init();
	SystemCoreClockUpdate();
}

static float32_t dist3d(p3d p1, p3d p2)
{
	p3d r = {p1.x-p2.x, p1.y-p2.y, p1.z-p2.z};

	r.x *= r.x;
	r.y *= r.y;
	r.z *= r.z;

	r.x += r.y + r.z;

	arm_sqrt_f32(r.x, &(r.x));

	return r.x;
}

static p3d trilaterate(p3d * ref, float32_t * r, uint8_t N)
{
	float32_t distref[N-1];
	float32_t aData[(N-1)*3];
	float32_t atData[(N-1)*3];
	float32_t bData[(N-1)*1];

	arm_status s;
	uint8_t i;

	//calculo distancias desde referencia n (n!=0) a referencia 0
	for(i=0; i<N-1; i++)
		distref[i] = dist3d(ref[i+1], ref[0]);

	//coeficientes de la matriz A
	for(i=0; i<N-1; i++)
	{
		aData[3*i]   = ref[i+1].x - ref[0].x;
		aData[3*i+1] = ref[i+1].y - ref[0].y;
		aData[3*i+2] = ref[i+1].z - ref[0].z;
	}

	//matriz A
	arm_matrix_instance_f32 matA = {N-1, 3, aData};

	//matriz A traspuesta
	arm_matrix_instance_f32 matAT = {3, N-1, atData};

	//coeficientes de la matriz B
	for(i=0; i<N-1; i++)
		bData[i] = (r[0]*r[0] - r[i+1]*r[i+1] + distref[i]*distref[i])/2;

	//matriz B
	arm_matrix_instance_f32 matB = {N-1, 1, bData};

	s = arm_mat_trans_f32(&matA, &matAT);

	//matriz auxiliar 1
	float32_t aux1data[3*3];
	arm_matrix_instance_f32 mataux1 = {3, 3, aux1data};

	//(A'*A) -> aux1
	s = arm_mat_mult_f32(&matAT, &matA, &mataux1);

	//aux1^-1 -> inv
	float32_t auxinvdata[3*3];
	arm_matrix_instance_f32 matinv = {3, 3, auxinvdata};

	s = arm_mat_inverse_f32(&mataux1, &matinv);

	//matriz auxiliar 2
	float32_t aux2data[3*(N-1)];
	arm_matrix_instance_f32 mataux2 = {3, N-1, aux2data};

	//inv*A' -> aux2
	s = arm_mat_mult_f32(&matinv, &matAT, &mataux2);

	//aux2*B -> aux1
	s = arm_mat_mult_f32(&mataux2, &matB, &mataux1);

	p3d res = {aux1data[0]+ref[0].x,aux1data[1]+ref[0].y,aux1data[2]+ref[0].z};

	return res;
}

/*==================[external functions definition]==========================*/

int main(void)
{
	initHardware();

	p3d p = {3,-1,4};
	p3d referencias[] =
	{
			{0,0,0},
			{3,0,0},
			{0,3,0},
			{0,0,3},
			{3,3,3},//5
			{3,3,0},
			{3,0,3},
			{0,3,3},
			{6,0,0},
			{0,6,0},//10
			{0,0,6},
			{6,6,6},
			{6,6,0},
			{6,0,6},
			{0,6,6} //15
	};

	//distancias medidas, agrego error
	float32_t distancias[] =
	{
			dist3d(referencias[0], p)+1,
			dist3d(referencias[1], p)-1,
			dist3d(referencias[2], p)-5,
			dist3d(referencias[3], p)-2,
			dist3d(referencias[4], p)-15,
			dist3d(referencias[5], p)+2,
			dist3d(referencias[6], p)+9,
			dist3d(referencias[7], p)+1,
			//		dist3d(referencias[8], p),
			//		dist3d(referencias[9], p),
			//		dist3d(referencias[10], p),
			//		dist3d(referencias[11], p),
			//		dist3d(referencias[12], p),
			//		dist3d(referencias[13], p),
			//		dist3d(referencias[14], p),
	};


	p = trilaterate(referencias, distancias, 8);


	return 0 ;
}

/** @} doxygen end group definition */

/*==================[end of file]============================================*/
