#ifndef _FIR_H_
#define _FIR_H_

typedef struct
{
	unsigned int last_index;
	int num_taps;
	const int * kernel;
	int * history;
}fir_q31_t;

void fir_q31_init(fir_q31_t * f, int * history, const int * kernel, int num_taps);
void fir_q31_put(fir_q31_t * f, int input);
int fir_q31_get(fir_q31_t * f);

extern void asm_fir_q31_put(fir_q31_t * f, int input);
extern int 	asm_fir_q31_get(fir_q31_t * f);

#endif
