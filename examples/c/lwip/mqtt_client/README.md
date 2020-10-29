# Ejemplo LwIP: Cliente MQTT

En este proeycto se ejemplifica el uso del cliente MQTT provisto por la librería LwIP. 

Luego de iniciado el stack TCP/IP, el cliente se conecta al broker MQTT público `test.mosquitto.org`, se suscribe al *topic* `ciaa/print_payload` y por último inicia una tarea encargada de publicar periódicamente en el *topic* `ciaa/publish_test` los números del 1 al 100.

Para corroborar el funcionamiento del ejemplo, se puede utilizar la herramienta `mosquitto_sub` y `mosquitto_pub`.
Con `mosquitto_sub` nos suscribimos al topic donde publica periódicamente nuestro cliente:
```mosquitto_sub -h test.mosquitto.org -t "ciaa/publish_test"```

Y con `mosquitto_pub` publicamos un mensaje en el *topic* correspondiente para que la placa lo imprima a través del puerto serie:
```mosquitto_pub -h test.mosquitto.org -t "ciaa/print_payload" -m "Hola mundo"```