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
{
    return esp8266SendCommandWithResponse(
              "AT\r\n", "OK\r\n", 
              WIFI_MODULE_DETECTION_STARTED,
              ESP8266_MOST_COMMON_AT_CMD_TIMEOUT );
}

// Responses:
// WIFI_MODULE_DETECTED
// WIFI_MODULE_NOT_DETECTED
// WIFI_MODULE_BUSY
wifiModuleRequestResult_t wifiModuleDetectionResponse()
{
    return esp8266CheckCommandResponse( WIFI_MODULE_DETECTED );
}

// Reset module ---------------------------------------------------------------

// Responses:
// WIFI_MODULE_RESET_STARTED
// WIFI_MODULE_BUSY
wifiModuleRequestResult_t wifiModuleStartReset()
{
    return esp8266SendCommandWithResponse( "AT+RST\r\n", "OK\r\n\r\nready\r\n", 
                                           WIFI_MODULE_RESET_STARTED,
                                           ESP8266_AT_RST_CMD_TIMEOUT );
}

// Responses:
// WIFI_MODULE_RESET_COMPLETE
// WIFI_MODULE_NOT_DETECTED
// WIFI_MODULE_BUSY
wifiModuleRequestResult_t wifiModuleResetResponse()
{
    return esp8266CheckCommandResponse( WIFI_MODULE_RESET_COMPLETE );
}

// Initialize module ----------------------------------------------------------

// Responses:
// WIFI_MODULE_INIT_STARTED
// WIFI_MODULE_BUSY
wifiModuleRequestResult_t wifiModuleStartInit()
{
    return esp8266SendCommandWithResponse(
              "AT+CWMODE=1\r\n", "OK\r\n", 
              WIFI_MODULE_INIT_STARTED,
              ESP8266_MOST_COMMON_AT_CMD_TIMEOUT );
}

// Responses:
// WIFI_MODULE_INIT_COMPLETE
// WIFI_MODULE_NOT_DETECTED
// WIFI_MODULE_BUSY
wifiModuleRequestResult_t wifiModuleInitResponse()
{
    return esp8266CheckCommandResponse( WIFI_MODULE_INIT_COMPLETE );
}

// AP connection --------------------------------------------------------------

// Check if connected with AP

// Responses:
// WIFI_MODULE_IS_CONNECTED_AP_STARTED
// WIFI_MODULE_BUSY
wifiModuleRequestResult_t wifiModuleStartIsConnectedWithAP()
{
    return esp8266SendCommandWithResponse(
              "AT+CIPSTATUS\r\n", "STATUS:\r\n\r\nOK\r\n", 
              WIFI_MODULE_IS_CONNECTED_AP_STARTED,
              ESP8266_MOST_COMMON_AT_CMD_TIMEOUT );
}

