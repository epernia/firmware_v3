# Statechart example using sysTick and Yakindu Time Events


- Description: The Statechart example simply toggle an LED by pressing a button using the sysTick timer and Yakindu Time Events (Timer Ticks), include software debounce.

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
				
			- Then Get status of buttons
			- Then if there are a pressed button
				- Then Raise an Event -> evTECXOprimodo => OK,
				  and Value of pressed button -> viTecla
			-  Then else Raise an Event -> evTECXNoOprimido => OK

			- Then Run an Cycle of Statechart.
		
		- And then the uC sleeps again. 
