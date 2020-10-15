//=====[Libraries]=============================================================

#include "wifi_com.h"

#include "sapi.h"
#include "wifi_module.h"

//=====[Declaration of private defines]========================================

#define pcSerialComStringWrite   printf

//=====[Declaration of private data types]=====================================

typedef enum{
    WIFI_STATE_MODULE_DETECT,
    WIFI_STATE_MODULE_NOT_DETECTED,
    WIFI_STATE_MODULE_INIT,
    WIFI_STATE_MODULE_CHECK_AP_CONNECTION,
    WIFI_STATE_MODULE_NOT_CONNECTED,
    WIFI_STATE_COMMUNICATION,
} wifiFsmComState_t;

//=====[Declaration and initialization of public global objects]===============

//=====[Declaration of external public global variables]=======================

//=====[Declaration and initialization of public global variables]=============

//=====[Declaration and initialization of private global variables]============

static wifiFsmComState_t wifiComFsmState;

//=====[Declarations (prototypes) of private functions]========================

static void runStateWifiModuleDetect();
static void runStateWifiModuleNotDetected();

static void runStateWifiModuleInit();

static void runStateWifiModuleCheckAPConnection();
static void runStateWifiModuleNotConnected();

//=====[Implementations of public functions]===================================

// Wi-Fi FSM ------------------------------------------------------------------

void wifiComInit()
{
    wifiComFsmState = WIFI_STATE_MODULE_DETECT;
    wifiModuleInit();
}

void wifiComUpdate()
{
    switch ( wifiComFsmState ) {
        case WIFI_STATE_MODULE_DETECT: 
            runStateWifiModuleDetect();
        break;
        case WIFI_STATE_MODULE_NOT_DETECTED:
            runStateWifiModuleNotDetected();
        break;
        case WIFI_STATE_MODULE_INIT:
            runStateWifiModuleInit();
        break;
        case WIFI_STATE_MODULE_CHECK_AP_CONNECTION:
            runStateWifiModuleCheckAPConnection();
        break;
        case WIFI_STATE_MODULE_NOT_CONNECTED:
            runStateWifiModuleNotConnected();
        break;
        case WIFI_STATE_COMMUNICATION:
            runStateWifiCommunication();
        break;
        default:
            wifiComFsmState = WIFI_STATE_MODULE_DETECT;
        break;
    }
}

//=====[Implementations of private functions]==================================

// En este estado si detecta el modulo pasa al estado 
// WIFI_STATE_MODULE_INIT.
// Si no pasa al estado: WIFI_STATE_MODULE_NOT_DETECTED
static void runStateWifiModuleDetect()
{
    static bool stateEntryFlag = false;
    // ENTRY ----------------------------------------
    if( stateEntryFlag == false ){
        if( wifiModuleStartDetection() != WIFI_MODULE_DETECTION_STARTED ){
            return;
        }
        stateEntryFlag = true;
    }

    // CHECK TRANSITION CONDITIONS ------------------
    switch( wifiModuleDetectionResponse() ) {
        case WIFI_MODULE_DETECTED:
            wifiComFsmState = WIFI_STATE_MODULE_INIT;
            pcSerialComStringWrite( "Wi-Fi module detected.\r\n" );
        break;
        case WIFI_MODULE_NOT_DETECTED:
            wifiComFsmState = WIFI_STATE_MODULE_NOT_DETECTED;
        break;
        case WIFI_MODULE_BUSY: // Module busy, not do anything
        default:
        break;
    }

    // EXIT ------------------------------------------
    if( wifiComFsmState != WIFI_STATE_MODULE_DETECT ){
        stateEntryFlag = false;
    }
}

