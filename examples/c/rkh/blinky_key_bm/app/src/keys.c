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
#include "keys.h"
#include "common.h"
#include "bsp.h"

/* ----------------------------- Local macros ------------------------------ */
#define KEY_DEBOUNCE            RKH_TIME_MS(20)

/* ......................... Declares active object ........................ */
typedef struct Key Key;

/* ................... Declares states and pseudostates .................... */
RKH_DCLR_BASIC_STATE KeyUp;         //estado estable
RKH_DCLR_BASIC_STATE KeyUpToDown;   //transicion
RKH_DCLR_BASIC_STATE KeyDown ;      //estado estable
RKH_DCLR_BASIC_STATE KeyDownToUp;   //transicion

RKH_DCLR_COND_STATE  timeout_u2d;   //pseudo estado
RKH_DCLR_COND_STATE  timeout_d2u;   //pseudo estado


/* ........................ Declares initial action ........................ */
static void key_init( Key *const me, gpioMap_t key_index );
static void key_init0( Key *const me  );
static void key_init1( Key *const me  );
static void key_init2( Key *const me  );
static void key_init3( Key *const me  );

/* ........................ Declares effect actions ........................ */

/* accion de estrada */

rbool_t is_key_pressed( Key *const me, RKH_EVT_T *pe );

static void key_start_timer_down( Key *const me, RKH_EVT_T *pe );
static void key_start_timer_up( Key *const me, RKH_EVT_T *pe );


static void record_d2u_time( Key *const me, RKH_EVT_T *pe );

/* ......................... Declares entry actions ........................ */
/* ......................... Declares exit actions ......................... */
/* ............................ Declares guards ............................ */
/* ........................ States and pseudostates ........................ */

/* estado KeyUp */
/*                      nombre   accion_entrada  accion_salida   estado_padre    preprocessor */
RKH_CREATE_BASIC_STATE( KeyUp  , NULL ,          NULL ,           RKH_ROOT      , NULL );            //creacion del estado

RKH_CREATE_TRANS_TABLE( KeyUp )                                                                     //inicio de tabla de transicion
/*         signal      condicion  efecto                estado_destino */
RKH_TRREG( KEY_PRESS , NULL,      key_start_timer_down, &KeyUpToDown ),
RKH_END_TRANS_TABLE                                                                                 //fin de tabla de transicion

/* estado KeyDown */
/*                      nombre   accion_entrada  accion_salida   estado_padre    preprocessor */
RKH_CREATE_BASIC_STATE( KeyDown, NULL ,          NULL ,          RKH_ROOT      , NULL );            //creacion del estado

RKH_CREATE_TRANS_TABLE( KeyDown )                                                                   //inicio de tabla de transicion
/*         signal        condicion  efecto                estado_destino */
RKH_TRREG( KEY_RELEASE , NULL,      key_start_timer_up,   &KeyDownToUp ),
RKH_END_TRANS_TABLE                                                                                 //fin de tabla de transicion

/* estado KeyUpToDown */
/*                      nombre       accion_entrada  accion_salida   estado_padre    preprocessor */
RKH_CREATE_BASIC_STATE( KeyUpToDown, NULL ,          NULL ,          RKH_ROOT,       NULL );        //creacion del estado

RKH_CREATE_TRANS_TABLE( KeyUpToDown )                                                               //inicio de tabla de transicion
/*         signal        condicion  efecto                estado_destino */
RKH_TRREG( KEY_TIMEOUT,  NULL,      NULL,                 &timeout_u2d ),
RKH_END_TRANS_TABLE                                                                                 //fin de tabla de transicion

/*                      nombre       accion_entrada  accion_salida   estado_padre    preprocessor */
RKH_CREATE_BASIC_STATE( KeyDownToUp, NULL ,          NULL ,          RKH_ROOT ,      NULL );        //creacion del estado

RKH_CREATE_TRANS_TABLE( KeyDownToUp )                                                               //inicio de tabla de transicion
/*         signal        condicion  efecto                estado_destino */
RKH_TRREG( KEY_TIMEOUT,  NULL,      NULL,                 &timeout_d2u ),
RKH_END_TRANS_TABLE                                                                                 //fin de tabla de transicion

/* presudo estado timeout_u2d */
RKH_CREATE_COND_STATE( timeout_u2d );                                                               //creacion del pseudo estado

