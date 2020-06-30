/* Copyright 2017, Agustin Bassi.
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
 *
    Copyright 2001 Georges Menie
    https://www.menie.org/georges/embedded/small_printf_source_code.html

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
	putchar is the only external dependency for this file,
	if you have a working putchar, just remove the following
	define. If the function should be called something else,
	replace outbyte(c) by your own function call.
 */

/*
 * Date: 2017-11-23
 */

/*==================[inlcusiones]============================================*/

#include "sapi.h"        // <= sAPI header

#include "seos_pont_2014_isr.h"       // <= dispatcher and task management header
#include "seos_pont_2014_scheduler.h" // <= scheduler and system initialization header

/*==================[definiciones y macros]==================================*/

CONSOLE_PRINT_ENABLE

// Tiempos de coccion asociados a las teclas
#define TIEMPO_COCCION_1		10
#define TIEMPO_COCCION_2		20
#define TIEMPO_COCCION_3		30

/**
 * En este enum se definen las entradas y
 * salidas que tendra el microondas de una
 * manera descriptiva. Notar que los valores
 * son los mismos que los pines que utiliza
 * la biblioteca sAPI.
 */
typedef enum PinMicroondas {
	TEC_20_SEG   = TEC1,//!< TEC_20_SEG
	TEC_40_SEG   = TEC2,//!< TEC_40_SEG
	TEC_60_SEG   = TEC3,//!< TEC_60_SEG
	TEC_COMENZAR = TEC4,//!< TEC_COMENZAR
	MOTOR        = LEDB,//!< MOTOR
	LUZ          = LED1,//!< LUZ
	ALARMA       = LED3 //!< ALARMA
} PinMicroondas_t;

// Nuevo tipo de datos enumerado llamado estadoMEF
typedef enum{
	_BUTTON_UP,      // Mientras el boton esta liberado.
	_BUTTON_FALLING, // Mientras esta ocurriendo el flanco descendente, hace el anti-rebote.
	_BUTTON_RISING,  // Mientras esta ocurriendo el flanco ascendente, hace el anti-rebote.
	_BUTTON_DOWN     // Mientras el boton esta presionado.
} EstadoMefDebounce_t;

/*==================[definiciones de datos internos]=========================*/

/** Variable que se va a setear con distintos delay en funcion del
 * tiempo seleccionado con las teclas TEC_20_SEG, TEC_40_SEG, TEC_60_SEG */
static int32_t TiempoCoccion = TIEMPO_COCCION_1;
static bool_t FlagFuncionamiento = FALSE;

/*==================[definiciones de datos externos]=========================*/

/*==================[declaraciones de funciones internas]====================*/

static void	EjecutarMefDebounceParametric	( EstadoMefDebounce_t * estadoMefDebounce, uint8_t teclaToRead);

/*==================[declaraciones de funciones externas]====================*/

void TareaTec1( void* taskParam );
void TareaTec2( void* taskParam );
void TareaTec3( void* taskParam );
void TareaTec4( void* taskParam );
void TareaRealizarCoccion( void* taskParam );
void TareaMostrarTiempo( void* taskParam );

/*==================[funcion principal]======================================*/

/**
 * @brief Descripcion del funcionamiento del microondas con SO Pont.
 *
 * - Las teclas TEC1, TEC2 y TEC3 ingresan un tiempo de coccion del
 *   microondas de 20, 40 y 60 segundos respectivamente.
 * - La tecla TEC4 confirma el comienzo del funcionamiento del microondas.
	 Mientras esta funcionando el microondas se debe:
		- Encender el motor del microondas mediante el LEDB.
		- Encender la luz del microondas mediante el LED1.
		- Mostrar en la UART USB el tiempo restante para terminar la coccion.
		- Decrementar el valor del tiempo de coccion.
	 Cuando paso el tiempo (se llego a 0) se debe prender la alarma
	 mediante el LED3 avisando al usuario que la coccion esta realizada.
*/

