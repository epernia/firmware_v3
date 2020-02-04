# Statechart example using sysTick and Yakindu Time Events


- Description: The Statechart example simply toggle an LED at a periodic rate using the sysTick timer and Yakindu Time Events (Timer Ticks).

	- The sysTick Interrupt Handler only set a Flag.
	
	- LED state is toggled in the main program. 
		- The uC sleeps waiting for an interruption.
	
		- When a interrupt wakes to the uC, the main program validates it,
		  checking the waited Flag.
			- Then reset its Flag.
			- Then Update all Timer Ticks.
			- Then Scan all Timer Ticks and if there are pending events
				- Then Raise an Event -> Ticks.evid => OK
				- Then Mark as Attached -> Ticks.evid => OK
			- Then Run an Cycle of Statechart.
		
		- And then the uC sleeps again. 
