# Colas con freeRTOS y sAPI. 

Conectar a la UART_USB, 8N1 a 115200 baudios.

Existen 2 tareas productoras que escriben en una cola y una consumidora que permanece bloqueada hasta que hay undato en la cola, cuando hay un dato se desbloquea y lo manda por UART.
