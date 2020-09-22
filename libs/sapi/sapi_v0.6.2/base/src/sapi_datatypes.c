/* Copyright 2016, Eric Pernia.
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
 */

// File creation date: 2016-06-05

//==================[inclusions]===============================================

#include "sapi_datatypes.h"

//==================[macros and definitions]===================================

//==================[internal data declaration]================================

//==================[internal functions declaration]===========================

//==================[internal data definition]=================================

//==================[external data definition]=================================

//==================[internal functions definition]============================

//==================[external functions definition]============================

// Null Function Pointer definition
// --------------------------------------
// param:  void * - Not used
// return: bool_t - Return always true
bool_t sAPI_NullFuncPtr( void* ptr )
{
   return 1;
}

//==================[ISR external functions definition]========================

//==================[end of file]==============================================

// EASTER EGG FUNCTION POINTER VECTOR EXAMPLE

/*
// Fuinction to avoid NULL pointer
void dummy(void){
    return;
}

// Function pointer typedef definition
typedef void (*voidFunctionPointer_t)(void);

// Function pointer array variable definition initialized with dummy (NULL)
voidFunctionPointer_t voidFunctionPointer[2] = {dummy, dummy};

// Execute the funcion
(* voidFunctionPointer[0] )();
(* voidFunctionPointer[1] )();

// Asign a function on each array position
voidFunctionPointer[0] = ledB;
voidFunctionPointer[1] = led1;
*/
