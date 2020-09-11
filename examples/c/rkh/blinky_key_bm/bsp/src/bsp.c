/* Copyright 2020, Franco Bucafusco.
 * All rights reserved.
 *
 * This file is part sAPI library for microcontrollers.
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

/* --------------------------------- Notes --------------------------------- */
/* ----------------------------- Include files ----------------------------- */
#include "rkh.h"
#include "rkhfwk_sched.h"
#include "blinky.h"
#include "bsp.h"


RKH_THIS_MODULE

void key_signal_event( gpioMap_t idx, rui32_t edge );
extern pinInitGpioLpc4337_t gpioPinsInit[];

/* ----------------------------- Local macros ------------------------------ */
/* ------------------------------- Constants ------------------------------- */
/* ---------------------------- Local data types --------------------------- */
/* ---------------------------- Global variables --------------------------- */
/* ---------------------------- Local variables ---------------------------- */
static RKH_TS_T tstamp;

/* ----------------------- Local function prototypes ----------------------- */
/* ---------------------------- Local functions ---------------------------- */
/* ---------------------------- Global functions --------------------------- */

/**
   @brief Callback que incrementa el contador de ticks

 */
void bsp_timeTick( void )
{
    ++tstamp;
}

RKH_TS_T rkh_trc_getts( void )
{
    return tstamp;
}

/**
   @brief Inicializa las interrupciones de los puertos asociados a TEC1 TEC2 TEC3 TEC4

 */
void isrConfig()
{
    int i=TEC1 ;
    int j=0;
    for(  ; i<=TEC4 ; i++, j++ )
    {
        /*Seteo la interrupción para el flanco descendente
                            channel, GPIOx,                        [y]    <- no es la config del pin, sino el nombre interno de la señal
                                |       |                           |
                                |       |                           |    */
        Chip_SCU_GPIOIntPinSel( j,  gpioPinsInit[i].gpio.port, gpioPinsInit[i].gpio.pin );   // TECi
        Chip_PININT_ClearIntStatus( LPC_GPIO_PIN_INT, PININTCH( j ) );                      // INTj (canal j -> hanlder GPIOj)       //Borra el pending de la IRQ
        Chip_PININT_SetPinModeEdge( LPC_GPIO_PIN_INT, PININTCH( j ) );                      // INTj //Selecciona activo por flanco
        Chip_PININT_EnableIntLow( LPC_GPIO_PIN_INT, PININTCH( j ) );                        // INTj //Selecciona activo por flanco descendente
        Chip_PININT_EnableIntHigh( LPC_GPIO_PIN_INT, PININTCH( j ) );                       // INTj //Selecciona activo por flanco ascendente

        //NVIC_SetPriority(PIN_INT0_IRQn, 7);
        NVIC_ClearPendingIRQ( PIN_INT0_IRQn+j );
        NVIC_EnableIRQ( PIN_INT0_IRQn+j );
    }
}

/**
   @brief Inicializa los perifericos necesarios del ejemplo

   @param argc
   @param argv
 */
void bsp_init( int argc, char *argv[] )
{
    ( void )argc;
    ( void )argv;

    /* inicializacion de la placa (API de SAPI) */
    boardConfig();

    /* configuro isr */
    isrConfig();

    /* inicializacion del framework rkh */
    rkh_fwk_init();

    /* configuraciones del framework */
    RKH_FILTER_ON_GROUP( RKH_TRC_ALL_GROUPS );
    RKH_FILTER_ON_EVENT( RKH_TRC_ALL_EVENTS );
    RKH_FILTER_OFF_EVENT( RKH_TE_TMR_TOUT );
    RKH_FILTER_OFF_EVENT( RKH_TE_SM_STATE );
    RKH_FILTER_OFF_SMA( blinky0 );
    RKH_FILTER_OFF_SMA( blinky1 );
    RKH_FILTER_OFF_SMA( blinky2 );
    RKH_FILTER_OFF_SMA( blinky3 );
    RKH_FILTER_OFF_ALL_SIGNALS();

    RKH_TRC_OPEN();

    RKH_ENA_INTERRUPT();
}

/**
   @brief Accion que enciende el LED "LEDx".

   @param LEDx  Para educiaa, puede ser cualquier LEDR,  LEDG,  LEDB,  LED1,  LED2,  LED3
 */
void bsp_ledOn( rui32_t LEDx )
{
    gpioWrite( LEDx, ON );
}

