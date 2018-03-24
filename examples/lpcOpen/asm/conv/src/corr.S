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
	.syntax unified

	.text
	.global	conv_asm
	.thumb_func

#define x  r0
#define nx r1
#define h  r2
#define nh r3
#define y  r4
#define i  r5
#define j  r6

conv_asm:
	push {r1-r11,lr}

	ldm r0,{r0-r4}

	/*
	for(i=0; i<(nx+nh); i++)
		y[i] = 0;
	*/
	add r5,nx,nh /* r5 = nx + nh */
	eor r6,r6
	eor r7,r7
loop_zero:
	str r7,[y, r6, lsl 2]
	add r6, 1
	cmp r6,r5
	bne loop_zero

/* lazo principal
	for(i=0; i<nx; i++)
	{
		for(j=0; j<nh; j++)
		{
			y[i+j] += h[j] * x[i];
		}
	}
*/
	eor i,i
loop_i:
	eor j,j
loop_j:
	/* r7 = h[j] */
	ldr r7,[h, j, lsl 2]
	/* r8 = x[i] */
	ldr r8,[x, i, lsl 2]

	/* r9 = i+j */
	add r9, i, j

	/* r10 = y[i+j] */
	ldr r10,[y, r9, lsl 2]

	/* r11 = 0 */
	eor r11, r11

	/* r10:r11 += r7 * r8 */
	/* y[i+j] += h[j] * x[i]; */
	smlal r11,r10,r7,r8

	str r10,[y, r9, lsl 2]

	add j, 1

	cmp j,nh
	bne loop_j

	add i, 1

	cmp i,nx
	bne loop_i

	pop {r1-r11,pc}

	.end
