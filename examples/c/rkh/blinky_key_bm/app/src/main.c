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
#include "keys.h"
#include "common.h"
#include "bsp.h"

/* ----------------------------- Local macros ------------------------------ */
#define QSTO_SIZE                   4
#define NUM_LEDS                    4

#define CANTIDAD_EVENTOS_TECLAS     4
#define SIZEOF_EVENTO_TECLA         sizeof( t_key_event )

/* ------------------------------- Constants ------------------------------- */
/* ---------------------------- Local data types --------------------------- */
/* ---------------------------- Global variables --------------------------- */
/* ---------------------------- Local variables ---------------------------- */

/* ALMACENAMIENTO PARA LOS EVENTOS DINAMICOS */
static rui8_t   key_event_storage[ CANTIDAD_EVENTOS_TECLAS*SIZEOF_EVENTO_TECLA ];

/* cola de eventos de entrada para cada AO de leds */
static RKH_EVT_T *event_q[NUM_LEDS][QSTO_SIZE];

/* cola de eventos de entrada para cada AO de keys */
static RKH_EVT_T *event_key[CANTIDAD_EVENTOS_TECLAS][QSTO_SIZE];

/* ----------------------- Local function prototypes ----------------------- */
/* ---------------------------- Local functions ---------------------------- */
/* ---------------------------- Global functions --------------------------- */
int main( int argc, char *argv[] )
{
    /* inicializo el board support package */
    bsp_init( argc, argv );

    /* inicializo el framework */
    rkh_fwk_init();

    /* inicializo el motor de eventos dinamicos */
    rkh_dynEvt_init();

    /* registro el pool de eventos dinamicos */
    rkh_fwk_registerEvtPool( key_event_storage, CANTIDAD_EVENTOS_TECLAS*SIZEOF_EVENTO_TECLA, SIZEOF_EVENTO_TECLA  );

    /* activa las maquinas de estados: estas acciones llaman a blinky_init */
    /*                app       cola ev        tamaño cola   stack    stascksize */
    RKH_SMA_ACTIVATE( blinky0 , event_q[0]   , QSTO_SIZE    , 0      , 0 );
    RKH_SMA_ACTIVATE( blinky1 , event_q[1]   , QSTO_SIZE    , 0      , 0 );
    RKH_SMA_ACTIVATE( blinky2 , event_q[2]   , QSTO_SIZE    , 0      , 0 );
    RKH_SMA_ACTIVATE( blinky3 , event_q[3]   , QSTO_SIZE    , 0      , 0 );

    /* activa las maquinas de estados: estas acciones llaman a keys_init */
    /*                app      cola ev         tamaño cola    stack    stascksize */
    RKH_SMA_ACTIVATE( key0,     event_key[0] , QSTO_SIZE    , 0      , 0 );
    RKH_SMA_ACTIVATE( key1,     event_key[1] , QSTO_SIZE    , 0      , 0 );
    RKH_SMA_ACTIVATE( key2,     event_key[2] , QSTO_SIZE    , 0      , 0 );
    RKH_SMA_ACTIVATE( key3,     event_key[3] , QSTO_SIZE    , 0      , 0 );

    /* comienzo a correr el framework */
    rkh_fwk_enter();

    RKH_TRC_CLOSE();
    return 0;
}

/* ------------------------------ End of file ------------------------------ */
