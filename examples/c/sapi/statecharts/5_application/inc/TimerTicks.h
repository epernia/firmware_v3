/* Copyright 2017, Juan Esteban Alarcon, Juan Manuel Cruz
 * All rights reserved.
 *
 * This file is part of Workspace.
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

#ifndef TIMERTICKS_H_
#define TIMERTICKS_H_


typedef struct
{
	sc_eventid evid;
	sc_uinteger time_ms;
	sc_uinteger count;
	sc_boolean periodic;
	sc_boolean active;
	sc_boolean evPending;
} TimerTicks;


void InitTimerTicks(TimerTicks *list, uint32_t len);
sc_eventid SetNewTimerTick(TimerTicks *list, uint32_t len, sc_eventid evid, sc_uinteger time_ms, sc_boolean periodic);
sc_eventid UnsetTimerTick(TimerTicks *list, uint32_t len, sc_eventid evid);
sc_boolean IsPendEvent(TimerTicks *list, uint32_t len, sc_eventid evid);
void MarkAsAttEvent(TimerTicks *list, uint32_t len, sc_eventid evid);
void UpdateTimers(TimerTicks *list, uint32_t len);

#endif /* TIMERTICKS_H_ */
