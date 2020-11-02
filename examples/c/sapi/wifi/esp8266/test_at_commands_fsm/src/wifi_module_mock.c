//=====[Libraries]=============================================================

#include "wifi_module.h"
#include "wifi_default_credentials.h"
#include "sapi.h"
#include "string.h"

//=====[Declaration of private defines]========================================

#define ESP8266_BAUD_RATE                    115200
#define ESP8266_MOST_COMMON_AT_CMD_TIMEOUT   50
#define ESP8266_AT_RST_CMD_TIMEOUT           10000
#define ESP8266_AT_CWJAP_CMD_TIMEOUT         20000

//=====[Declaration of private data types]=====================================

typedef enum{   
    ESP8266_IDLE,
    ESP8266_PROCESSING_AT_COMMAND,
}esp8266State_t;

// "AT+CIPSTATUS\r\n"
// status of the ESP32 Station interface.
typedef enum{
    ESP8266_STATUS_AP_IP = 2, // 2: The ESP32 Station is connected to an AP and
                              //    its IP is obtained.
    ESP8266_STATUS_TCP_UDP_CREATED = 3, // 3: The ESP32 Station has created a
                                        //    TCP or UDP transmission.
    ESP8266_STATUS_TCP_UDP_DISCONNECTED = 4, // 4: The TCP or UDP transmission
                                             //    of ESP32 Station is
                                             //    disconnected.
    ESP8266_STATUS_AP_NOT_CONNECTED = 5, // 5: The ESP32 Station does NOT
                                         // connect to an AP.
}esp8266StationStatus_t;

//=====[Declaration and initialization of public global objects]===============

//static Serial uartEsp8266( D42, D41 );

//=====[Declaration of external public global variables]=======================

//=====[Declaration and initialization of public global variables]=============

//=====[Declaration and initialization of private global variables]============

static char credential_ssid[WIFI_MODULE_CREDENTIAL_MAX_LEN] = WIFI_SSID; 
static char credential_password[WIFI_MODULE_CREDENTIAL_MAX_LEN] = WIFI_PASSWORD;

static parser_t parser;
static parserStatus_t parserStatus;

static parser_t parser2;
static parserStatus_t parser2Status;

static esp8266State_t esp8266State;

static wifiModuleRequestResult_t status = WIFI_MODULE_IS_NOT_CONNECTED;

//=====[Declarations (prototypes) of private functions]========================

static bool esp8266UartByteRead( char* receivedByte );
static void esp8266UartByteWrite( char byteToSend );
static void esp8266UartStringWrite( char const* str );

// Send commands that have only one valid response
static wifiModuleRequestResult_t esp8266SendCommandWithResponse( 
    char const* cmd, char const* cmdResponse,
    wifiModuleRequestResult_t informResult,
    tick_t timeout );

// Check response for previously sended commands that only have one response
static wifiModuleRequestResult_t
esp8266CheckCommandResponse( wifiModuleRequestResult_t resultMatch );

// Send commands that have two valid responses
static wifiModuleRequestResult_t esp8266SendCommandWithTwoResponses( 
    char const* cmd, char const* cmdResponse1, char const* cmdResponse2,
    wifiModuleRequestResult_t informResult,
    tick_t timeout );

//=====[Implementations of public functions]===================================

// Init module and status -----------------------------------------------------

void wifiModuleInit()
{  
//    uartEsp8266.baud(ESP8266_BAUD_RATE);
    esp8266State = ESP8266_IDLE;
}

// Update module status -------------------------------------------------------

void wifiModuleUpdate() {
    //esp8266Update();
}

// Set/Get AP credentials -----------------------------------------------------

// Responses:
// WIFI_MODULE_AP_SSID_SAVED
// WIFI_MODULE_AP_SSID_NOT_SAVED
wifiModuleRequestResult_t wifiModuleSetAP_SSID( char const* ssid )
{
    if( *ssid == '\0' ) {
        return WIFI_MODULE_AP_SSID_NOT_SAVED;
    }
    int i=0;
    while ( *ssid != '\0' && i<WIFI_MODULE_CREDENTIAL_MAX_LEN ) {
        credential_ssid[i] = *ssid;
        ssid++;
        i++;
    }
    if( i >= WIFI_MODULE_CREDENTIAL_MAX_LEN ) {
        return WIFI_MODULE_AP_SSID_NOT_SAVED;
    }
    return WIFI_MODULE_AP_SSID_SAVED;
}

