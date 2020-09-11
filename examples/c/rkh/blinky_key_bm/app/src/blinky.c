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
#include "rkhfwk_dynevt.h"
#include "blinky.h"
#include "common.h"
#include "bsp.h"

/* ----------------------------- Local macros ------------------------------ */
#define LED_OFF_TIME            RKH_TIME_MS(500)
#define LED_ON_TIME             RKH_TIME_MS(500)

/* ......................... Declares active object ........................ */
typedef struct Blinky Blinky;

/* ................... Declares states and pseudostates .................... */
RKH_DCLR_BASIC_STATE ledOn;
RKH_DCLR_BASIC_STATE ledOff;

/* ........................ Declares initial action ........................ */
static void blinky_init( Blinky *const me, gpioMap_t index );
static void blinky_init0( Blinky *const me );
static void blinky_init1( Blinky *const me );
static void blinky_init2( Blinky *const me );
static void blinky_init3( Blinky *const me );

/* ........................ Declares effect actions ........................ */

/* acciones de transicion de estado */
static void blinky_ledOn( Blinky *const me, RKH_EVT_T *pe );
static void blinky_ledOff( Blinky *const me, RKH_EVT_T *pe );
static void blinky_config_change( Blinky *const me, RKH_EVT_T *pe );

/* ......................... Declares entry actions ........................ */
/* ......................... Declares exit actions ......................... */
/* ............................ Declares guards ............................ */
/* ........................ States and pseudostates ........................ */

/* estado ledOn */
/*                      nombre  accion_entrada  accion_salida   estado_padre    preprocessor */
RKH_CREATE_BASIC_STATE( ledOn,  NULL,           NULL,           RKH_ROOT,       NULL );

RKH_CREATE_TRANS_TABLE( ledOn )
/*         signal               condicion  efecto                estado_destino */
RKH_TRREG( TIMEOUT,             NULL,      blinky_ledOff,        &ledOff ),  /* cuando ocurre un timeout*/
RKH_TRREG( BLINK_CONFIG_CHANGE, NULL,      blinky_config_change, &ledOn ),
RKH_END_TRANS_TABLE

/* estado ledOff */
/*                      nombre  accion_entrada  accion_salida   estado_padre    preprocessor */
RKH_CREATE_BASIC_STATE( ledOff, NULL,           NULL,           RKH_ROOT,       NULL );


RKH_CREATE_TRANS_TABLE( ledOff )    /* tabla de transicion del estado ledOff */
/*         signal               condicion  efecto                estado_destino */
RKH_TRREG( TIMEOUT,             NULL,      blinky_ledOn,         &ledOn ),
RKH_TRREG( BLINK_CONFIG_CHANGE, NULL,      blinky_config_change, &ledOff ),
RKH_END_TRANS_TABLE

/* ............................. Active object ............................. */
struct Blinky
{
	RKH_SMA_T sma;      /* base structure */
	rui8_t cnt;         /* private member */
	RKH_TMR_T timer;    /* which is responsible for toggling the LED */

	RKH_TNT_T timeout;  /* tiempo de medio periodo */
	rui32_t led_idx;    /* identifica que LED gestionará la SM */
};

/* instancio los AOs ( referencia http://rkh-reactivesys.sourceforge.net/qref.html#qref0 )
				tipo     nombre_instancia   prioridad    tipo    estado_inicial    accion_inicial   parametro */
RKH_SMA_CREATE( Blinky,  blinky0,           0,           HCAL,   &ledOn,           blinky_init0,    NULL );
RKH_SMA_DEF_PTR( blinky0 );

RKH_SMA_CREATE( Blinky,  blinky1,           1,           HCAL,   &ledOn,           blinky_init1,    NULL );
RKH_SMA_DEF_PTR( blinky1 );

RKH_SMA_CREATE( Blinky,  blinky2,           2,           HCAL,   &ledOn,           blinky_init2,    NULL );
RKH_SMA_DEF_PTR( blinky2 );

