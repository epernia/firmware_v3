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
	.global pack

	.thumb_func
pack:
	push {r3-r5,lr}

	mov r3,0
	mov r4,0

emp_loop:
	and r4,r0,7
	lsr r4,1

	ldrsh r3,[r0],2
	ssat r3,8,r3

	ldr r5,[r2]
	tbb [pc,r4]
emp_tabla:
	.byte (emp0-emp_tabla)/2
	.byte (emp1-emp_tabla)/2
	.byte (emp2-emp_tabla)/2
	.byte (emp3-emp_tabla)/2
emp0:
	bfi r5,r3,0,8
	str r5,[r2]
	b emp_loop_cont
emp1:
	bfi r5,r3,8,8
	str r5,[r2]
	b emp_loop_cont
emp2:
	bfi r5,r3,16,8
	str r5,[r2]
	b emp_loop_cont
emp3:
	bfi r5,r3,24,8
	str r5,[r2]

	add r2,4
emp_loop_cont:
	subs r1,1
	bne emp_loop

	pop {r3-r5,pc}

	.end