RKH_CREATE_BRANCH_TABLE( timeout_u2d )
/*          condicion       efecto            estado_destino */
RKH_BRANCH( is_key_pressed, NULL,             &KeyDown    ),
RKH_BRANCH( ELSE,           NULL,             &KeyUp     ),
RKH_END_BRANCH_TABLE

/* presudo estado timeout_d2u */
RKH_CREATE_COND_STATE( timeout_d2u );                                                               //creacion del pseudo estado

RKH_CREATE_BRANCH_TABLE( timeout_d2u )
/*          condicion       efecto            estado_destino */
RKH_BRANCH( is_key_pressed, NULL,             &KeyDown    ),
RKH_BRANCH( ELSE,           record_d2u_time,  &KeyUp      ),
RKH_END_BRANCH_TABLE

/* ............................. Active object ............................. */
struct Key
{
	RKH_SMA_T sma;      /* base structure */
	gpioMap_t idx;      /* index de tecla */
	RKH_TMR_T timer;    /* for debouncing */
	rui32_t t_down;     /* ultimo tiempo en que se registro un KeyPress */
	rui32_t t_up;       /* ultimo tiempo en que se registro un KeyRelease */
};

/* instancio los AOs ( referencia http://rkh-reactivesys.sourceforge.net/qref.html#qref0 )
				tipo  nombre_instancia   prioridad    tipo    estado_inicial    accion_inicial   parametro */
RKH_SMA_CREATE( Key,  key0,              4,           HCAL,   &KeyUp,           key_init0,       NULL );
RKH_SMA_DEF_PTR( key0 );
RKH_SMA_CREATE( Key,  key1,              5,           HCAL,   &KeyUp,           key_init1,       NULL );
RKH_SMA_DEF_PTR( key1 );
RKH_SMA_CREATE( Key,  key2,              6,           HCAL,   &KeyUp,           key_init2,       NULL );
RKH_SMA_DEF_PTR( key2 );
RKH_SMA_CREATE( Key,  key3,              7,           HCAL,   &KeyUp,           key_init3,       NULL );
RKH_SMA_DEF_PTR( key3 );

/* PSEUDO STATES (HABILITADO CON RKH_CFG_SMA_PSEUDOSTATE_EN) (REF: http://rkh-reactivesys.sourceforge.net/qref.html#qref3) */

/* ------------------------------- Constants ------------------------------- */
/* ---------------------------- Local data types --------------------------- */
/* ---------------------------- Global variables --------------------------- */
/* ---------------------------- Local variables ---------------------------- */

/* declaracion del evento estatico (alocado en flash) */
static RKH_ROM_STATIC_EVENT( e_tout, KEY_TIMEOUT );


/* ----------------------- Local function prototypes ----------------------- */
/* ---------------------------- Local functions ---------------------------- */
/* ............................ Initial action ............................. */
static void key_init( Key *const me, gpioMap_t key_index )
{
	/* REGISTRO DE OBJETOS PARA HACER TRACE VIA UART */
	RKH_TR_FWK_AO( me );
	RKH_TR_FWK_QUEUE( &RKH_UPCAST( RKH_SMA_T, me )->equeue );
	RKH_TR_FWK_STATE( me, &KeyUp );
	RKH_TR_FWK_STATE( me, &KeyUpToDown );
	RKH_TR_FWK_STATE( me, &KeyDown );
	RKH_TR_FWK_STATE( me, &KeyDownToUp );
	RKH_TR_FWK_STATE( me, &timeout_u2d );
	RKH_TR_FWK_STATE( me, &timeout_d2u );
	RKH_TR_FWK_TIMER( &me->timer );
	RKH_TR_FWK_SIG( KEY_TIMEOUT );
	RKH_TR_FWK_SIG( KEY_PRESS );
	RKH_TR_FWK_SIG( KEY_RELEASE );

	/* INICIALIZA EL TIMER */
	RKH_TMR_INIT( &me->timer, &e_tout, NULL );

	/* inicializo otros campos */
	me->idx = key_index;
}

/**
   @brief Inicializacion de la SM key0

   @param me
 */
static void key_init0( Key *const me )
{
	key_init( me, TEC1 );
}

/**
   @brief Inicializacion de la SM key1

   @param me
 */
static void key_init1( Key *const me )
{
	key_init( me, TEC2 );
}

