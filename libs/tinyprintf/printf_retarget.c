#include <unistd.h>

void _putchar(char character)
{
   write(1, &character, 1);
}
