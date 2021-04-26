/*=============================================================================
 * Copyright (c) 2016, Nicolás Alvarez
 * Copyright (c) 2021, Eric Pernia <ericpernia@gmail.com>
 * All rights reserved.
 * License: bsd-3-clause
 * Date: 2016/05/06
 * Version: 1.0
 *===========================================================================*/

// Ejemplo para prender un LED haciendo polling	sobre el estado de un pulsador


// Direcciones de los registros ===============================================

// System Control Unit (SCU) registers address --------------------------------

#define SCU_BASE                        0x40086000

// Offset de los registros de configuracion para los pines
// (pag. 405 de User Manual)

// Pulsadores (Touch Buttons)
#define	SFSP1_0              0x080        // pin P1_0  -> TEC1
#define	SFSP1_1              0x084        // pin P1_1  -> TEC2
#define	SFSP1_2              0x088        // pin P1_2  -> TEC3
#define	SFSP1_6              0x098        // pin P1_6  -> TEC4

// LEDs
#define	SFSP2_0              0x100        // pin P2_0  -> LEDR
#define	SFSP2_1              0x104        // pin P2_1  -> LEDG
#define	SFSP2_2              0x108        // pin P2_2  -> LEDB
#define	SFSP2_10             0x128        // pin P2_10 -> LED1
#define	SFSP2_11             0x12C        // pin P2_11 -> LED2
#define	SFSP2_12             0x130        // pin P2_12 -> LED3

// Funcion y modo (pag. 413 de User Manual)

#define SCU_MODE_FUNC0       0x0          // Selects pin function 0
#define SCU_MODE_FUNC1       0x1          // Selects pin function 1
#define SCU_MODE_FUNC2       0x2          // Selects pin function 2
#define SCU_MODE_FUNC3       0x3          // Selects pin function 3
#define SCU_MODE_FUNC4       0x4          // Selects pin function 4
#define SCU_MODE_FUNC5       0x5          // Selects pin function 5

#define SCU_MODE_INACT       (0x2 << 3)   // Disable pull-down and pull-up
#define SCU_MODE_PULLUP      (0x0 << 3)   // Enable pull-up resistor at pad
#define SCU_MODE_PULLDOWN    (0x3 << 3)   // Enable pull-down resistor at pad

#define SCU_MODE_INBUFF_EN   (0x1 << 6)   // Enable Input buffer

#define SCU_MODE_ZIF_DIS     (0x1 << 7)   // Disable input glitch filter

// General Purpose Input/Output (GPIO) registers address ----------------------

#define GPIO_BASE               0x400F4000

#define GPIO_PORT0_B_OFFSET     0x0000
#define GPIO_PORT0_DIR_OFFSET   0x2000
#define GPIO_PORT0_SET_OFFSET   0x2200
#define GPIO_PORT0_CLR_OFFSET   0x2280

#define GPIO_PORT1_B_OFFSET     0x0020
#define GPIO_PORT1_DIR_OFFSET   0x2004
#define GPIO_PORT1_SET_OFFSET   0x2204
#define GPIO_PORT1_CLR_OFFSET   0x2284


// Macro para calculo de direcciones (direccion = base + offset) ==============

#define ADDRESS(x, offset) (*(volatile int *)(volatile char *) ((x)+(offset)))


// Inicio de programa principal ===============================================

int main( void )
{    
   // Diodo LED: LED1 <---> P2_10, GPIO0[14] ----------------------------------
    
   // Configuro que el pin P2_10 se conecte internamente a GPIO0[14]
   ADDRESS( SCU_BASE, SFSP2_10 ) = SCU_MODE_FUNC0 | SCU_MODE_INACT;   

   // Configuro GPIO0[14] como salida  
   ADDRESS( GPIO_BASE, GPIO_PORT0_DIR_OFFSET ) |= 1 << 14;

   // Pongo en estado bajo el GPIO0[14]
   ADDRESS( GPIO_BASE, GPIO_PORT0_CLR_OFFSET ) |= 1 << 14;

   // Pulsador: TEC1 <---> P1_0, GPIO0[4] -------------------------------------

   // Configuro que el pin P1_0 se conecte internamente a GPIO0[4]
   ADDRESS( SCU_BASE, SFSP1_0 ) =  SCU_MODE_FUNC0 | SCU_MODE_INACT |SCU_MODE_INBUFF_EN;   

   // Configuro GPIO0[4] como entrada
   ADDRESS( GPIO_BASE, GPIO_PORT0_DIR_OFFSET ) &= ~(1 << 4);

   while( 1 ) {
       
      // Repetir por siempre --------------------------------------------------
       
      // !(TEC1) --> LED1
      // Negado (!) porque el pulsador esta en 1 por defecto y al presionar da 0
      unsigned char value = ADDRESS( GPIO_BASE, GPIO_PORT0_B_OFFSET + 4 );
      ADDRESS( GPIO_BASE, GPIO_PORT0_B_OFFSET + 14 ) = !value; 
   }

   return 0;
}