// Responses:
// WIFI_MODULE_AP_PASSWORD_SAVED
// WIFI_MODULE_AP_PASSWORD_NOT_SAVED
wifiModuleRequestResult_t wifiModuleSetAP_Password( char const* password )
{
    if( *password == '\0' ) {
        return WIFI_MODULE_AP_PASSWORD_NOT_SAVED;
    }
    int i=0;
    while ( *password != '\0' && i<WIFI_MODULE_CREDENTIAL_MAX_LEN ) {
        credential_password[i] = *password;
        password++;
        i++;
    }
    if( i >= WIFI_MODULE_CREDENTIAL_MAX_LEN ) {
        return WIFI_MODULE_AP_PASSWORD_NOT_SAVED;
    }
    return WIFI_MODULE_AP_PASSWORD_SAVED;
}

char const* wifiModuleGetAP_SSID()
{
    return (char const*) credential_ssid;
}

char const* wifiModuleGetAP_Password()
{
    return (char const*) credential_password;
}

// Detect module --------------------------------------------------------------

// Responses:
// WIFI_MODULE_DETECTION_STARTED
// WIFI_MODULE_BUSY
wifiModuleRequestResult_t wifiModuleStartDetection()
{/*
    char receivedChar = '\0';
    pcSerialComStringWrite( "wifiModuleStartDetection()\r\n" );
    printf("Type:\r\n");
    printf("  '1' for WIFI_MODULE_DETECTION_STARTED\r\n");
    printf("  Other for WIFI_MODULE_BUSY\r\n");
    while( receivedChar == '\0' ) receivedChar = pcSerialComCharRead();
    switch ( receivedChar ) {
        case '1': return WIFI_MODULE_DETECTION_STARTED; break;
        default:  return WIFI_MODULE_BUSY; break;
    }*/
    return WIFI_MODULE_DETECTION_STARTED;
}

// Responses:
// WIFI_MODULE_DETECTED
// WIFI_MODULE_NOT_DETECTED
// WIFI_MODULE_BUSY
wifiModuleRequestResult_t wifiModuleDetectionResponse()
{
    char receivedChar = '\0';
    pcSerialComStringWrite( "wifiModuleDetectionResponse()\r\n" );
    printf("Type:\r\n");
    printf("  '1' for WIFI_MODULE_DETECTED\r\n");
    printf("  '2' for WIFI_MODULE_NOT_DETECTED\r\n");
    printf("  Other for WIFI_MODULE_BUSY\r\n");
    while( receivedChar == '\0' ) receivedChar = pcSerialComCharRead();
    switch ( receivedChar ) {
        case '1': return WIFI_MODULE_DETECTED; break;
        case '2': return WIFI_MODULE_NOT_DETECTED; break;
        default:  return WIFI_MODULE_BUSY; break;
    }
    //return WIFI_MODULE_DETECTED;
}

// Reset module ---------------------------------------------------------------

// Responses:
// WIFI_MODULE_RESET_STARTED
// WIFI_MODULE_BUSY
wifiModuleRequestResult_t wifiModuleStartReset()
{/*
    char receivedChar = '\0';
    pcSerialComStringWrite( "wifiModuleStartReset()\r\n" );
    printf("Type:\r\n");
    printf("  '1' for WIFI_MODULE_RESET_STARTED\r\n");
    printf("  Other for WIFI_MODULE_BUSY\r\n");
    while( receivedChar == '\0' ) receivedChar = pcSerialComCharRead();
    switch ( receivedChar ) {
        case '1': return WIFI_MODULE_RESET_STARTED; break;
        default:  return WIFI_MODULE_BUSY; break;
    }*/
    return WIFI_MODULE_RESET_STARTED;
}

