#ifndef NEWLIB_STUBS_H_8QSRQHO3
#define NEWLIB_STUBS_H_8QSRQHO3

#include <errno.h>
#include <sys/stat.h>
#include <sys/times.h>
#include <sys/unistd.h>
#include "chip.h"

#ifndef STDOUT_UART
#define STDOUT_UART 2
#endif

#ifndef STDERR_UART
#define STDERR_UART 2
#endif

#ifndef STDIN_UART
#define STDIN_UART 2
#endif

#ifdef lpc4337_m4
#define LPC_UART2 LPC_USART2
#endif

#ifdef lpc54102_m4
#define LPC_UART2 LPC_USART2
#endif

#ifdef lpc11u68
#define LPC_UART2 LPC_USART2
#endif

#if STDIN_UART == 1
	 #define MYSTDIN UART1
#elif STDIN_UART == 2
	 #define MYSTDIN LPC_UART2
#elif STDIN_UART == 3
	 #define MYSTDIN UART3
#else
	 #error "You defined STDIN_UART wrong!"
#endif

#if STDOUT_UART == 1
	 #define MYSTDOUT UART1
#elif STDOUT_UART == 2
	 #define MYSTDOUT LPC_UART2
#elif STDOUT_UART == 3
	 #define MYSTDOUT UART3
#else
	 #error "You defined STDOUT_UART wrong!"
#endif

#if STDERR_UART == 1
	 #define MYSTDERR UART1
#elif STDERR_UART == 2
	 #define MYSTDERR LPC_UART2
#elif STDERR_UART == 3
	 #define MYSTDERR UART3
#else
	 #error "You defined STDERR_UART wrong!"
#endif



/*
 environ
 A pointer to a list of environment variables and their values.
 For a minimal environment, this empty list is adequate:
 *
char *__env[1] = { 0 };
char **environ = __env;
*/

int _close(int file);
void _exit(int status);
int _fstat(int file, struct stat *st);
int _isatty(int file);
int _link(char *old, char *new);
int _lseek(int file, int ptr, int dir);
int _read(int file, char *ptr, int len);
caddr_t _sbrk(int incr);
int _stat(const char *filepath, struct stat *st);
clock_t _times(struct tms *buf);
int _unlink(char *name);
int _wait(int *status);
int _write(int file, char *ptr, int len);

#endif /* end of include guard: NEWLIB_STUBS_H_8QSRQHO3 */