// En este estado reintenta detectar el módulo Wi-Fi cada 10 segundos (lo resetea).
// Cuando lo detecta pasa al estado WIFI_STATE_MODULE_INIT
static void runStateWifiModuleNotDetected()
{
    static bool stateEntryFlag = false;
    static bool resetSended = false;
    static delay_t reintentsDelay;
    // ENTRY ----------------------------------------
    if( stateEntryFlag == false ) {
        pcSerialComStringWrite( "\r\nERROR: Wi-Fi module not detected!\r\n" );
        pcSerialComStringWrite( "It will re-intent automaticaly in 10 " );
        pcSerialComStringWrite( "seconds...\r\n" );
        delayInit( &reintentsDelay, 10000 );
        stateEntryFlag = true;
    }

    // CHECK TRANSITION CONDITIONS ------------------
    if( delayRead(&reintentsDelay) && !resetSended ) {
        pcSerialComStringWrite( "Reseting Wi-Fi module..\r\n" );
        if( wifiModuleStartReset() == WIFI_MODULE_RESET_STARTED ) {
            resetSended = true;
        }
    }
    if ( resetSended ) {            
        switch( wifiModuleResetResponse() ) {
            case WIFI_MODULE_RESET_COMPLETE:
                wifiComFsmState = WIFI_STATE_MODULE_INIT;
                pcSerialComStringWrite( "Wi-Fi module detected.\r\n" );
            break;
            case WIFI_MODULE_NOT_DETECTED:
                wifiComFsmState = WIFI_STATE_MODULE_NOT_DETECTED;
                stateEntryFlag = false; // Asi relanzo el entry de este estado   
            break;
            case WIFI_MODULE_BUSY: // Module busy, not do anything
            default:
            break;
        }
        resetSended = false;   
    }

    // EXIT ------------------------------------------
    if( wifiComFsmState != WIFI_STATE_MODULE_NOT_DETECTED ){
        stateEntryFlag = false;
    }
}

// En este estado inicializa el modulo Wi-Fi para poder conectarse a un AP
// Cuando termina la inicializacion pasa al estado
// WIFI_STATE_MODULE_CHECK_AP_CONNECTION
static void runStateWifiModuleInit()
{
    static bool stateEntryFlag = false;
    // ENTRY ----------------------------------------
    if( stateEntryFlag == false ){
        if( wifiModuleStartInit() != WIFI_MODULE_INIT_STARTED ){
            return;
        }
        stateEntryFlag = true;
    }

    // CHECK TRANSITION CONDITIONS ------------------
    switch( wifiModuleInitResponse() ) {
        case WIFI_MODULE_INIT_COMPLETE:
            pcSerialComStringWrite( "Wi-Fi module initialized.\r\n" );
            wifiComFsmState = WIFI_STATE_MODULE_CHECK_AP_CONNECTION;
        break;
        case WIFI_MODULE_NOT_DETECTED:
            wifiComFsmState = WIFI_STATE_MODULE_NOT_DETECTED;
        break;
        case WIFI_MODULE_BUSY: // Module busy, not do anything
        default:
        break;
    }

    // EXIT ------------------------------------------
    if( wifiComFsmState != WIFI_STATE_MODULE_INIT ){
        stateEntryFlag = false;
    }
}

// Chequea si esta conectado y tiene IP.
// Si esta conectado y tiene IP pasa al estado WIFI_STATE_COMMUNICATIONS_INIT
// Si no esta conectado pasa al estado: WIFI_STATE_MODULE_NOT_CONNECTED
static void runStateWifiModuleCheckAPConnection()
{
    static bool stateEntryFlag = false;
    char ip[20] = ""; 
    // ENTRY ----------------------------------------
    if( stateEntryFlag == false ){
        if( wifiModuleStartIsConnectedWithAP() != WIFI_MODULE_INIT_STARTED ){
            return;
        }
        stateEntryFlag = true;
    }

    // CHECK TRANSITION CONDITIONS ------------------
    switch( wifiModuleIsConnectedWithAPResponse() ) {  // ??????????????????????????????????
        case WIFI_MODULE_IS_CONNECTED:
            pcSerialComStringWrite( "Wi-Fi module is connected. IP = " );
            pcSerialComStringWrite( ip );
            pcSerialComStringWrite( "\r\n" );
            wifiComFsmState = WIFI_STATE_COMMUNICATION;
        break;
        case WIFI_MODULE_IS_NOT_CONNECTED:
            pcSerialComStringWrite( "Wi-Fi module is not connected.\r\n" );
            wifiComFsmState = WIFI_STATE_MODULE_NOT_CONNECTED;
        break;
        case WIFI_MODULE_NOT_DETECTED:
            wifiComFsmState = WIFI_STATE_MODULE_NOT_DETECTED;
        break;
        case WIFI_MODULE_BUSY: // Module busy, not do anything
        default:
        break;
    }

    // EXIT ------------------------------------------
    if( wifiComFsmState != WIFI_STATE_MODULE_CHECK_AP_CONNECTION ){
        stateEntryFlag = false;
    }
}