/**
   @brief Accion que apaga el LED "LEDx".

   @param LEDx  Para educiaa, puede ser cualquier LEDR,  LEDG,  LEDB,  LED1,  LED2,  LED3
 */
void bsp_ledOff( rui32_t LEDx )
{
    gpioWrite( LEDx, OFF );
}

/**
   @brief Handler de interrupcion para la TEC1 segun la inicializacion del periferico hecha en isrConfig
          Invoca a key_signal_event para señalizar la SM que sea necesaria.
 */
void GPIO0_IRQHandler( void )
{
    if ( Chip_PININT_GetFallStates( LPC_GPIO_PIN_INT ) & PININTCH0 )
    {
        //Verificamos que la interrupción es la esperada
        Chip_PININT_ClearIntStatus( LPC_GPIO_PIN_INT, PININTCH0 ); //Borramos el flag de interrupción
        //codigo a ejecutar si ocurrió la interrupción
        key_signal_event( TEC1, 1 );  //1 es falling
    }

    if ( Chip_PININT_GetRiseStates( LPC_GPIO_PIN_INT ) & PININTCH0 )
    {
        Chip_PININT_ClearIntStatus( LPC_GPIO_PIN_INT, PININTCH0 );
        key_signal_event( TEC1, 0 );  //1 es RISING
    }
}

/**
   @brief Handler de interrupcion para la TEC2 segun la inicializacion del periferico hecha en isrConfig
          Invoca a key_signal_event para señalizar la SM que sea necesaria.
 */
void GPIO1_IRQHandler( void )
{
    if ( Chip_PININT_GetFallStates( LPC_GPIO_PIN_INT ) & PININTCH1 )
    {
        //Verificamos que la interrupción es la esperada
        Chip_PININT_ClearIntStatus( LPC_GPIO_PIN_INT, PININTCH1 ); //Borramos el flag de interrupción
        //codigo a ejecutar si ocurrió la interrupción
        key_signal_event( TEC2, 1 );  //1 es falling
    }

    if ( Chip_PININT_GetRiseStates( LPC_GPIO_PIN_INT ) & PININTCH1 )
    {
        Chip_PININT_ClearIntStatus( LPC_GPIO_PIN_INT, PININTCH1 );
        key_signal_event( TEC2, 0 );  //1 es RISING
    }
}

/**
   @brief Handler de interrupcion para la TEC3 segun la inicializacion del periferico hecha en isrConfig.
          Invoca a key_signal_event para señalizar la SM que sea necesaria.
*/
void GPIO2_IRQHandler( void )
{
    if ( Chip_PININT_GetFallStates( LPC_GPIO_PIN_INT ) & PININTCH2 )
    {
        //Verificamos que la interrupción es la esperada
        Chip_PININT_ClearIntStatus( LPC_GPIO_PIN_INT, PININTCH2 ); //Borramos el flag de interrupción
        //codigo a ejecutar si ocurrió la interrupción
        key_signal_event( TEC3, 1 );  //1 es falling
    }

    if ( Chip_PININT_GetRiseStates( LPC_GPIO_PIN_INT ) & PININTCH2 )
    {
        Chip_PININT_ClearIntStatus( LPC_GPIO_PIN_INT, PININTCH2 );
        key_signal_event( TEC3, 0 );  //1 es RISING
    }
}

/**
   @brief Handler de interrupcion para la TEC4 segun la inicializacion del periferico hecha en isrConfig
          Invoca a key_signal_event para señalizar la SM que sea necesaria.
 */
void GPIO3_IRQHandler( void )
{
    if ( Chip_PININT_GetFallStates( LPC_GPIO_PIN_INT ) & PININTCH3 )
    {
        //Verificamos que la interrupción es la esperada
        Chip_PININT_ClearIntStatus( LPC_GPIO_PIN_INT, PININTCH3 ); //Borramos el flag de interrupción
        //codigo a ejecutar si ocurrió la interrupción
        key_signal_event( TEC4, 1 );  //1 es falling
    }

    if ( Chip_PININT_GetRiseStates( LPC_GPIO_PIN_INT ) & PININTCH3 )
    {
        Chip_PININT_ClearIntStatus( LPC_GPIO_PIN_INT, PININTCH3 );
        key_signal_event( TEC4, 0 );  //1 es RISING
    }
}



/* ------------------------------ End of file ------------------------------ */
