#include "fir_q31.h"

void fir_q31_init(fir_q31_t * f, int * history, const int * kernel, int num_taps)
{
	int i;

	f->num_taps = num_taps;
	f->history = history;
	f->kernel = kernel;

	for(i = 0; i < f->num_taps; ++i)
		f->history[i] = 0;
	f->last_index = 0;
}

void fir_q31_put(fir_q31_t * f, int input)
{
  f->history[f->last_index++] = input;
  if(f->last_index == f->num_taps)
    f->last_index = 0;
}

int fir_q31_get(fir_q31_t * f)
{
	long long acc = 0;
	int index = f->last_index, i;
	for(i = 0; i < f->num_taps; ++i)
	{
		index = index != 0 ? index-1 : f->num_taps-1;
		acc += (long long)f->history[index] * f->kernel[i];
	};
	return acc >> 31;
}