// Si no esta conectado reintenta conectarse cada 10 segundos.
// Si no detecta el modulo vuelve a: 
//     WIFI_STATE_MODULE_NOT_DETECTED.
static void runStateWifiModuleNotConnected()
{
    static bool stateEntryFlag = false;
    char ip[20] = ""; 
    static bool isWaitingForNextTry = false;
    static bool isWaitingForIp = false;
    static delay_t reintentsDelay;
    // ENTRY ----------------------------------------
    if( stateEntryFlag == false ){
        if( wifiModuleStartConnectWithAP() != WIFI_MODULE_CONNECT_AP_STARTED ){
            return;
        }        
        pcSerialComStringWrite( "Wi-Fi try to connect with AP SSID: " );
        pcSerialComStringWrite( wifiModuleGetAP_SSID() );
        pcSerialComStringWrite( "\r\n" );
        stateEntryFlag = true;
    }

    // CHECK TRANSITION CONDITIONS ------------------
    if( isWaitingForNextTry ) {       
        if( delayRead(&reintentsDelay) ) {
            wifiComFsmState = WIFI_STATE_MODULE_NOT_DETECTED;
            isWaitingForNextTry = false;            
        }
        return;
    } else{
        switch( wifiModuleConnectWithAPResponse() ) {
            case WIFI_MODULE_IS_CONNECTED:
                isWaitingForIp = true;
            break;
            case WIFI_MODULE_NOT_DETECTED:
                wifiComFsmState = WIFI_STATE_MODULE_NOT_DETECTED;          
            break;                
            // Errors trying to connect
            case WIFI_MODULE_CONNECT_AP_ERR_TIMEOUT:
                pcSerialComStringWrite( "\r\nERROR: Connection timeout. " );
                isWaitingForNextTry = true;
            break;
            case WIFI_MODULE_CONNECT_AP_ERR_WRONG_PASS:
                pcSerialComStringWrite( "\r\nERROR: Wrong password. " );
                isWaitingForNextTry = true;
            break;
            case WIFI_MODULE_CONNECT_AP_ERR_AP_NOT_FOUND:
                pcSerialComStringWrite( "\r\nERROR: Cannot find the target " );
                pcSerialComStringWrite( "AP. " );
                isWaitingForNextTry = true;
            break;
            case WIFI_MODULE_CONNECT_AP_ERR_CONN_FAIL:
                pcSerialComStringWrite( "\r\nERROR: Connection failed. " );
                isWaitingForNextTry = true;
            break;            
            // Module busy, not do anything
            case WIFI_MODULE_BUSY:
            default:
            break;
        }
        if( isWaitingForNextTry ) {
            pcSerialComStringWrite( "Wi-Fi not connected!\r\n" );
            pcSerialComStringWrite( "It will re-intent automaticaly in 10 " );
            pcSerialComStringWrite( "secondsr\nafter restet the module..." );
            pcSerialComStringWrite( "\r\n" );
            delayInit( &reintentsDelay, 10000 );
        }
    }
    if( isWaitingForIp ){
        
        // ... TODO: FALTA ------------------------------------------------ ????????????????????
        
        pcSerialComStringWrite( "Wi-Fi module is connected. IP = " );
        pcSerialComStringWrite( ip );
        pcSerialComStringWrite( "\r\n" );
        wifiComFsmState = WIFI_STATE_COMMUNICATION;
    }

    // EXIT ------------------------------------------
    if( wifiComFsmState != WIFI_STATE_MODULE_NOT_CONNECTED ){
        stateEntryFlag = false;
    }
}

// En este estado:
//     embeddedServerInit(); // Chapter 9 y 10
//     clientInit();         // Chapter 10
//     Y  cuando termina pasa al estado:
//        WIFI_STATE_COMMUNICATION_UPDATE
// En este estado levanta el Server y se pasa al siguiente estado.
// Si no detecta el módulo vuelve a: 
//     WIFI_STATE_MODULE_NOT_DETECTED.
// Si se pierde la conexión entre el módulo y el AP vuelve a: 
//     WIFI_STATE_MODULE_WAIT_FOR_AP_CONNECTION
static void runStateWifiCommunication()
{
    static bool stateEntryFlag = false;
    // ENTRY ----------------------------------------
    if( stateEntryFlag == false ){
        stateEntryFlag = true;
        pcSerialComStringWrite( "Check connection status\r\n" );
    }

    // UPDATE OUTPUTS -------------------------------


    // CHECK TRANSITION CONDITIONS ------------------

    // EXIT ------------------------------------------
    if( wifiComFsmState != WIFI_STATE_COMMUNICATION ){
        stateEntryFlag = false;
    }
}
