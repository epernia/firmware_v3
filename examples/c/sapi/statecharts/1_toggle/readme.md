# Statechart example using sysTick


- Description: The Statechart example simply toggle an LED at a periodic rate using the sysTick timer.

	- The sysTick Interrupt Handler only set a Flag.
	
	- LED state is toggled in the main program. 
		- The uC sleeps waiting for an interruption.
	
		- When a interrupt wakes to the uC, the main program validates it,
		  checking the waited Flag.
			- Then reset its Flag.
			- Then Raise an Event -> evTick => OK.
			- Then Run an Cycle of Statechart.
		
		- And then the uC sleeps again. 
