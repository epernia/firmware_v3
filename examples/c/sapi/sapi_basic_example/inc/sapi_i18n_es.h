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

#ifndef _SAPI_I18N_ES_H_
#define _SAPI_I18N_ES_H_

/*==================[inclusiones]============================================*/

/*==================[c++]====================================================*/

#ifdef __cplusplus
extern "C" {
#endif

/*==================[macros]=================================================*/

#include "sapi.h"

#define CIERTO                                    TRUE
#define VERDADERO                                 TRUE
#define FALSO                                     FALSE

#define ENCENDER                                  ON
#define APAGAR                                    OFF

#define ALTO                                      HIGH
#define BAJO                                      LOW

#define booleano_t                                bool_t

#define enteroSinSigno8_t                         uint8_t
#define enteroSinSigno16_t                        uint16_t
#define enteroSinSigno32_t                        uint32_t
#define enteroSinSigno64_t                        uint64_t

#define entero8_t                                 int8_t
#define entero16_t                                int16_t
#define entero32_t                                int32_t
#define entero64_t                                int64_t

#define contadorDeBaseDeTiempo_t                  tick_t

#define sAPI_PunteroAFuncion_t                    sAPI_FuncPtr_t
#define sAPI_PunteroAFuncionNula                  sAPI_NullFuncPtr


#define mapaDePines_t                             gpioMap_t
#define mapaDeConversoresAD_t                     adcMap_t
#define mapaDeConversoresDA_t                     dacMap_t
#define mapaDeUARTs_t                             uartMap_t
#define mapaDeTemporizadores_t                    timerMap_t
#define mapaDeI2Cs_t                              i2cMap_t
#define mapaDePWMs_t                              pwmMap_t
#define mapaDeSERVOs_t                            servoMap_t

#define HABILITAR_CONVERSOR_AD                    ADC_ENABLE
#define DESHABILITAR_CONVERSOR_AD                 ADC_DISABLE
#define configurarConversorAD_t                   adcConfig_t
#define configurarConversorAD                     adcConfig
#define leerConversorAD                           adcRead

#define configurarPlataforma                      boardConfig


#define HABILITAR_CONVERSOR_DA                    DAC_ENABLE
#define DESHABILITAR_CONVERSOR_DA                 DAC_DISABLE
#define configurarConversorDA_t                   dacConfig_t
#define configurarConversorDA                     dacConfig
#define escribirConversorDA                       dacWrite

#define retardo_t                                 delay_t
#define retardoInexacto                           delayInaccurate
#define retardoInexactoUs                         delayInaccurateUs
#define retardo                                   delay
#define configurarRetardo                         delayConfig
#define seCumplioElRetardo                        delayRead
#define escribirRetardo                           delayWrite

#define HABILITAR_PINES                           GPIO_ENABLE
#define ENTRADA                                   GPIO_INPUT
#define SALIDA                                    GPIO_OUTPUT
#define ENTRADA_CON_PULLUP                        GPIO_INPUT_PULLUP
#define ENTRADA_CON_PULLDOWN                      GPIO_INPUT_PULLDOWN
#define ENTRADA_CON_PULLUP_Y_PULLDOWN             GPIO_INPUT_PULLUP_PULLDOWN
#define configurarPin_t                           gpioConfig_t
#define configurarPin                             gpioConfig
#define leerPin                                   gpioRead
#define escribirPin                               gpioWrite
#define intercambiarPin                           gpioToggle

#define inicializarI2c                            i2cInit
#define leerI2c                                   i2cRead
#define escribirI2c                               i2cWrite
#define escribirLeerI2c                           i2cWriteRead

#define HABILITAR_PWM                             PWM_ENABLE
#define DESHABILITAR_PWM                          PWM_DISABLE
#define HABILITAR_SALIDA_PWM                      PWM_ENABLE_OUTPUT
#define DESHABILITAR_SALIDA_PWM                   PWM_DISABLE_OUTPUT
#define configurarPwm_t                           pwmConfig_t
#define configurarPwm                             pwmConfig
#define leerPwm                                   pwmRead
#define escribirPwm                               pwmWrite

#define relojDeTiempoReal_t                       rtc_t
#define inicializarRelojDeTiempoReal              rtcInit
#define leerRelojDeTiempoReal                     rtcRead
#define escribirRelojDeTiempoReal                 rtcWrite

#define HABILITAR_SERVO                           SERVO_ENABLE
#define DESHABILITAR_SERVO                        SERVO_DISABLE
#define HABILITAR_SALIDA_SERVO                    SERVO_ENABLE_OUTPUT
#define DESHABILITAR_SALIDA_SERVO                 SERVO_DISABLE_OUTPUT
#define inicializarServo_t                        servoInit_t
#define inicializarServo                          servoInit
#define leerServo                                 servoRead
#define escribirServo                             servoWrite

#define dormirHastaLaProximaInterrupcion          sleepUntilNextInterrupt

#define inicializarBaseDeTiempo                   tickInit
#define leerContadorDeBaseDeTiempo                tickRead
#define escribirContadorDeBaseDeTiempo            tickWrite

#define estadoDeEsperarARecibirCadenaOTiempoCumplido_t   waitForReceiveStringOrTimeoutState_t
#define esperarARecibirCadenaOTiempoCumplido_t           waitForReceiveStringOrTimeout_t
#define esperarARecibirCadenaOTiempoCumplido             waitForReceiveStringOrTimeout
#define esperarARecibirCadenaOTiempoCumplidoBloqueando   waitForReceiveStringOrTimeoutBlocking
#define inicializarUart                                   uartInit
#define recibirBytePorUart                               uartReadByte
#define eviarBytePorUart                                 uartWriteByte
#define eviarCadenaPorUart                               uartWriteString

#define APAGAR_DISPLAY_7_SEGMENTOS                DISPLAY_7_SEGMENT_OFF
#define testearPinesDeDisplay7Segmentos           display7SegmentTestPins
#define inicializarPinesDeDisplay7Segmentos       display7SegmentPinInit
#define escribirDisplay7Segmentos                 display7SegmentWrite

#define tecladoMatricial_t                        keypad_t
#define inicializarTecladoMatricial               keypadInit
#define leerTecladoMatricial                      keypadRead


/*==================[tipos de datos declarados por el usuario]===============*/

/*==================[declaraciones de datos externos]========================*/

/*==================[declaraciones de funciones externas]====================*/

/*==================[c++]====================================================*/
#ifdef __cplusplus
}
#endif

/*==================[end of file]============================================*/
#endif /* _SAPI_I18N_ES_H_ */
