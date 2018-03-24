#ifndef TIMERTICKS_H_
#define TIMERTICKS_H_

#include "Prefix.h"

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
