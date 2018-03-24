/* Copyright (c) 2017, Eric Pernia.
 * All rights reserved.
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
 * =============================================================================
 *
 * Copyright (c) 2017, Eric Pernia.
 * Todos los derechos reservados.
 *
 * La redistribucion y el uso en las formas de codigo fuente y binario, con o
 * sin modificaciones, estan permitidos siempre que se cumplan las siguientes
 * condiciones:
 *
 * 1. Las redistribuciones del codigo fuente deben conservar el aviso de
 *    copyright anterior, esta lista de condiciones y el siguiente descargo de
 *    responsabilidad.
 *
 * 2. Las redistribuciones en formato binario deben reproducir el aviso de
 *    copyright anterior, esta lista de condiciones y el siguiente descargo de
 *    responsabilidad en la documentacion y/u otros materiales suministrados
 *    con la distribucion.
 *
 * 3. Ni el nombre de los titulares de derechos de autor ni los nombres de sus
 *    colaboradores pueden usarse para apoyar o promocionar productos
 *    derivados de este software sin permiso especi�fico previo y por escrito.
 *
 * ESTE SOFTWARE SE SUMINISTRA POR LOS PROPIETARIOS DEL COPYRIGHT Y
 * COLABORADORES "COMO ESTA" Y CUALQUIER GARANTIA EXPRESA O IMPLICITA,
 * INCLUYENDO, PERO NO LIMITADO A, LAS GARANTIAS IMPLICITAS DE COMERCIALIZACION
 * Y APTITUD PARA UN PROPIO�SITO PARTICULAR SON RECHAZADAS. EN NINGUN CASO LOS
 * PROPIETARIOS DEL COPYRIGHT Y COLABORADORES SERAN RESPONSABLES POR NINGUN
 * DA�O�O DIRECTO, INDIRECTO, INCIDENTAL, ESPECIAL, EJEMPLAR O COSECUENCIAL
 * (INCLUYENDO, PERO NO LIMITADO A, LA ADQUISICON O SUSTITUCION DE BIENES O
 * SERVICIOS; LA PERDIDA DE USO, DE DATOS O DE BENEFICIOS; O INTERRUPCION DE LA
 * ACTIVIDAD EMPRESARIAL) O POR CUALQUIER TEORIA DE RESPONSABILIDAD, YA SEA POR
 * CONTRATO, RESPONSABILIDAD ESTRICTA O AGRAVIO (INCLUYENDO NEGLIGENCIA O
 * CUALQUIER OTRA CAUSA) QUE SURJA DE CUALQUIER MANERA DEL USO DE ESTE SOFTWARE,
 * INCLUSO SI SE HA ADVERTIDO DE LA POSIBILIDAD DE TALES DA�OS.
 *
 * =============================================================================
 * Fecha de creacion: 2017-03-15
 * =============================================================================
 */

#ifndef _C_I18N_ES_H_
#define _C_I18N_ES_H_

/*==================[inclusiones]============================================*/

/*==================[c++]====================================================*/

#ifdef __cplusplus
extern "C" {
#endif

/*==================[macros]=================================================*/

/*
#include <stdio.h>
#include <stdint.h>
*/

#define definir                                   define
#define incluir                                   include
#define siDefinido                                ifdef
#define siNoDefinido                              ifndef
#define finSi                                     endif

#define funcionPrincipal                          main

#define vacio                                     void

#define entero                                    int
#define corto                                     short
#define largo                                     long
#define caracter                                  char
#define flotante                                  float
#define dobleFlotante                             double

#define sinSigno                                  unsigned
#define registro                                  register
#define volatil                                   volatile
#define externa                                   extern
#define privada                                   static

#define si                                        if
#define siNo                                      else
#define conmutar                                  switch
#define elegir                                    switch
#define caso                                      case
#define casoPorDefecto                            default
#define hacer                                     do
#define mientras                                  while
#define repetirPara                               for

#define retornar                                  return
#define continuar                                 continue
#define romper                                    break

#define imprimir                                  printf

/*==================[tipos de datos declarados por el usuario]===============*/

/*==================[declaraciones de datos externos]========================*/

/*==================[declaraciones de funciones externas]====================*/

/*==================[c++]====================================================*/
#ifdef __cplusplus
}
#endif

/*==================[end of file]============================================*/
#endif /* _C_I18N_ES_H_ */