/**
   @brief Inicializacion de la SM key2

   @param me
 */
static void key_init2( Key *const me )
{
	key_init( me, TEC3 );
}

/**
   @brief Inicializacion de la SM key3

   @param me
 */
static void key_init3( Key *const me )
{
	key_init( me, TEC4 );
}

/* ............................ Effect actions ............................. */

/**
   @brief Accion que se ejecuta cuando se confirma el flanco ascendente de una tecla.
		  Se utiliza para hacer post de un evento a una maquina de estados de LEDs

   @param me
   @param pe
 */
static void record_d2u_time( Key *const me, RKH_EVT_T *pe )
{
	/* envio el evento a la SM del led */
	t_key_event* event_hnd = RKH_ALLOC_EVT( t_key_event, BLINK_CONFIG_CHANGE , me );

	/* cargo el indice de tecla */
	event_hnd->key = me->idx;

	/* cargo la diferencia de tiempo en el evento dinamico  */
	event_hnd->time_diff = me->t_up - me->t_down ;

	/* envio de evento de tecla a la SM del led asociado */
	RKH_SMA_T* state_machine_de_destino;

	if( TEC1 == me->idx )
	{
		state_machine_de_destino = blinky0;
	}
	if( TEC2 == me->idx )
	{
		state_machine_de_destino = blinky1;
	}
	if( TEC3 == me->idx )
	{
		state_machine_de_destino = blinky2;
	}
	if( TEC4 == me->idx )
	{
		state_machine_de_destino = blinky3;
	}

	rkh_sma_post_fifo( ( RKH_SMA_T * ) state_machine_de_destino, ( RKH_EVT_T* ) event_hnd );
}

/* ............................. Entry actions ............................. */
/* ............................. Exit actions .............................. */
/* ................................ Guards ................................. */

/**
   @brief Condicion utilizada para evaluar por encuesta cierta tecla, y que sirve
		  para el branching de los pseudoestados
   @param me
   @param pe
   @return rbool_t
 */
rbool_t is_key_pressed( Key *const me, RKH_EVT_T *pe )
{
	if( gpioRead( me->idx ) == FALSE )
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

/**
   @brief Accion que se ejecuta al confirmar un flanco descendente

   @param me
   @param pe
 */
static void key_start_timer_down( Key *const me, RKH_EVT_T *pe )
{
	t_key_event* event_hnd = ( t_key_event* ) pe;

	me->t_down = event_hnd->time_diff;

	RKH_TMR_ONESHOT( &me->timer, RKH_UPCAST( RKH_SMA_T, me ), KEY_DEBOUNCE );
}

/**
   @brief @brief Accion que se ejecuta al confirmar un flanco ascendente

   @param me
   @param pe
 */
static void key_start_timer_up( Key *const me, RKH_EVT_T *pe )
{
	t_key_event* event_hnd = ( t_key_event* ) pe;
	me->t_up = event_hnd->time_diff;

	RKH_TMR_ONESHOT( &me->timer, RKH_UPCAST( RKH_SMA_T, me ), KEY_DEBOUNCE );
}

/**
   @brief API de señalizacion de un evento de HW, proveniente del handler de ISR.

   @param idx
   @param edge
 */
void key_signal_event( gpioMap_t idx, rui32_t edge )
{
	Key * state_machine_de_destino;

	if( TEC1 == idx )
	{
		state_machine_de_destino = key0;
	}
	if( TEC2 == idx )
	{
		state_machine_de_destino = key1;
	}
	if( TEC3 == idx )
	{
		state_machine_de_destino = key2;
	}
	if( TEC4 == idx )
	{
		state_machine_de_destino = key3;
	}

	/* creo el evento dinamico */
	t_key_event* event_hnd = RKH_ALLOC_EVT( t_key_event, ( edge==1 )?KEY_PRESS:KEY_RELEASE, state_machine_de_destino );

	/* cargo el indice de tecla */
	event_hnd->key = idx;

	/* cargo el valor del tick actual (levantandolo de la API de sapi) en el registro de timediff */
	event_hnd->time_diff = tickRead();

	rkh_sma_post_fifo( ( RKH_SMA_T * ) state_machine_de_destino, ( RKH_EVT_T* ) event_hnd );
}

/* ---------------------------- Global functions --------------------------- */
/* ------------------------------ End of file ------------------------------ */
