/* Copyright 2015-2016, Eric Pernia.
 * All rights reserved.
 *
 * This file is part of CIAA Firmware.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * 3. Neither the name of the copyright holder nor the names of its
 *    contributors may be used to endorse or promote products derived from this
 *    software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 */

/* Date: 2015-09-23 */

/*==================[inclusions]=============================================*/

#include "sapi_gpio.h"

/*==================[macros and definitions]=================================*/

/*==================[internal data declaration]==============================*/

/*==================[internal functions declaration]=========================*/

/*==================[internal data definition]===============================*/

const pinInitGpioLpc4337_t gpioPinsInit[] = {

   //{ {PinNamePortN ,PinNamePinN}, PinFUNC, {GpioPortN, GpioPinN} }
   
   // Configure GPIO pins for each board
   #if BOARD==ciaa_nxp

   // --------------------------------------------------------------- 
   //                             CIAA-NXP                            
   // --------------------------------------------------------------- 
   //                                     sAPI  Connector  Serigraphy 
   // --------------------------------------------------------------- 

      { {4, 0}, FUNC0, {2, 0} },   //  0   DI0     BORN_24   DIN0
      { {4, 1}, FUNC0, {2, 1} },   //  1   DI1     BORN_25   DIN1
      { {4, 2}, FUNC0, {2, 2} },   //  2   DI2     BORN_26   DIN2
      { {4, 3}, FUNC0, {2, 3} },   //  3   DI3     BORN_27   DIN3
      { {7, 3}, FUNC0, {3,11} },   //  4   DI4     BORN_28   DIN4
      { {7, 4}, FUNC0, {3,12} },   //  5   DI5     BORN_29   DIN5
      { {7, 5}, FUNC0, {3,13} },   //  6   DI6     BORN_30   DIN6
      { {7, 6}, FUNC0, {3,14} },   //  7   DI7     BORN_31   DIN7

      { {2, 1}, FUNC4, {5, 1} },   //  8   DO0     BORN_14   DOUT0
      { {4, 6}, FUNC0, {2, 6} },   //  9   DO1     BORN_06   DOUT1
      { {4, 5}, FUNC0, {2, 5} },   // 10   DO2     BORN_08   DOUT2
      { {4, 4}, FUNC0, {2, 4} },   // 11   DO3     BORN_10   DOUT3
      { {4, 8}, FUNC4, {5,12} },   // 12   DO4     BORN_14   DOUT4
      { {4, 9}, FUNC4, {5,13} },   // 13   DO5     BORN_15   DOUT5
      { {4,10}, FUNC4, {5,14} },   // 14   DO6     BORN_16   DOUT6
      { {1, 5}, FUNC0, {1, 8} },   // 15   DO7     BORN_17   DOUT7

      // P12
      { {6, 1}, FUNC0, {3, 0} },   // 16   GPIO0   P12
      { {2, 5}, FUNC4, {5, 5} },   // 17   GPIO1   P12
      { {7, 0}, FUNC0, {3, 8} },   // 18   GPIO2   P12
      { {6, 7}, FUNC4, {5,15} },   // 19   GPIO3   P12
      { {6, 3}, FUNC0, {3, 2} },   // 20   GPIO7   P12
      { {6, 6}, FUNC0, {0, 5} },   // 21   GPIO8   P12
   
      // P14 header
      { {1, 3}, FUNC0, {0,10} },   // 16   SPI_MISO P14
      { {1, 4}, FUNC0, {0,11} },   // 17   SPI_MOSI P14
      { {6, 7}, FUNC4, {5,15} },   // 18   SPI_CS   P14

      //#error CIAA-NXP

   #elif BOARD==edu_ciaa_nxp

      // ---------------------------------------------------------------
      //                           EDU-CIAA-NXP                         
      // ---------------------------------------------------------------
      //                             Snap  sAPI   Connector  Serigraphy 
      // ---------------------------------------------------------------

      // { {1,15}, FUNC0, {0, 2} },   //  0   DIO0    CON2_09   ENET_RXD0

      // { {1, 4}, FUNC0, {0,11} },   //  1   DIO1    CON2_21   SPI_MOSI
      // { {4, 9}, FUNC4, {5,13} },   //  2   DIO2    CON2_23   LCD_EN

      // { {6, 1}, FUNC0, {3, 0} },   //  3   DIO3    CON2_29   GPIO0
      // { {6, 5}, FUNC0, {3, 4} },   //  4   DIO4    CON2_31   GPIO2
      // { {6, 8}, FUNC4, {5,16} },   //  5   DIO5    CON2_33   GPIO4
      // { {6,10}, FUNC0, {3, 6} },   //  6   DIO6    CON2_35   GPIO6

      // { {0, 0}, FUNC0, {0, 0} },   //  7   DIO7    CON2_04   ENET_RXD1   
      // { {0, 1}, FUNC0, {0, 1} },   //  8   DIO8    CON2_06   ENET_TXEN   
      // { {7, 7}, FUNC0, {3,15} },   //  9   DIO9    CON2_08   ENET_MDC    
      // { {1,16}, FUNC0, {0, 3} },   // 10   DIO10   CON2_10   ENET_CRS_DV 
      // { {1,17}, FUNC0, {0,12} },   // 11   DIO11   CON2_12   ENET_MDIO   
      // { {1,18}, FUNC0, {0,13} },   // 12   DIO12   CON2_14   ENET_TXD0   
      // { {1,20}, FUNC0, {0,15} },   // 13   DIO13   CON2_16   ENET_TXD1   
      // { {1, 3}, FUNC0, {0,10} },   // 14   DIO14   CON2_18   SPI_MISO    

      // { {4,10}, FUNC4, {5,14} },   // 15   DIO15   CON2_22   LCD4        
      // { {4, 8}, FUNC4, {5,12} },   // 16   DIO16   CON2_24   LCDRS       
      // { {4, 6}, FUNC0, {2, 6} },   // 17   DIO17   CON2_26   LCD3        
      // { {4, 5}, FUNC0, {2, 5} },   // 18   DIO18   CON2_28   LCD2        
      // { {4, 4}, FUNC0, {2, 4} },   // 19   DIO19   CON2_30   LCD1        

      // { {6, 4}, FUNC0, {3, 3} },   // 20   DIO20   CON2_32   GPIO1       
      // { {6, 7}, FUNC4, {5,15} },   // 21   DIO21   CON2_34   GPIO3       
      // { {6, 9}, FUNC0, {3, 5} },   // 22   DIO22   CON2_36   GPIO5       
      // { {6,11}, FUNC0, {3, 7} },   // 23   DIO23   CON2_38   GPIO7       
      // { {6,12}, FUNC0, {2, 8} },   // 24   DIO24   CON2_40   GPIO8       

      // { {2, 4}, FUNC4, {5, 4} },   // 25   DIO25   CON1_23   RS232_RXD   
      // { {2, 3}, FUNC4, {5, 3} },   // 26   DIO26   CON1_25   RS232_TXD   
      // { {3, 1}, FUNC4, {5, 8} },   // 27   DIO27   CON1_27   CAN_RD     
      // { {3, 2}, FUNC4, {5, 9} },   // 28   DIO28   CON1_29   CAN_TD     
      // { {7, 4}, FUNC0, {3,12} },   // 29   DIO29   CON1_31   T_COL1     
      // { {4, 0}, FUNC0, {2, 0} },   // 30   DIO30   CON1_33   T_FIL0     
      // { {4, 3}, FUNC0, {2, 3} },   // 31   DIO31   CON1_35   T_FIL3     
      // { {4, 2}, FUNC0, {2, 2} },   // 32   DIO32   CON1_37   T_FIL2     
      // { {1, 5}, FUNC0, {1, 8} },   // 33   DIO33   CON1_39   T_COL0     

      // { {7, 5}, FUNC0, {3,13} },   // 34   DIO34   CON1_34   T_COL2     
      // { {4, 1}, FUNC0, {2, 1} },   // 35   DIO35   CON1_36   T_FIL1     

      { {4, 1}, FUNC0, {2, 1} },   //   0   CON1_36   T_FIL1          
      { {7, 5}, FUNC0, {3,13} },   //   1   CON1_34   T_COL2          

      { {1, 5}, FUNC0, {1, 8} },   //   2   CON1_39   T_COL0          
      { {4, 2}, FUNC0, {2, 2} },   //   3   CON1_37   T_FIL2          
      { {4, 3}, FUNC0, {2, 3} },   //   4   CON1_35   T_FIL3          
      { {4, 0}, FUNC0, {2, 0} },   //   5   CON1_33   T_FIL0          
      { {7, 4}, FUNC0, {3,12} },   //   6   CON1_31   T_COL1          

      { {3, 2}, FUNC4, {5, 9} },   //   7   CON1_29   CAN_TD          
      { {3, 1}, FUNC4, {5, 8} },   //   8   CON1_27   CAN_RD          

      { {2, 3}, FUNC4, {5, 3} },   //   9   CON1_25   RS232_TXD       
      { {2, 4}, FUNC4, {5, 4} },   //  10   CON1_23   RS232_RXD       

      { {6,12}, FUNC0, {2, 8} },   //  11   CON2_40   GPIO8           
      { {6,11}, FUNC0, {3, 7} },   //  12   CON2_38   GPIO7           
      { {6, 9}, FUNC0, {3, 5} },   //  13   CON2_36   GPIO5           
      { {6, 7}, FUNC4, {5,15} },   //  14   CON2_34   GPIO3           
      { {6, 4}, FUNC0, {3, 3} },   //  15   CON2_32   GPIO1           

      { {4, 4}, FUNC0, {2, 4} },   //  16   CON2_30   LCD1            
      { {4, 5}, FUNC0, {2, 5} },   //  17   CON2_28   LCD2            
      { {4, 6}, FUNC0, {2, 6} },   //  18   CON2_26   LCD3            
      { {4, 8}, FUNC4, {5,12} },   //  19   CON2_24   LCDRS           
      { {4,10}, FUNC4, {5,14} },   //  20   CON2_22   LCD4            

      { {1, 3}, FUNC0, {0,10} },   //  21   CON2_18   SPI_MISO        

      { {1,20}, FUNC0, {0,15} },   //  22   CON2_16   ENET_TXD1       
      { {1,18}, FUNC0, {0,13} },   //  23   CON2_14   ENET_TXD0       
      { {1,17}, FUNC0, {0,12} },   //  24   CON2_12   ENET_MDIO       
      { {1,16}, FUNC0, {0, 3} },   //  25   CON2_10   ENET_CRS_DV      
      { {7, 7}, FUNC0, {3,15} },   //  26   CON2_08   ENET_MDC         
      { {0, 1}, FUNC0, {0, 1} },   //  27   CON2_06   ENET_TXEN        
      { {0, 0}, FUNC0, {0, 0} },   //  28   CON2_04   ENET_RXD1        

      { {6,10}, FUNC0, {3, 6} },   //  29   CON2_35   GPIO6            
      { {6, 8}, FUNC4, {5,16} },   //  30   CON2_33   GPIO4            
      { {6, 5}, FUNC0, {3, 4} },   //  31   CON2_31   GPIO2            
      { {6, 1}, FUNC0, {3, 0} },   //  32   CON2_29   GPIO0            

      { {4, 9}, FUNC4, {5,13} },   //  33   CON2_23   LCDEN            

      { {1, 4}, FUNC0, {0,11} },   //  34   CON2_21   SPI_MOSI         

      { {1,15}, FUNC0, {0, 2} },   //  35   CON2_09   ENET_RXD0       


      { {1, 0}, FUNC0, {0, 4} },   // 36   TEC1    TEC_1  
      { {1, 1}, FUNC0, {0, 8} },   // 37   TEC2    TEC_2  
      { {1, 2}, FUNC0, {0, 9} },   // 38   TEC3    TEC_3  
      { {1, 6}, FUNC0, {1, 9} },   // 39   TEC4    TEC_4  

      { {2, 0}, FUNC4, {5, 0} },   // 43   LEDR    LED0_R 
      { {2, 1}, FUNC4, {5, 1} },   // 44   LEDG    LED0_G 
      { {2, 2}, FUNC4, {5, 2} },   // 45   LEDB    LED0_B 
      { {2,10}, FUNC0, {0,14} },   // 40   LED1    LED1   
      { {2,11}, FUNC0, {1,11} },   // 41   LED2    LED2   
      { {2,12}, FUNC0, {1,12} },   // 42   LED3    LED3   

   #elif BOARD==ciaa_z3r0
      #error CIAA-Z3R0

   #elif BOARD==pico_ciaa
      #error PicoCIAA

   #else
      #error BOARD compile variable must be defined

   #endif

};

