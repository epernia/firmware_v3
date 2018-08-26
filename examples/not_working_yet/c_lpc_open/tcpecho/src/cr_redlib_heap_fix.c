#ifndef STACK_SIZE
#define STACK_SIZE  (0x400)
#endif

/* Top of stack that comes from linker script */
extern void _vStackTop(void);

// *****************************************************************************
//
// Heap overflow check function required by REDLib_V2 library
// Without the fix Redlib's malloc calls from inside freeRtos tasks
// will always return NULL.
//
// *****************************************************************************
extern unsigned int *_pvHeapStart;
unsigned int __check_heap_overflow (void * new_end_of_heap)
{
	unsigned long stackend = ((unsigned long) &_vStackTop) - STACK_SIZE;
	return ((unsigned long)new_end_of_heap >= stackend);
}
