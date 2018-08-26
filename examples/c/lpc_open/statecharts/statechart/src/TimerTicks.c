#include "Prefix.h"

#include "TimerTicks.h"


void InitTimerTicks(TimerTicks *list, uint32_t len)
{
	uint32_t i;

	for(i = 0; i < len; i++) {
		list[i].evid = (sc_eventid)0;
		list[i].time_ms = 0;
		list[i].count = 0;
		list[i].periodic = false;
		list[i].active = false;
		list[i].evPending = false;
	}
}


sc_eventid SetNewTimerTick(TimerTicks *list, uint32_t len, sc_eventid evid, sc_uinteger time_ms, sc_boolean periodic)
{
	uint32_t i;

	sc_eventid ret = (sc_eventid)0;
	for(i = 0; i < len; i++) {
		if(!list[i].evid) {
			list[i].time_ms = time_ms;
			list[i].count = time_ms;
			list[i].evid = evid;
			list[i].periodic = periodic;
			list[i].active = true;
			list[i].evPending = false;
			ret = evid;
			break;
		}
	}
	return ret;
}


void* UnsetTimerTick(TimerTicks *list, uint32_t len, sc_eventid id)
{
	uint32_t i;
	sc_eventid ret = (sc_eventid)0;

	for(i = 0; i < len; i++) {
		if(list[i].evid == id) {
			list[i].evid = (sc_eventid)0;
			list[i].time_ms = 0;
			list[i].count = 0;
			list[i].periodic = false;
			list[i].active = false;
			list[i].evPending = false;
			ret = id;
			break;
		}
	}
	return ret;
}


sc_boolean IsPendEvent(TimerTicks *list, uint32_t len, sc_eventid id)
{
	int i;
	sc_boolean ret = false;
	for(i=0; i < len; i++) {
		if((list[i].evid == id) && (list[i].evPending == true)) {
			ret = true;
			break;
		}
	}
	return ret;
}


void MarkAsAttEvent(TimerTicks *list, uint32_t len, sc_eventid id)
{
	uint32_t i;

	for(i = 0; i < len; i++) {
		if(list[i].evid == id) {
			list[i].evPending = false;
			break;
		}
	}
}


void UpdateTimers(TimerTicks *list, uint32_t len)
{
	uint32_t i;

	for(i = 0; i < len; i++) {
		if(list[i].active == true) {
			if(list[i].count)
				list[i].count--;
			if(!list[i].count) {
				if(list[i].periodic)
					list[i].count = list[i].time_ms;
				else
					list[i].active = false;
				list[i].evPending = true;
			}
		}
	}
}
