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
#include "bsp.h"
#include "sapi.h"

RKH_THIS_MODULE

/* ----------------------------- Local macros ------------------------------ */
/* ------------------------------- Constants ------------------------------- */
/* ---------------------------- Local data types --------------------------- */
/* ---------------------------- Global variables --------------------------- */
/* ---------------------------- Local variables ---------------------------- */
/* ----------------------- Local function prototypes ----------------------- */
static bool_t tickHook( void *p );

/* ---------------------------- Local functions ---------------------------- */
static bool_t
tickHook( void *p )
{
    RKH_TIM_TICK();
}

/* ---------------------------- Global functions --------------------------- */


/**
   @brief Inicializa los hooks del sistema
          En este caso, utiliza la API de sapi para declarar un tick periodico
          que le de soporte al timing de RKH.
 */
void rkh_hook_start( void )
{
    /* uses sapi for the tick processing */
    tickInit( BSP_TICK_RATE_MS );

    /* defines the callback when the the tick is signalled */
    tickCallbackSet( tickHook , 0 );
}

/**
   @brief

 */
void rkh_hook_exit( void )
{
    RKH_TRC_FLUSH();
}


void rkh_hook_timetick( void )
{
    bsp_timeTick();
}

void rkh_hook_idle( void )           /* called within critical section */
{
    RKH_ENA_INTERRUPT();
    RKH_TRC_FLUSH();
}

/* ------------------------------ File footer ------------------------------ */