// Responses:
// WIFI_MODULE_RESET_COMPLETE
// WIFI_MODULE_NOT_DETECTED
// WIFI_MODULE_BUSY
wifiModuleRequestResult_t wifiModuleResetResponse()
{
    char receivedChar = '\0';
    pcSerialComStringWrite( "wifiModuleResetResponse()\r\n" );
    printf("Type:\r\n");
    printf("  '1' for WIFI_MODULE_RESET_COMPLETE\r\n");
    printf("  '2' for WIFI_MODULE_NOT_DETECTED\r\n");
    printf("  Other for WIFI_MODULE_BUSY\r\n");
    while( receivedChar == '\0' ) receivedChar = pcSerialComCharRead();
    switch ( receivedChar ) {
        case '1': return WIFI_MODULE_RESET_COMPLETE; break;
        case '2': return WIFI_MODULE_NOT_DETECTED; break;
        default:  return WIFI_MODULE_BUSY; break;
    }
    //return WIFI_MODULE_RESET_COMPLETE;
}

// Initialize module ----------------------------------------------------------

// Responses:
// WIFI_MODULE_INIT_STARTED
// WIFI_MODULE_BUSY
wifiModuleRequestResult_t wifiModuleStartInit()
{/*
    char receivedChar = '\0';
    pcSerialComStringWrite( "wifiModuleStartInit()\r\n" );
    printf("Type:\r\n");
    printf("  '1' for WIFI_MODULE_INIT_STARTED\r\n");
    printf("  Other for WIFI_MODULE_BUSY\r\n");
    while( receivedChar == '\0' ) receivedChar = pcSerialComCharRead();
    switch ( receivedChar ) {
        case '1': return WIFI_MODULE_INIT_STARTED; break;
        default:  return WIFI_MODULE_BUSY; break;
    }*/
    return WIFI_MODULE_INIT_STARTED;
}

// Responses:
// WIFI_MODULE_INIT_COMPLETE
// WIFI_MODULE_NOT_DETECTED
// WIFI_MODULE_BUSY
wifiModuleRequestResult_t wifiModuleInitResponse()
{
    char receivedChar = '\0';
    pcSerialComStringWrite( "wifiModuleInitResponse()\r\n" );
    printf("Type:\r\n");
    printf("  '1' for WIFI_MODULE_INIT_COMPLETE\r\n");
    printf("  '2' for WIFI_MODULE_NOT_DETECTED\r\n");
    printf("  Other for WIFI_MODULE_BUSY\r\n");
    while( receivedChar == '\0' ) receivedChar = pcSerialComCharRead();
    switch ( receivedChar ) {
        case '1': return WIFI_MODULE_INIT_COMPLETE; break;
        case '2': return WIFI_MODULE_NOT_DETECTED; break;
        default:  return WIFI_MODULE_BUSY; break;
    }
    //return WIFI_MODULE_INIT_COMPLETE;
}

// AP connection --------------------------------------------------------------

// Check if connected with AP

// Responses:
// WIFI_MODULE_IS_CONNECTED_AP_STARTED
// WIFI_MODULE_BUSY
wifiModuleRequestResult_t wifiModuleStartIsConnectedWithAP()
{/*
    char receivedChar = '\0';
    pcSerialComStringWrite( "wifiModuleStartIsConnectedWithAP()\r\n" );
    printf("Type:\r\n");
    printf("  '1' for WIFI_MODULE_IS_CONNECTED_AP_STARTED\r\n");
    printf("  Other for WIFI_MODULE_BUSY\r\n");
    while( receivedChar == '\0' ) receivedChar = pcSerialComCharRead();
    switch ( receivedChar ) {
        case '1': return WIFI_MODULE_IS_CONNECTED_AP_STARTED; break;
        default:  return WIFI_MODULE_BUSY; break;
    }*/
    return WIFI_MODULE_IS_CONNECTED_AP_STARTED;
}

// Responses:
// WIFI_MODULE_IS_CONNECTED
// WIFI_MODULE_IS_NOT_CONNECTED
// WIFI_MODULE_NOT_DETECTED
// WIFI_MODULE_BUSY
wifiModuleRequestResult_t wifiModuleIsConnectedWithAPResponse()
{
    char receivedChar = '\0';
    pcSerialComStringWrite( "wifiModuleIsConnectedWithAPResponse()\r\n" );
    printf("Type:\r\n");
    printf("  '1' for WIFI_MODULE_IS_CONNECTED\r\n");
    printf("  '2' for WIFI_MODULE_IS_NOT_CONNECTED\r\n");
    printf("  '3' for WIFI_MODULE_NOT_DETECTED\r\n");
    printf("  Other for WIFI_MODULE_BUSY\r\n");
    while( receivedChar == '\0' ) receivedChar = pcSerialComCharRead();
    switch ( receivedChar ) {
        case '1': return WIFI_MODULE_IS_CONNECTED; break;
        case '2': return WIFI_MODULE_IS_NOT_CONNECTED; break;
        case '3': return WIFI_MODULE_NOT_DETECTED; break;
        default:  return WIFI_MODULE_BUSY; break;
    }
    //return WIFI_MODULE_IS_CONNECTED;
}

