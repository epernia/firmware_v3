#include <stdio.h>
#include <stdbool.h>

#include "minut.h"

int add(int a, int b)
{
   return 0;   // Code with error
   //return a + b;   // Correct code   
}

int main(void)
{
   MINUT(true);
   int a = add(1, 2);
   printf("a = %d\r\n", a);
   return 0;
}

TEST(add_1)
{
   int num_a = 2;
   int num_b = 3;

   ASSERT_EQ(
      5,
      add(num_a, num_b)
   );
}

TEST(add_2)
{
   int num_a = -3;
   int num_b = 3;

   ASSERT_EQ(
      0,
      add(num_a, num_b)
   );
}

MINUT_BEG
RUN(add_1());
RUN(add_2());
MINUT_END