/*==================[external data definition]===============================*/

/*==================[internal functions definition]==========================*/

static void gpioObtainPinInit( gpioMap_t pin,
                               int8_t *pinNamePort, int8_t *pinNamePin,
                               int8_t *func, int8_t *gpioPort,
                               int8_t *gpioPin )
{

   *pinNamePort = gpioPinsInit[pin].pinName.port;
   *pinNamePin  = gpioPinsInit[pin].pinName.pin;
   *func        = gpioPinsInit[pin].func;
   *gpioPort    = gpioPinsInit[pin].gpio.port;
   *gpioPin     = gpioPinsInit[pin].gpio.pin;
}

/*==================[external functions definition]==========================*/

bool_t gpioInit( gpioMap_t pin, gpioInit_t config )
{
   if( pin == VCC ){
	  return FALSE;
   }
   if( pin == GND ){
	  return FALSE;
   }

   bool_t ret_val     = 1;

   int8_t pinNamePort = 0;
   int8_t pinNamePin  = 0;

   int8_t func        = 0;

   int8_t gpioPort    = 0;
   int8_t gpioPin     = 0;

   gpioObtainPinInit( pin, &pinNamePort, &pinNamePin, &func,
                      &gpioPort, &gpioPin );

   switch(config) {

   case GPIO_ENABLE:
      /* Initializes GPIO */
      Chip_GPIO_Init(LPC_GPIO_PORT);
      break;

   case GPIO_INPUT:
      Chip_SCU_PinMux(
         pinNamePort,
         pinNamePin,
         SCU_MODE_INACT | SCU_MODE_INBUFF_EN | SCU_MODE_ZIF_DIS,
         func
      );
      Chip_GPIO_SetDir( LPC_GPIO_PORT, gpioPort, ( 1 << gpioPin ), GPIO_INPUT );
      break;

   case GPIO_INPUT_PULLUP:
      Chip_SCU_PinMux(
         pinNamePort,
         pinNamePin,
         SCU_MODE_PULLUP | SCU_MODE_INBUFF_EN | SCU_MODE_ZIF_DIS,
         func
      );
      Chip_GPIO_SetDir( LPC_GPIO_PORT, gpioPort, ( 1 << gpioPin ), GPIO_INPUT );
      break;

   case GPIO_INPUT_PULLDOWN:
      Chip_SCU_PinMux(
         pinNamePort,
         pinNamePin,
         SCU_MODE_PULLDOWN | SCU_MODE_INBUFF_EN | SCU_MODE_ZIF_DIS,
         func
      );
      Chip_GPIO_SetDir( LPC_GPIO_PORT, gpioPort, ( 1 << gpioPin ), GPIO_INPUT );
      break;
   case GPIO_INPUT_PULLUP_PULLDOWN:
      Chip_SCU_PinMux(
         pinNamePort,
         pinNamePin,
         SCU_MODE_REPEATER | SCU_MODE_INBUFF_EN | SCU_MODE_ZIF_DIS,
         func
      );
      Chip_GPIO_SetDir( LPC_GPIO_PORT, gpioPort, ( 1 << gpioPin ), GPIO_INPUT );
      break;

   case GPIO_OUTPUT:
      Chip_SCU_PinMux(
         pinNamePort,
         pinNamePin,
         SCU_MODE_INACT | SCU_MODE_ZIF_DIS | SCU_MODE_INBUFF_EN,
         func
      );
      Chip_GPIO_SetDir( LPC_GPIO_PORT, gpioPort, ( 1 << gpioPin ), GPIO_OUTPUT );
      Chip_GPIO_SetPinState( LPC_GPIO_PORT, gpioPort, gpioPin, 0);
      break;

   default:
      ret_val = 0;
      break;
   }

   return ret_val;

}