// Connect with AP

// Responses:
// WIFI_MODULE_CONNECT_AP_STARTED
// WIFI_MODULE_BUSY
wifiModuleRequestResult_t wifiModuleStartConnectWithAP()
{/*
    char receivedChar = '\0';
    pcSerialComStringWrite( "wifiModuleStartConnectWithAP()\r\n" );
    printf("Type:\r\n");
    printf("  '1' for WIFI_MODULE_CONNECT_AP_STARTED\r\n");
    printf("  Other for WIFI_MODULE_BUSY\r\n");
    while( receivedChar == '\0' ) receivedChar = pcSerialComCharRead();
    switch ( receivedChar ) {
        case '1': return WIFI_MODULE_CONNECT_AP_STARTED; break;
        default:  return WIFI_MODULE_BUSY; break;
    }*/
    return WIFI_MODULE_CONNECT_AP_STARTED;
}

// Responses:
// WIFI_MODULE_IS_CONNECTED
// WIFI_MODULE_IS_NOT_CONNECTED
// WIFI_MODULE_CONNECT_AP_ERR_TIMEOUT
// WIFI_MODULE_CONNECT_AP_ERR_WRONG_PASS
// WIFI_MODULE_CONNECT_AP_ERR_AP_NOT_FOUND
// WIFI_MODULE_CONNECT_AP_ERR_CONN_FAIL
// WIFI_MODULE_NOT_DETECTED
// WIFI_MODULE_BUSY
wifiModuleRequestResult_t wifiModuleConnectWithAPResponse()
{
    char receivedChar = '\0';
    pcSerialComStringWrite( "wifiModuleConnectWithAPResponse()\r\n" );
    printf("Type:\r\n");
    printf("  '1' for WIFI_MODULE_IS_CONNECTED\r\n");
    printf("  '2' for WIFI_MODULE_IS_NOT_CONNECTED\r\n");
    printf("  '3' for WIFI_MODULE_CONNECT_AP_ERR_TIMEOUT\r\n");
    printf("  '4' for WIFI_MODULE_CONNECT_AP_ERR_WRONG_PASS\r\n");
    printf("  '5' for WIFI_MODULE_CONNECT_AP_ERR_AP_NOT_FOUND\r\n");
    printf("  '6' for WIFI_MODULE_CONNECT_AP_ERR_CONN_FAIL\r\n");
    printf("  '7' for WIFI_MODULE_NOT_DETECTED\r\n");
    printf("  Other for WIFI_MODULE_BUSY\r\n");
    while( receivedChar == '\0' ) receivedChar = pcSerialComCharRead();
    switch ( receivedChar ) {
        case '1': return WIFI_MODULE_IS_CONNECTED; break;
        case '2': return WIFI_MODULE_IS_NOT_CONNECTED; break;
        case '3': return WIFI_MODULE_CONNECT_AP_ERR_TIMEOUT; break;
        case '4': return WIFI_MODULE_CONNECT_AP_ERR_WRONG_PASS; break;
        case '5': return WIFI_MODULE_CONNECT_AP_ERR_AP_NOT_FOUND; break;
        case '6': return WIFI_MODULE_CONNECT_AP_ERR_CONN_FAIL; break;
        case '7': return WIFI_MODULE_NOT_DETECTED; break;
        default:  return WIFI_MODULE_BUSY; break;
    }
    //return WIFI_MODULE_IS_CONNECTED;
}


// Get IP address

