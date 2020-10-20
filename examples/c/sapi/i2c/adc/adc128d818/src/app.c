/*============================================================================
 * Autor:
 * Licencia:
 * Fecha:
 *===========================================================================*/

// Inlcusiones

#include "sapi.h"               // <= Biblioteca sAPI
#include <string.h>

#define BAUD_RATE 115200
#define BF_L      1024          // Uart's string buffer length

/*==================[internal data declaration]==============================*/


/*==================[internal functions declaration]=========================*/

/** @brief hardware initialization function
*	@return none
*/
//static void initHardware(void);

/*==================[internal data definition]===============================*/

char intbuff[1024];

/*==================[external data definition]===============================*/

/*==================[internal functions definition]==========================*/

char *intToHex(uint64_t value, uint8_t digits)
{
    char *ptr1;
    char *ptr2;
    char c;
    int ind = 0;
        
    do {
        if ((value & 0xf) < 10)
            intbuff[ind++] = '0'+(value & 0xf);
        else
            intbuff[ind++] = 'a'+((value & 0xf)-10);
        value = (value - (value & 0xf)) >> 4;
        digits--;
    } while (digits != 0);
    
    intbuff[ind] = '\0';
    
    ptr1 = intbuff;
    ptr2 = intbuff+(ind-1);
    while (ptr1 < ptr2) {
        c = *ptr1;
        *ptr1++ = *ptr2;
        *ptr2-- = c;
    }
    
    return intbuff;
}


/*==================[external functions definition]==========================*/

void printTickCount(int length) {
    char buff[100]; 
    char buff2[100];
    
    sprintf(buff, "%u", tickRead());
    while (strlen(buff) < length) {
        strcpy(buff2, buff);
        strcpy(buff, "0");
        strcat(buff, buff2);
    }
    strcat(buff, "ms ");
    uartWriteString( UART_USB,  buff);
}

void logError(char *str) {
    printTickCount(8);
    uartWriteString( UART_USB, "\033[31mError: " );
    uartWriteString( UART_USB, str );
    uartWriteString( UART_USB, "\033[0m" );
    uartWriteString( UART_USB, "\r\n" );
}

void goodbye() {
    printf("System Halt\r\n");
}


// FUNCION PRINCIPAL, PUNTO DE ENTRADA AL PROGRAMA LUEGO DE ENCENDIDO O RESET.
int main( void )
{
    int result;
    int i;
    char k;
    uint32_t p;
    bool_t wait_key;
    uint8_t count;
    uint8_t v;
    // ---------- CONFIGURACIONES ------------------------------

    // Inicializar y configurar la plataforma
    boardConfig();
    
    /* Inicializar Uart */
    uartConfig( UART_USB, BAUD_RATE );
    printf("\e[2J");  // Escape command: Clear screen
    printf("\r\n");
    printf("\r\n");
    printf("\r\n");
    printf("New boot...\r\n");
    printf("\r\n");
    
    printf("I2C bus start\r\n");
    i2cInit(I2C0, 100000);

    // Set the LED to the state of "Off"
    // Board_Dout_Set(0, false);

    /* Inicializar el conteo de Ticks con resoluci\F3n de 1ms, sin tickHook */
    //tickConfig( 1, 0 );

   /* Inicializar ADC128D818 */
    adc128d818_init_t init_U12;

    init_U12.address = ADC128D818_ADDRESS_LOW_LOW;
    init_U12.op_mode = ADC128D818_OPERATION_MODE_1;
    init_U12.rate = ADC128D818_RATE_ONE_SHOT;
    init_U12.ref_mode = ADC128D818_VREF_INT;
    init_U12.ref_voltage = 0;
    init_U12.enabled_mask = 0;

    result = adc128d818_init(&init_U12);

    if (!result) {
        logError("No device ADC128D818 or device busy");
        return 0;
    }

    adc128d818_init_t init_U14;
    init_U14.address = ADC128D818_ADDRESS_HIGH_HIGH;
    init_U14.op_mode = ADC128D818_OPERATION_MODE_1;
    init_U14.rate = ADC128D818_RATE_ONE_SHOT;
    init_U14.ref_mode = ADC128D818_VREF_INT;
    init_U14.ref_voltage = 0;
    init_U14.enabled_mask = 0;

    result = adc128d818_init(&init_U14);

    if (!result) {
        logError("No device ADC128D818 or device busy");
        return 0;
    }
    printf("\r\n");
    printf("\r\n");
    printf("=============================================\r\n");
    printf(" Start: press space or wait 5 seconds...\r\n");
    printf("\e[2J");

    uint16_t time_count = 0;
    while (TRUE) {
        if (uartRxReady( UART_USB )) {
            k = uartRxRead( UART_USB );
            if (k == 32 ) {
                break;
            }
        }
        delay(1);
        if (++time_count >= 5000) {
            break;
        }

    }
    count = 0;
    
    // ---------- REPETIR POR SIEMPRE --------------------------
    while( TRUE ) {
      
        for (i=0; i<8; i++) {
            int16_t v = adc128d818_readChannel(init_U14.address, i);
            printf("Chip U15. AN#0%u: %u     \r\n", i, v);
        }

        for (i=0; i<8; i++) {
            p=adc128d818_readChannel(init_U12.address, i);
            if (i+8<10) {
                printf("Chip U13. AN#0%u: %u     \r\n", i+8, p);
            } else {
                printf("Chip U13. AN#%u: %u     \r\n", i+8, p);
            }
        }
        printf("=============================================\r\n");
        printf(" Start/Stop: press space\r\n");

        while (uartRxReady( UART_USB )) {
            k = uartRxRead( UART_USB );
            wait_key = TRUE;
            while (wait_key) {
                if (uartRxReady( UART_USB )) {
                    char k = uartRxRead( UART_USB );
                    if (k == 32 ) wait_key = FALSE;
                }
            }
        }
        delay(150);
        printf("\e[100D");   // Escape command: go left 100 (line start)
        printf("\e[18A");    // Escape command: go up 18
        if (count>=4) count = 0;
    }

    // NO DEBE LLEGAR NUNCA AQUI, debido a que a este programa se ejecuta
    // directamenteno sobre un microcontroladore y no es llamado por ningun
    // Sistema Operativo, como en el caso de un programa para PC.
    return 0;
}