bool_t gpioWrite( gpioMap_t pin, bool_t value )
{
   if( pin == VCC ){
	  return FALSE;
   }
   if( pin == GND ){
	  return FALSE;
   }

   bool_t ret_val     = 1;

   int8_t pinNamePort = 0;
   int8_t pinNamePin  = 0;

   int8_t func        = 0;

   int8_t gpioPort    = 0;
   int8_t gpioPin     = 0;

   gpioObtainPinInit( pin, &pinNamePort, &pinNamePin, &func,
                      &gpioPort, &gpioPin );

   Chip_GPIO_SetPinState( LPC_GPIO_PORT, gpioPort, gpioPin, value);

   return ret_val;
}


bool_t gpioToggle( gpioMap_t pin )
{
   return gpioWrite( pin, !gpioRead(pin) );
}


bool_t gpioRead( gpioMap_t pin )
{
   if( pin == VCC ){
      return TRUE;
   }
   if( pin == GND ){
      return FALSE;
   }

   bool_t ret_val     = OFF;

   int8_t pinNamePort = 0;
   int8_t pinNamePin  = 0;

   int8_t func        = 0;

   int8_t gpioPort    = 0;
   int8_t gpioPin     = 0;

   gpioObtainPinInit( pin, &pinNamePort, &pinNamePin, &func,
                      &gpioPort, &gpioPin );

   ret_val = (bool_t) Chip_GPIO_ReadPortBit( LPC_GPIO_PORT, gpioPort, gpioPin );

   return ret_val;
}

/*==================[end of file]============================================*/