// Responses:
// WIFI_MODULE_IP_GET_STARTED
// WIFI_MODULE_BUSY
wifiModuleRequestResult_t wifiModuleStartIpGet()
{/*
    char receivedChar = '\0';
    pcSerialComStringWrite( "wifiModuleStartIpGet()\r\n" );
    printf("Type:\r\n");
    printf("  '1' for WIFI_MODULE_IP_GET_STARTED\r\n");
    printf("  Other for WIFI_MODULE_BUSY\r\n");
    while( receivedChar == '\0' ) receivedChar = pcSerialComCharRead();
    switch ( receivedChar ) {
        case '1': return WIFI_MODULE_IP_GET_STARTED; break;
        default:  return WIFI_MODULE_BUSY; break;
    }*/
    return WIFI_MODULE_IP_GET_STARTED;
}

// Responses:
// WIFI_MODULE_IP_GET_COMPLETE
// WIFI_MODULE_NOT_DETECTED
// WIFI_MODULE_BUSY
wifiModuleRequestResult_t wifiModuleIpGetResponse( char* ip )
{
    char receivedChar = '\0';
    pcSerialComStringWrite( "wifiModuleIpGetResponse()\r\n" );
    printf("Type:\r\n");
    printf("  '1' for WIFI_MODULE_IP_GET_COMPLETE\r\n");
    printf("  '2' for WIFI_MODULE_NOT_DETECTED\r\n");
    printf("  Other for WIFI_MODULE_BUSY\r\n");
    while( receivedChar == '\0' ) receivedChar = pcSerialComCharRead();
    switch ( receivedChar ) {
        case '1': return WIFI_MODULE_IS_CONNECTED; break;
        case '2': return WIFI_MODULE_NOT_DETECTED; break;
        default:  return WIFI_MODULE_BUSY; break;
    }
    //return WIFI_MODULE_IP_GET_COMPLETE;
}

//=====[Implementations of private functions]==================================

static bool esp8266UartByteRead( char* receivedByte )
{
   bool areNewData = uartReadByte( UART_232, receivedByte );
   if ( areNewData ) {
      uartWriteByte( UART_USB, *receivedByte );
   }
   return areNewData;
}

static void esp8266UartByteWrite( char byteToSend )
{
    uartWriteByte( UART_232, byteToSend );
}

static void esp8266UartStringWrite( char const* str )
{
    while ( *str != NULL ) {
        esp8266UartByteWrite( (uint8_t)*str );
        str++;
    }
}

// Send commands that have only one valid response
static wifiModuleRequestResult_t esp8266SendCommandWithResponse( 
    char const* cmd, char const* cmdResponse,
    wifiModuleRequestResult_t informResult,
    tick_t timeout )
{
    if( esp8266State == ESP8266_IDLE ){
        parserInit( &parser, cmdResponse, strlen(cmdResponse), timeout );
        esp8266State = ESP8266_PROCESSING_AT_COMMAND;
        esp8266UartStringWrite( cmd );
        return informResult;
    } else {        
        return WIFI_MODULE_BUSY;   
    }    
}

// Check response for previously sended commands that only have one response
static wifiModuleRequestResult_t
esp8266CheckCommandResponse( wifiModuleRequestResult_t resultMatch )
{
    char receivedChar = '\0';
    esp8266UartByteRead( &receivedChar );    
    parserStatus = parserUpdate( &parser, receivedChar );
    switch( parserStatus ) {
        case PARSER_PATTERN_MATCH:
            esp8266State = ESP8266_IDLE;
            return resultMatch;
        break;
        case PARSER_TIMEOUT:
            esp8266State = ESP8266_IDLE;
            return WIFI_MODULE_NOT_DETECTED;
        break;
        default:
            return WIFI_MODULE_BUSY;
        break;
    }
}

// Send commands that have two valid responses
static wifiModuleRequestResult_t esp8266SendCommandWithTwoResponses( 
    char const* cmd, char const* cmdResponse1, char const* cmdResponse2,
    wifiModuleRequestResult_t informResult,
    tick_t timeout )
{
    if( esp8266State == ESP8266_IDLE ){
        // Lanzo 2 parsers en paralelo que buscan las 2 posibles respuestas,
        // ambos con el mismo timeout (el peor de los 2 casos)
        parserInit( &parser, cmdResponse1, strlen(cmdResponse1), timeout );
        parserInit( &parser2, cmdResponse2, strlen(cmdResponse2), timeout );
        esp8266State = ESP8266_PROCESSING_AT_COMMAND;
        esp8266UartStringWrite( cmd );
        return informResult;
    } else {        
        return WIFI_MODULE_BUSY;   
    }    
}