RKH_SMA_CREATE( Blinky,  blinky3,           3,           HCAL,   &ledOn,           blinky_init3,    NULL );
RKH_SMA_DEF_PTR( blinky3 );


/* ------------------------------- Constants ------------------------------- */
/* ---------------------------- Local data types --------------------------- */
/* ---------------------------- Global variables --------------------------- */
/* ---------------------------- Local variables ---------------------------- */
/*
 * Este evento posee una configuracion fija, por lo que se instancia en ROM y se usa para todos
 * las instancias de blinky
 */
static RKH_ROM_STATIC_EVENT( e_tout, TIMEOUT );

/* ----------------------- Local function prototypes ----------------------- */
/* ---------------------------- Local functions ---------------------------- */
/* ............................ Initial action ............................. */
static void blinky_init( Blinky *const me, gpioMap_t index )
{
	/* REGISTRO DE OBJETOS PARA HACER TRACE VIA UART */
	RKH_TR_FWK_AO( me );
	RKH_TR_FWK_QUEUE( &RKH_UPCAST( RKH_SMA_T, me )->equeue );
	RKH_TR_FWK_STATE( me, &ledOn );
	RKH_TR_FWK_STATE( me, &ledOff );
	RKH_TR_FWK_TIMER( &me->timer );
	RKH_TR_FWK_SIG( TIMEOUT );
	RKH_TR_FWK_SIG( BLINK_CONFIG_CHANGE );

	/* INICIALIZA EL TIMER */
	RKH_TMR_INIT( &me->timer, &e_tout, NULL );

	/* inicializo otros campos */
	me->timeout = LED_ON_TIME;
	me->led_idx = index;

	/* arranca encendido */
	blinky_ledOn( me, NULL );
}

/**
   @brief Inicializacion de la SM LEDB

   @param me
 */
static void blinky_init0( Blinky *const me )
{
	blinky_init(  me, LEDB );
}

/**
   @brief Inicializacion de la SM LED1

   @param me
 */
static void blinky_init1( Blinky *const me )
{
	blinky_init(  me, LED1 );
}

/**
   @brief Inicializacion de la SM LED2

   @param me
 */
static void blinky_init2( Blinky *const me )
{
	blinky_init(  me, LED2 );
}

/**
   @brief Inicializacion de la SM LED3

   @param me
 */
static void blinky_init3( Blinky *const me )
{
	blinky_init(  me, LED3 );
}

/* ............................ Effect actions ............................. */

/**
   @brief Accion de transicion de apagado a encendido

   @param me
   @param pe
 */
static void blinky_ledOn( Blinky *const me, RKH_EVT_T *pe )
{
	( void )pe;

	/* arranco el timer para hacer un toggle */
	RKH_TMR_ONESHOT( &me->timer, RKH_UPCAST( RKH_SMA_T, me ), me->timeout );
	bsp_ledOn( me->led_idx );
	++me->cnt;
}

/**
   @brief accion de transicion de encendido a apagado

   @param me
   @param pe
 */
static void blinky_ledOff( Blinky *const me, RKH_EVT_T *pe )
{
	( void )me;
	( void )pe;

	RKH_TMR_ONESHOT( &me->timer, RKH_UPCAST( RKH_SMA_T, me ), me->timeout );
	bsp_ledOff( me->led_idx );
}


/**
   @brief accion de transicion, al recibir un evento de cambio de configuracion.

   @param me
   @param pe
 */
static void blinky_config_change( Blinky *const me, RKH_EVT_T *pe )
{
	t_key_event* event_hnd = (t_key_event*) pe;

	me->timeout = event_hnd->time_diff;
}




/* ............................. Entry actions ............................. */
/* ............................. Exit actions .............................. */
/* ................................ Guards ................................. */
/* ---------------------------- Global functions --------------------------- */
/* ------------------------------ End of file ------------------------------ */