int main( void ){

	// ---------- CONFIGURACIONES ------------------------------
	// Inicializar y configurar la plataforma
	boardConfig();

	uartConfig( UART_USB, 115200 );

	// FUNCION que inicializa el planificador de tareas
   schedulerInit();

	schedulerAddTask( TareaTec1, 0, 0, 40 );
	schedulerAddTask( TareaTec2, 0, 10, 40 );
	schedulerAddTask( TareaTec3, 0, 20, 40 );
	schedulerAddTask( TareaTec4, 0, 30, 40 );
	schedulerAddTask( TareaRealizarCoccion, 0, 40, 1000 );
	schedulerAddTask( TareaMostrarTiempo, 0, 50, 1000 );

	schedulerStart( 1 );

	// ---------- REPETIR POR SIEMPRE --------------------------
	while( TRUE ){
		schedulerDispatchTasks();
	}
	return 0;
}

/*==================[definiciones de funciones internas]=====================*/

/**
 * Maquina de estados parametrizable para la lectura de teclas con antirrebote.
 * @param estadoMefDebounce estado anterior de la tecla
 * @param teclaToRead tecla a leer
 */
static void	EjecutarMefDebounceParametric( 
               EstadoMefDebounce_t * estadoMefDebounce, 
               uint8_t teclaToRead){
	switch(*estadoMefDebounce) {
		case _BUTTON_UP:
			if( !gpioRead(teclaToRead) ){
				*estadoMefDebounce = _BUTTON_FALLING;
			}
			break;
		case _BUTTON_FALLING:
			if( !gpioRead(teclaToRead) ){
				*estadoMefDebounce = _BUTTON_DOWN;
			} else{
				*estadoMefDebounce = _BUTTON_UP;
			}
			break;
		case _BUTTON_RISING:  // Mientras esta ocurriendo el flanco ascendente,
			if( gpioRead(teclaToRead) ){
				*estadoMefDebounce = _BUTTON_UP;
			} else{
				*estadoMefDebounce = _BUTTON_DOWN;
			}
			break;
		case _BUTTON_DOWN:
			if( gpioRead(teclaToRead) ){
				*estadoMefDebounce = _BUTTON_RISING;
			}
			break;
		default:
			*estadoMefDebounce = _BUTTON_UP;
			break;
	}
}

/*==================[definiciones de funciones externas]=====================*/

void TareaTec1( void* taskParam ){
   static EstadoMefDebounce_t estadoDebounce = BUTTON_UP;
	EjecutarMefDebounceParametric(&estadoDebounce, TEC_20_SEG);
	if (estadoDebounce == BUTTON_DOWN){
		TiempoCoccion = TIEMPO_COCCION_1;
	}
}

void TareaTec2( void* taskParam ){
   static EstadoMefDebounce_t estadoDebouce = BUTTON_UP;
	EjecutarMefDebounceParametric(&estadoDebouce, TEC_40_SEG);
	if (estadoDebouce == BUTTON_DOWN){
		TiempoCoccion = TIEMPO_COCCION_2;
	}
}

void TareaTec3( void* taskParam ){
   static EstadoMefDebounce_t estadoDebounce = BUTTON_UP;
	EjecutarMefDebounceParametric(&estadoDebounce, TEC_60_SEG);
	if (estadoDebounce == BUTTON_DOWN){
		TiempoCoccion = TIEMPO_COCCION_3;
	}
}

void TareaTec4( void* taskParam ){
   static EstadoMefDebounce_t estadoDebounce = BUTTON_UP;
	EjecutarMefDebounceParametric(&estadoDebounce, TEC_COMENZAR);
	if (estadoDebounce == BUTTON_DOWN){
		FlagFuncionamiento = TRUE;
		gpioWrite(MOTOR, TRUE);
		gpioWrite(LUZ, TRUE);
		gpioWrite(ALARMA, FALSE);
	}
}

void TareaRealizarCoccion( void* taskParam ){
	if (FlagFuncionamiento == TRUE){
		if (--TiempoCoccion == 0){
			gpioWrite(MOTOR, FALSE);
			gpioWrite(LUZ, FALSE);
		} else if (TiempoCoccion < 0 && TiempoCoccion > -6){
			gpioToggle(ALARMA);
		} else if (TiempoCoccion < -7){
			gpioWrite(ALARMA, FALSE);
			FlagFuncionamiento = FALSE;
		}
	}
}

void TareaMostrarTiempo( void* taskParam ){
	if (FlagFuncionamiento == TRUE && TiempoCoccion >=0){
		consolePrintString("Tiempo restante: ");
		consolePrintInt(TiempoCoccion);
		consolePrintString("\n\r");
	}
}

/*==================[fin del archivo]========================================*/