// Responses:
// WIFI_MODULE_IS_CONNECTED
// WIFI_MODULE_IS_NOT_CONNECTED
// WIFI_MODULE_NOT_DETECTED
// WIFI_MODULE_BUSY
wifiModuleRequestResult_t wifiModuleIsConnectedWithAPResponse()
{
    static esp8266StationStatus_t status = ESP8266_STATUS_AP_NOT_CONNECTED;
    char receivedChar = '\0';
    // Leo un caracter desde la UART, si no hay nada para leer receivedChar queda en NULL como estaba inicializada
    esp8266UartByteRead( &receivedChar );
    // Si el caracter recibido es digito me lo guardo 
    if( charIsDigit(receivedChar) ) {
        status = (esp8266StationStatus_t) charDigitToIntDigit(receivedChar);
    }
    // Actualizao el parser pasandole el caracter recibido
    parserStatus = parserUpdate( &parser, receivedChar );
    // Actuo segun el estado del parser
    switch( parserStatus ) {
        case PARSER_PATTERN_MATCH:
            esp8266State = ESP8266_IDLE;
            if( status == ESP8266_STATUS_AP_NOT_CONNECTED ) {
                status = ESP8266_STATUS_AP_NOT_CONNECTED;
                return WIFI_MODULE_IS_NOT_CONNECTED;
            } else {
                status = ESP8266_STATUS_AP_NOT_CONNECTED;
                return WIFI_MODULE_IS_CONNECTED;
            }
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

// Connect with AP

// Responses:
// WIFI_MODULE_CONNECT_AP_STARTED
// WIFI_MODULE_BUSY
wifiModuleRequestResult_t wifiModuleStartConnectWithAP()
{
    char cmd[100] = "AT+CWJAP=\"";
    strcat( cmd, credential_ssid );
    strcat( cmd, "\",\"" );
    strcat( cmd, credential_password );
    strcat( cmd, "\"\r\n" );    
    // Estas lineas forman cmd = AT+CWJAP="userSSID","userPassword"\r\n
    
    status = WIFI_MODULE_IS_NOT_CONNECTED; // Status default initial value
 
    return esp8266SendCommandWithTwoResponses(
              cmd, "WIFI CONNECTED\r\nWIFI GOT IP\r\n",
              "+CWJAP:\r\n\r\nFAIL\r\n",
              WIFI_MODULE_CONNECT_AP_STARTED,
              ESP8266_AT_CWJAP_CMD_TIMEOUT );
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
    // Leo un caracter desde la UART, si no hay nada para leer receivedChar queda en NULL como estaba inicializada
    esp8266UartByteRead( &receivedChar );
    // Si el caracter recibido es digito me lo guardo 
    if( charIsDigit(receivedChar) ) {
        status = (wifiModuleRequestResult_t) charDigitToIntDigit(receivedChar);
    }
    // Actualizo los 2 parsers pasándole a cada uno el mismo caracter que llego, por esto decimos que actua en paralelo
    parserStatus = parserUpdate( &parser, receivedChar );
    parser2Status = parserUpdate( &parser2, receivedChar );
    // Actuo segun los resultados de ambos parsers
    // Matcheo parser 1, entonces se conecto bien
    if( parserStatus == PARSER_PATTERN_MATCH ) {
        esp8266State = ESP8266_IDLE;
        return WIFI_MODULE_IS_CONNECTED;
    } else 
    // Matcheo parser 2, entonces fallo al intentar conectar al AP, retorno la causa de falla
    if( parser2Status == PARSER_PATTERN_MATCH ) {
        esp8266State = ESP8266_IDLE;
        //printf("Satatus: %d\r\n", status); // Debug
        if( status >= WIFI_MODULE_CONNECT_AP_ERR_TIMEOUT &&
                status <= WIFI_MODULE_CONNECT_AP_ERR_CONN_FAIL ) {
            return status;
        } else {
            return WIFI_MODULE_IS_NOT_CONNECTED;
        }
     } else 
     // Alguno de los 2 parser salio por timeout
     if ( parserStatus == PARSER_TIMEOUT || parser2Status == PARSER_TIMEOUT ) {
         esp8266State = ESP8266_IDLE;
         return WIFI_MODULE_NOT_DETECTED;
     } 
     // Por defecto si ninguno de los parsers termino retorno que el modulo esta ocupado
     else {
         return WIFI_MODULE_BUSY; 
     }
}


// Get IP address

// Responses:
// WIFI_MODULE_IP_GET_STARTED
// WIFI_MODULE_BUSY
wifiModuleRequestResult_t wifiModuleStartIpGet()
{
    return esp8266SendCommandWithResponse(
              "AT+CIFSR\r\n", "+CIFSR:STAIP,\"\"\r\n\r\nOK\r\n", 
              WIFI_MODULE_IP_GET_STARTED,
              ESP8266_MOST_COMMON_AT_CMD_TIMEOUT );
}

// Responses:
// WIFI_MODULE_IP_GET_COMPLETE
// WIFI_MODULE_NOT_DETECTED
// WIFI_MODULE_BUSY
wifiModuleRequestResult_t wifiModuleIpGetResponse( char* ip )
{
    static int i = 0;
    static int quotationMarksCounter = 0;
    char receivedChar = '\0';
    // Leo un caracter desde la UART, si no hay nada para leer receivedChar queda en NULL como estaba inicializada
    esp8266UartByteRead( &receivedChar );
    // Si el caracter recibido es " aumento el contrador de comillas
    if( receivedChar == '"' ) {
        quotationMarksCounter++;
    }
    // Si el caracter recibido es digito o un punto ('.') me lo guardo 
    if( (charIsDigit(receivedChar) || receivedChar == '.') && 
            quotationMarksCounter <= 2 ) {
        ip[i] = receivedChar;
        i++;
    }
    // Actualizao el parser pasandole el caracter recibido
    parserStatus = parserUpdate( &parser, receivedChar );
    // Actuo segun el estado del parser
    switch( parserStatus ) {
        case PARSER_PATTERN_MATCH:
            esp8266State = ESP8266_IDLE;
            ip[i] = '\0'; // Agrego el null para que la IP sea un string valido
            i = 0;
            quotationMarksCounter = 0;
            return WIFI_MODULE_IP_GET_COMPLETE;
        break;
        case PARSER_TIMEOUT:
            esp8266State = ESP8266_IDLE;
            i = 0;
            quotationMarksCounter = 0;
            return WIFI_MODULE_NOT_DETECTED;
        break;
        default:
            return WIFI_MODULE_BUSY;
        break;
    }
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
