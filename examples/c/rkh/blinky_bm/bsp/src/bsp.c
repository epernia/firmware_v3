/*
 *  --------------------------------------------------------------------------
 *
 *                                Framework RKH
 *                                -------------
 *
 *            State-machine framework for reactive embedded systems
 *
 *                      Copyright (C) 2010 Leandro Francucci.
 *          All rights reserved. Protected by international copyright laws.
 *
 *
 *  RKH is free software: you can redistribute it and/or modify it under the
 *  terms of the GNU General Public License as published by the Free Software
 *  Foundation, either version 3 of the License, or (at your option) any
 *  later version.
 *
 *  RKH is distributed in the hope that it will be useful, but WITHOUT ANY
 *  WARRANTY; without even the implied warranty of MERCHANTABILITY or
 *  FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 *  more details.
 *
 *  You should have received a copy of the GNU General Public License along
 *  with RKH, see copying.txt file.
 *
 *  Contact information:
 *  RKH web site:   http://sourceforge.net/projects/rkh-reactivesys/
 *  e-mail:         francuccilea@gmail.com
 *  ---------------------------------------------------------------------------
 */

/**
 *  \file       bsp.c
 *  \brief      BSP for Cortex-M3 EDU-CIAA LPC4337 ARM-GCC
 *
 *  \ingroup    bsp
 */

/* -------------------------- Development history -------------------------- */
/*
 *  2017.06.23  DaBa  v1.0.00  Initial version
 */

/* -------------------------------- Authors -------------------------------- */
/*
 *  DaBa  Dario Bali\F1a       dariosb@gmail.com
 */

/* --------------------------------- Notes --------------------------------- */
/* ----------------------------- Include files ----------------------------- */
#include "rkh.h"
#include "rkhfwk_sched.h"
#include "blinky.h"
#include "bsp.h"


RKH_THIS_MODULE

/* ----------------------------- Local macros ------------------------------ */
/* ------------------------------- Constants ------------------------------- */
/* ---------------------------- Local data types --------------------------- */
/* ---------------------------- Global variables --------------------------- */
/* ---------------------------- Local variables ---------------------------- */
static RKH_TS_T tstamp;

/* ----------------------- Local function prototypes ----------------------- */
/* ---------------------------- Local functions ---------------------------- */
/* ---------------------------- Global functions --------------------------- */
void bsp_timeTick( void )
{
    ++tstamp;
}

RKH_TS_T rkh_trc_getts( void )
{
    return tstamp;
}

void bsp_init( int argc, char *argv[] )
{
    ( void )argc;
    ( void )argv;

    boardConfig();

    gpioConfig( 0, GPIO_ENABLE );
    gpioConfig( LED1, GPIO_OUTPUT );

    rkh_fwk_init();

    RKH_FILTER_ON_GROUP( RKH_TRC_ALL_GROUPS );
    RKH_FILTER_ON_EVENT( RKH_TRC_ALL_EVENTS );
    RKH_FILTER_OFF_EVENT( RKH_TE_TMR_TOUT );
    RKH_FILTER_OFF_EVENT( RKH_TE_SM_STATE );
    RKH_FILTER_OFF_SMA( blinky );
    RKH_FILTER_OFF_ALL_SIGNALS();

    RKH_TRC_OPEN();

    RKH_ENA_INTERRUPT();
}

void bsp_ledOn( rui32_t LEDx )
{
    gpioWrite( LEDx, ON );
}

void bsp_ledOff( rui32_t LEDx )
{
    gpioWrite( LEDx, OFF );
}

/* ------------------------------ End of file ------------------------------ */
