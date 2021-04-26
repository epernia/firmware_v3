/*=============================================================================
 * Copyright (c) 2021, Eric Pernia <ericpernia@gmail.com>
 * All rights reserved.
 * License: bsd-3-clause
 * Date: 2021/04/24
 * Version: 1.0
 *===========================================================================*/

// Ejemplo para prender un LED haciendo polling	sobre el estado de un pulsador


// Bibliotecas del fabricante LCP Open ========================================

#include <chip.h> // Este incluye:
                  // <scu_18xx_43xx.h> --> System Control Unit (SCU)
                  // <gpio_18xx_43xx.h> --> General Purpose Input/Output (GPIO)
                  // entre otros

// Constantes agregadas =======================================================

#define OUTPUT   1
#define INPUT    0

#define ON       1
#define OFF      0

// Inicio de programa principal ===============================================

int main( void )
{
   // Diodo LED: LED1 <---> P2_10, GPIO0[14] ----------------------------------

   // Configuro que el pin P2_10 se conecte internamente a GPIO0[14]
   Chip_SCU_PinMux( 2, 10, SCU_MODE_INACT, SCU_MODE_FUNC0 );

   // Configuro GPIO0[14] como salida
   Chip_GPIO_SetDir( LPC_GPIO_PORT, 0, (1<<14), OUTPUT );

   // Pongo en estado bajo el GPIO0[14]
   Chip_GPIO_SetPinState( LPC_GPIO_PORT, 0, 14, OFF );

   // Pulsador: TEC1 <---> P1_0, GPIO0[4] -------------------------------------

   // Configuro que el pin P1_0 se conecte internamente a GPIO0[4]
   Chip_SCU_PinMux( 1, 0, SCU_MODE_INACT | SCU_MODE_INBUFF_EN, SCU_MODE_FUNC0 );

   // Configuro GPIO0[4] como entrada
   Chip_GPIO_SetDir( LPC_GPIO_PORT, 0, (1<<4), INPUT );

   while( 1 ) {
       
      // Repetir por siempre --------------------------------------------------
       
      // !(TEC1) --> LED1
      // Negado (!) porque el pulsador esta en 1 por defecto y al presionar da 0
      uint8_t value = Chip_GPIO_GetPinState( LPC_GPIO_PORT, 0, 4 );
      Chip_GPIO_SetPinState( LPC_GPIO_PORT, 0, 14, !value );
   }

   return 0;
}
