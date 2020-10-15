//=====[#include guards - begin]===============================================

#ifndef _WIFI_MODULE_H_
#define _WIFI_MODULE_H_

//=====[Libraries]=============================================================

//=====[Declaration of public defines]=========================================

#define WIFI_MODULE_CREDENTIAL_MAX_LEN   101

//=====[Declaration of public data types]======================================

typedef enum{    
    WIFI_MODULE_CONNECT_AP_STARTED          = 0,
    WIFI_MODULE_CONNECT_AP_ERR_TIMEOUT      = 1,
    WIFI_MODULE_CONNECT_AP_ERR_WRONG_PASS   = 2,
    WIFI_MODULE_CONNECT_AP_ERR_AP_NOT_FOUND = 3,
    WIFI_MODULE_CONNECT_AP_ERR_CONN_FAIL    = 4,
    
    WIFI_MODULE_AP_SSID_SAVED,
    WIFI_MODULE_AP_SSID_NOT_SAVED,
    WIFI_MODULE_AP_PASSWORD_SAVED,
    WIFI_MODULE_AP_PASSWORD_NOT_SAVED,
    
    WIFI_MODULE_BUSY,
    
    WIFI_MODULE_DETECTION_STARTED,
    WIFI_MODULE_DETECTED,
    WIFI_MODULE_NOT_DETECTED,
    
    WIFI_MODULE_RESET_STARTED,
    WIFI_MODULE_RESET_COMPLETE,
    
    WIFI_MODULE_INIT_STARTED,
    WIFI_MODULE_INIT_COMPLETE,
    
    WIFI_MODULE_IS_CONNECTED_AP_STARTED,
    WIFI_MODULE_IS_CONNECTED,
    WIFI_MODULE_IS_NOT_CONNECTED,
    
    WIFI_MODULE_IP_GET_STARTED,
    WIFI_MODULE_IP_GET_COMPLETE,
    
} wifiModuleRequestResult_t;

//=====[Declarations (prototypes) of public functions]=========================

// Init module and status
void wifiModuleInit();

// Update module status
void wifiModuleUpdate();

// Set/Get AP credentials
wifiModuleRequestResult_t wifiModuleSetAP_SSID( char const* ssid );
wifiModuleRequestResult_t wifiModuleSetAP_Password( char const* password );
char const* wifiModuleGetAP_SSID();
char const* wifiModuleGetAP_Password();

// Detect module
wifiModuleRequestResult_t wifiModuleStartDetection();
wifiModuleRequestResult_t wifiModuleDetectionResponse();

// Reset module
wifiModuleRequestResult_t wifiModuleStartReset();
wifiModuleRequestResult_t wifiModuleResetResponse();

// Initialize module
wifiModuleRequestResult_t wifiModuleStartInit();
wifiModuleRequestResult_t wifiModuleInitResponse();

// Check if connected with AP
wifiModuleRequestResult_t wifiModuleStartIsConnectedWithAP();
wifiModuleRequestResult_t wifiModuleIsConnectedWithAPResponse();

// Connect with AP
wifiModuleRequestResult_t wifiModuleStartConnectWithAP();
wifiModuleRequestResult_t wifiModuleConnectWithAPResponse();

// Get IP address
wifiModuleRequestResult_t wifiModuleStartIpGet();
wifiModuleRequestResult_t wifiModuleIpGetResponse( char* ip );

//=====[#include guards - end]=================================================

#endif // _WIFI_MODULE_H_
