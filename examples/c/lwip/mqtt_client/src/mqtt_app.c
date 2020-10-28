#include "mqtt_app.h"

#include "FreeRTOS.h"
#include "task.h"

#include "lwip/opt.h"

#include "lwip/sys.h"
#include "lwip/api.h"
#include "lwip/ip_addr.h"
#include "lwip/dns.h"

#include "lwip/apps/mqtt.h"

#include "string.h"

#define BROKER_URL "test.mosquitto.org"
ip_addr_t broker;

mqtt_client_t *client_ptr;

struct mqtt_connect_client_info_t ci;

void mqtt_connect( ip_addr_t * );
dns_found_callback mqtt_brokerFound(const char *, ip_addr_t *, void *);


static mqtt_request_cb_t mqtt_publish_cb( void *arg, err_t error ){
    //printf( "Mensaje publicado" );
}


void taskPeriodicPublish( void* taskParmPtr )
{
    const char * subtopic = "ciaa/publish_test";

    char payload[5];
    
    uint8_t count = 0;

    /* 500ms periodic task */
    portTickType xPeriodicity =  500 / portTICK_RATE_MS;

    /* Init timer */
    portTickType xLastWakeTime = xTaskGetTickCount();

    while( true ) {
        itoa( count, payload, 10 );

        /* Publish count */
        mqtt_publish( client_ptr, subtopic, payload, strlen(payload), 1, 0, mqtt_publish_cb, NULL );

        /* Increment count */
        count++;
        if( count > 100 )
            count = 0;

        /* Wait 500ms */
        vTaskDelayUntil( &xLastWakeTime, xPeriodicity );
    }       
}


/* The idea is to demultiplex topic and create some reference to be used in data callbacks
   Example here uses a global variable, better would be to use a member in arg
   If RAM and CPU budget allows it, the easiest implementation might be to just take a copy of
   the topic string and use it in mqtt_incoming_data_cb
*/
static int inpub_id;
static void mqtt_incoming_publish_cb(void *arg, const char *topic, u32_t tot_len)
{
    printf( "Publish entrante con tópico %s, tamaño total: %u\n\r", topic, (unsigned int)tot_len );
  
    /* Decode topic string into a user defined reference */
    if(strcmp(topic, "ciaa/print_payload") == 0) {
        inpub_id = 0;
    }
    else if(topic[0] == 'A'){
        /* All topics starting with 'A' might be handled at the same way */
        inpub_id = 1;
    }
    else{
        /* For all other topics */
        inpub_id = 2;
    }
}


static void mqtt_incoming_data_cb(void *arg, const u8_t *data, u16_t len, u8_t flags)
{
    printf( "Publish entranto con tamaño de payload igual a %d, flags %u\n\r", len, (unsigned int)flags );
  
    if(flags & MQTT_DATA_FLAG_LAST) {
        /* Last fragment of payload received (or whole part if payload fits receive buffer
           See MQTT_VAR_HEADER_BUFFER_LEN)  */
  
        /* Call function or do action depending on reference, in this case inpub_id */
        if(inpub_id == 0){
            /* Don't trust the publisher, check zero termination */
            printf("mqtt_incoming_data_cb: %s\n\r", (const char *)data);
        }
        else if(inpub_id == 1){
          /* Call an 'A' function... */
        }
        else{
            printf("mqtt_incoming_data_cb: Ignorando payload...\n\r");
        }
    }
    else{
        /* Handle fragmented payload, store in buffer, write to file or whatever */
    }
}


static void mqtt_sub_request_cb(void *arg, err_t result)
{
  /* Just print the result code here for simplicity, 
     normal behaviour would be to take some action if subscribe fails like 
     notifying user, retry subscribe or disconnect from server */
  printf("Subscribe result: %d\n\r", result);
}


static void mqtt_connection_cb(mqtt_client_t *client, void *arg, mqtt_connection_status_t status)
{
    err_t err;
    if(status == MQTT_CONNECT_ACCEPTED){
        printf("mqtt_connection_cb: Conexión exitosa\n\r");
    
        /* Setup callback for incoming publish requests */
        mqtt_set_inpub_callback(client, mqtt_incoming_publish_cb, mqtt_incoming_data_cb, arg);
    
        /* Subscribe to a topic named "ciaa/print_payload" with QoS level 1, call mqtt_sub_request_cb with result */ 
        err = mqtt_subscribe(client, "ciaa/print_payload", 1, mqtt_sub_request_cb, arg);

        /* Init periodic publish task */
        xTaskCreate( taskPeriodicPublish, (const char *)"taskPeriodicPublish", configMINIMAL_STACK_SIZE*2, 0, tskIDLE_PRIORITY+1, 0 );

        if(err != ERR_OK) {
            printf("mqtt_subscribe retorno: %d\n\r", err);
        }
    }
    else{
        printf("mqtt_connection_cb: Desconectado: %d\n\r", status);
    
        /* Its more nice to be connected, so try to reconnect */
        mqtt_app_init();
    }  
}


void mqtt_app_init(void)
{
    /**/
    client_ptr = mqtt_client_new();

    /* Setup an empty client info structure */
    memset( &ci, 0, sizeof(ci) );
    
    /* Minimal amount of information required is client identifier, so set it here */ 
    ci.client_id = "ciaa_lwip_mqtt_test";

    while(true){
        /* MQTT broker IP */
        err_t err = dns_gethostbyname( BROKER_URL, &broker, mqtt_brokerFound, NULL );

        if( err == ERR_INPROGRESS ){
            printf( "Resolviendo nombre de dominio...\n\r" );
            break;
        }
        else if( err == ERR_ARG ){
            printf( "Cliente DNS no inicializado o nombre de dominio invalido\n\r" );
        }
        else if( err == ERR_OK ){
            printf( "Nombre de dominio en tabla local\n\r" );
            mqtt_connect( &broker );
        }
    }
}


dns_found_callback mqtt_brokerFound(const char *name, ip_addr_t *ipaddr, void *arg)
{
    if(ipaddr)
    {
        mqtt_connect( ipaddr );
    }
    else{
        printf( "Error resolviendo nombre de dominio\n\r" );

    }
}


void mqtt_connect( ip_addr_t * ipaddr )
{
    broker = *ipaddr;

    printf( "Conectando al broker...\n\r" );

    /* Initiate client and connect to server, if this fails immediately an error code is returned
    otherwise mqtt_connection_cb will be called with connection result after attempting 
    to establish a connection with the server. 
    For now MQTT version 3.1.1 is always used */
    err_t err = mqtt_client_connect( client_ptr, &broker, MQTT_PORT, mqtt_connection_cb, 0, &ci );

    /* For now just print the result code if something goes wrong */
    if( err != ERR_OK ){
        printf( "mqtt_connect retorno: %d\n", err );
    }
}