/*
 * fir_q31.S
 *
 *  Created on: 05/09/2013
 *      Author: Pablo Ridolfi
 */
	.syntax unified
/*
	int asm_fir_q31_get(fir_q31_t * f);

	r4: unsigned int last_index;
	r5: int num_taps;
	r6: const int * kernel;
	r7: int * history;
*/
	.global asm_fir_q31_get
	.thumb_func
asm_fir_q31_get:
	push {r1-r8,lr}

	/* cargo estructura fir_q31_t */
	ldm r0,{r4-r7}

	/* acumulador r1:r0 */
	eor r0,r0
	eor r1,r1

	/* r5: indice loop, cuenta hacia abajo
	   backupeo r5 en r8 para recargar r4
	*/
	mov r8,r5

asm_fir_loop:
	/*
	if(index == 0)
		index = num_taps-1;
	else
		index = index-1;
	*/
	cmp r4,0
	it eq
	moveq r4,r8
	sub r4,1

	/* r2: history[index] */
	ldr r2,[r7,r4,lsl 2]
	/* r3: kernel[i] */
	ldr r3,[r6],4

	/* multiply-accumulate
		r1:r0 += r2 * r3
	 */
	smlal r0,r1,r2,r3

	/* loop num_taps veces */
	subs r5,1
	bne asm_fir_loop

	/* r1:r0 >>= 31 */
	lsr r0,31
	lsl r1,1
	orr r0,r1

	pop {r1-r8,pc}

/*
uint32_t asm_usat10(int sample);
*/
	.global asm_usat10
	.thumb_func
asm_usat10:
	usat r0,10,r0
	bx lr

/*
	void asm_fir_q31_put(fir_q31_t * f, int input);

	r2: unsigned int last_index;
	r3: int num_taps;
	r4: const int * kernel;
	r5: int * history;
*/
	.global asm_fir_q31_put
	.thumb_func
asm_fir_q31_put:
	push {r2-r5}

	ldm r0,{r2-r5}

	/* f->history[f->last_index++] = input; */
	str r1,[r5,r2,lsl 2]
	add r2,1

	/* if(f->last_index == f->num_taps)
		f->last_index = 0; */
	cmp r2,r3
	it eq
	eoreq r2,r2

	str r2,[r0]

	pop {r2-r5}
	bx lr
