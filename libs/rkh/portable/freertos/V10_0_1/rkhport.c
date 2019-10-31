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
 *  RKH site: http://vortexmakes.com/que-es/
 *  RKH GitHub: https://github.com/vortexmakes/RKH
 *  RKH Sourceforge: https://sourceforge.net/projects/rkh-reactivesys/
 *  e-mail: lf@vortexmakes.com
 *  ---------------------------------------------------------------------------
 */

/**
 *  \file       rkhport.c
 *  \brief      FreeRTOS
 *
 *  \ingroup    port
 */

/* -------------------------- Development history -------------------------- */
/*
 *  2017.04.14  LeFr  v2.4.05  Initial version
 */

/* -------------------------------- Authors -------------------------------- */
/*
 *  2018.09.23  FrBu Initial version
 */

/* --------------------------------- Notes --------------------------------- */
/* ----------------------------- Include files ----------------------------- */
#include "FreeRTOSConfig.h"
#include "rkh.h"
#include "rkhfwk_dynevt.h"
#include "FreeRTOS.h"

/* ----------------------------- Local macros ------------------------------ */
/* ------------------------------- Constants ------------------------------- */
RKH_MODULE_NAME( rkhport )
RKH_MODULE_VERSION( rkhport, 1.00 )
RKH_MODULE_DESC( rkhport, "FreeRTOS for EDUCIAA" )

/* ---------------------------- Local data types --------------------------- */
/* ---------------------------- Global variables --------------------------- */
/* ---------------------------- Local variables ---------------------------- */
/* ----------------------- Local function prototypes ----------------------- */
/* ---------------------------- Local functions ---------------------------- */

void vApplicationTickHook( void )
{
    rkh_tmr_tick( 0 );
}

static
void
thread_function( void *arg )
{
    RKH_SR_ALLOC();

    while ( ( ( RKH_SMA_T * )arg )->running != ( rbool_t )0 )
    {
        RKH_EVT_T *e = rkh_sma_get( ( RKH_SMA_T * )arg );
        rkh_sm_dispatch( ( RKH_SM_T * )arg, e );
        RKH_FWK_GC( e, arg );
    }

    rkh_sma_unregister( ( RKH_SMA_T * )arg );

    RKH_TR_SMA_TERM( ( RKH_SMA_T * )arg, RKH_GET_PRIO( ( RKH_SMA_T * )arg ) );

    vTaskDelete( 0 );/* deletes the currently running task */
}

/* ---------------------------- Global functions --------------------------- */
const
char *
rkhport_get_version( void )
{
    return RKH_MODULE_GET_VERSION();
}

const
char *
rkhport_get_desc( void )
{
    return RKH_MODULE_GET_DESC();
}

void
rkh_sma_block( RKH_SMA_T *const me )
{
    ( void )me;
}

void
rkh_sma_setReady( RKH_SMA_T *const me )
{
    ( void )me;
}

void
rkh_sma_setUnready( RKH_SMA_T *const me )
{
    ( void )me;
}

void
rkh_fwk_init( void )
{
}

void
rkh_fwk_enter( void )
{
    RKH_SR_ALLOC();

    RKH_HOOK_START();   /* RKH start-up callback */
    RKH_TR_FWK_EN();

    vTaskStartScheduler();      /* FreeRTOS start the multitasking */

    RKH_TRC_CLOSE();    /* cleanup the trace session */
    /* NEVER supposed to come back to this point */
}

void
rkh_fwk_exit( void )
{
    RKH_SR_ALLOC();
    RKH_TR_FWK_EX();
    RKH_HOOK_EXIT();    /* RKH cleanup callback */
}

void
rkh_sma_terminate( RKH_SMA_T *sma )
{
    //OS_ERR err;

    sma->running = ( rbool_t )0;

    vQueueDelete( &sma->equeue );	//OSQDel(&sma->equeue, OS_OPT_DEL_ALWAYS, &err);

    //RKH_ASSERT(err == OS_ERR_NONE);
}

void rkh_sma_activate( RKH_SMA_T *sma, const RKH_EVT_T **qs, RKH_QUENE_T qsize, void *stks, rui32_t stksize )
{
    rui8_t prio;

    RKH_REQUIRE( qs == ( const RKH_EVT_T ** )0 );

    sma->equeue = xQueueCreate( qsize, sizeof( void* ) );

    rkh_sma_register( sma );
    rkh_sm_init( ( RKH_SM_T * )sma );

    prio = RKH_GET_PRIO( sma ) + tskIDLE_PRIORITY + 1; //TODO: RESLVER EL TEMA DE LA PRIORIDAD.

    BaseType_t xReturned = xTaskCreate( thread_function, 				/* function */
                                        "freertos task",  				/* name */
                                        stksize / sizeof( StackType_t ), /* stack size */
                                        sma, 							/* function argument */
                                        prio,   						/* priority */
                                        &sma->thread
                                      );

    sma->running = ( rbool_t )1;
}

#if defined(RKH_USE_TRC_SENDER)
void
rkh_sma_post_fifo( RKH_SMA_T *sma, const RKH_EVT_T *e,
                   const void *const sender )
#else
void
rkh_sma_post_fifo( RKH_SMA_T *sma, const RKH_EVT_T *e )
#endif
{
    RKH_HOOK_SIGNAL( e );

    RKH_INC_REF( e );

    BaseType_t taskwoken;
    xQueueSendFromISR( sma->equeue,      	/* event queue object */
                       &e, 					/* actual event posted */
                       &taskwoken );

    //NOTA: Estoy ignorando el taskwoken... todas las tareas de que getionan SMS tiene la misma prioridad.

    RKH_TR_SMA_FIFO( sma, e, sender, e->pool, e->nref, 0, 0 );
}

#if (RKH_CFG_QUE_PUT_LIFO_EN == RKH_ENABLED) && \
    (defined(RKH_USE_TRC_SENDER))
void
rkh_sma_post_lifo( RKH_SMA_T *sma, const RKH_EVT_T *e,
                   const void *const sender )
#else
void
rkh_sma_post_lifo( RKH_SMA_T *sma, const RKH_EVT_T *e )
#endif
{

    RKH_SR_ALLOC();

    RKH_HOOK_SIGNAL( e );
    RKH_ENTER_CRITICAL_();

    RKH_INC_REF( e );

    BaseType_t rv = xQueueSendToFront( sma->equeue,  	/* event queue object */
                                       &e,    		/* actual event posted */
                                       portMAX_DELAY
                                     );

    RKH_TR_SMA_LIFO( sma, e, sender, e->pool, e->nref, 0, 0 );

    RKH_EXIT_CRITICAL_();
}

RKH_EVT_T *
rkh_sma_get( RKH_SMA_T *sma )
{
    RKH_EVT_T *e;
    RKH_SR_ALLOC();

    BaseType_t rv = xQueueReceive( sma->equeue,
                                   &e,
                                   portMAX_DELAY
                                 );

    RKH_TR_SMA_GET( sma, e, e->pool, e->nref, 0, 0 );

    return e;
}

/* ------------------------------ End of file ------------------------------ */
