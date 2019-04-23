#include <stdio.h>
#include <stdbool.h>

#include "minut.h" // Tests lib
#include "sapi.h"  // sAPI lib

#define SCU_BASE                0x40086000
#define	SFSP2_10                0x128      // Pin P2_10 ==> GPIO0[14] ==> LED1

#define GPIO_BASE               0x400F4000
#define GPIO_DIR0_OFFSET        0x2000     // Direction register for GPIO Port 0

#define LPC4337_SFSP2_10_REG       (HW_REG_32_RW( SCU_BASE + SFSP2_10 ))
#define LPC4337_GPIO_PORT0_DIR_REG (HW_REG_32_RW( GPIO_BASE + GPIO_DIR0_OFFSET ))

int main(void)
{
   // Test preconditions
   boardInit();
   // Ok to run tests
   MINUT(true);
   return 0;
}

TEST( gpio_input_1 )
{
   pinInit( TEC1, GPIO_INPUT );
   bool_t pinValue1 = pinValueGet( TEC1 );
   ASSERT_EQ( pinValue1, ON );
}

TEST( gpio_input_2 )
{
   pinInit( TEC2, GPIO_INPUT );
   bool_t pinValue2 = pinValueGet( TEC2 );
   ASSERT_EQ( pinValue2, OFF );
}

TEST( gpio_output )
{
   LPC4337_SFSP2_10_REG = 0x0;
   LPC4337_GPIO_PORT0_DIR_REG = 0x0;

   pinInit( LED1, GPIO_OUTPUT );

   ASSERT_EQ( LPC4337_SFSP2_10_REG,       0xD0  ); // User Manual page 420
   ASSERT_EQ( LPC4337_GPIO_PORT0_DIR_REG, 1<<14 ); // User Manual page 470

   pinValueSet( LED1, ON );
}

MINUT_BEG
RUN( gpio_input_1() );
RUN( gpio_input_2() );
RUN( gpio_output() );
MINUT_END
