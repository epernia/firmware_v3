/* Copyright 2017, Eric Pernia.
 */

/* Date: 2017-07-27 */

/*==================[inclusions]=============================================*/

#include "sapi_eeprom24xx1025.h"   // <= own header

#define eeprom24xx1025DelayMs   delayInaccurateMs

/*==================[internal data definition]===============================*/

DEBUG_PRINT_ENABLE

/*==================[internal functions definition]==========================*/

//static void eeprom24xx1025DelayUS(uint32_t us){
//   volatile uint32_t i=0;
//   for( i=0; i<(us*24); i++ );
//}

/*==================[external functions definition]==========================*/

//-----------------------------------------------------------------------------
// MANAGEMENT
//-----------------------------------------------------------------------------

uint8_t eeprom24xx1025I2cAddress( Eeprom24xx1025_t* eeprom, uint32_t address )
{
   bool_t a0 = 0;
   bool_t a1 = 0;

   // TODO: Mejorar este chequeo
   if( eeprom->gpioA0 >= 0 ) {
      a0 = gpioRead( eeprom->gpioA0 ) & 0x01;
   } else {
      a0 = eeprom->A0;
   }

   // TODO: Mejorar este chequeo
   if( eeprom->gpioA1 >= 0 ) {
      a1 = gpioRead( eeprom->gpioA1 ) & 0x01;
   } else {
      a1 = eeprom->A1;
   }

   if( address >= eeprom->memorySize / 2 ) {
      //                0b1010                B0=1       A1       A0
      return (EEPROM24xx1025_CTRL_CODE<<3) | (1<<2) | (a1<<1) | (a0<<0);
   } else {
      //                0b1010                B0=0       A1       A0
      return (EEPROM24xx1025_CTRL_CODE<<3) | (0<<2) | (a1<<1) | (a0<<0);
   }
}


// Reads may be sequential within address boundaries 0000h to FFFFh and 10000h to 1FFFFh


// Enable or disable the peripheral energy and clock
bool_t eeprom24xx1025PowerSet( Eeprom24xx1025_t* eeprom, bool_t power )
{
   if( eeprom->gpioPower >= 0 ){
      if( power ) {
         gpioWrite( eeprom->gpioPower, ON );
      } else {
         gpioWrite( eeprom->gpioPower, OFF );
      }
   }
   return TRUE;
}

/*---------------------------------------------------------------------------*/

bool_t eeprom24xx1025DataReady( void )
{
   // TODO: implment
   return TRUE;
}

// Amount of pages
uint32_t eeprom24xx1025AmountOfPagesGet( Eeprom24xx1025_t* eeprom )
{
   return eeprom->memorySize / eeprom->pageSize;
}

//-----------------------------------------------------------------------------
// INITIALIZATION
//-----------------------------------------------------------------------------

bool_t eeprom24xx1025Init( Eeprom24xx1025_t* eeprom,
                           int32_t i2c, bool_t A0, bool_t A1,
                           int32_t gpioA0, int32_t gpioA1,
                           int32_t gpioPower, int32_t gpioWP,
                           int32_t pageSize, int32_t memorySize )
{

   bool_t retVal = FALSE;
   uint8_t testByte = 0;

   // I2C port connected to EEPROM, example I2C0
   eeprom->i2c = i2c;
   // Use this if fixed address
   eeprom->A0 = A0; // EEPROM I2C addres, depends of A0 and A1 pins status and B0 value
   eeprom->A1 = A1; // EEPROM I2C addres, depends of A0 and A1 pins status and B0 value
   // Use this if controlling address from MCU
   eeprom->gpioA0 = gpioA0; // GPIO conected to A0 of EEPROM
   eeprom->gpioA1 = gpioA1; // GPIO conected to A1 of EEPROM
   // Use this if controlling EEPROM power from MCU
   eeprom->gpioPower = gpioPower;  // GPIO to manage power of EEPROM
   // Use this if controlling WP pin from MCU if not must be WP=0 (unprotected)
   eeprom->gpioWP = gpioWP;     // GPIO conected to Write Proyection Pin of EEPROM
   // EEPROM capacity
   eeprom->pageSize = pageSize;   // EEPROM page size [bytes]
   eeprom->memorySize = memorySize; // EEPROM total memory size [bytes]

   // TODO: Mejorar este chequeo
   if( eeprom->gpioA0 >= 0 ) gpioInit( eeprom->gpioA0, GPIO_OUTPUT );
   if( eeprom->gpioA1 >= 0 ) gpioInit( eeprom->gpioA1, GPIO_OUTPUT );
   if( eeprom->gpioPower >= 0 ) gpioInit( eeprom->gpioPower, GPIO_OUTPUT );
   if( eeprom->gpioWP >= 0 ) gpioInit( eeprom->gpioWP, GPIO_OUTPUT );
   //GPIO_PinModeSet(EEPROM_POWER_PORT, EEPROM_POWER_PIN, gpioModePushPull, 1);

   // TEST EEPROM

   // Power on EEPROM
//   eeprom24xx1025PowerSet( eeprom, ON );
//   // If can read EEPROM then return true
//   retVal = eeprom24xx1025ReadRandom( eeprom, 0x0100, &testByte );
//   // Power off EEPROM
//   eeprom24xx1025PowerSet( eeprom, OFF );

   // Configure EEPROM Power pin
   return TRUE; //retVal;
}

//-----------------------------------------------------------------------------
// WRITE OPERATIONS
//-----------------------------------------------------------------------------

// Byte Write
bool_t eeprom24xx1025WriteByte( Eeprom24xx1025_t* eeprom,
                                uint32_t memoryAddress, uint8_t byteToWrite )
{

   bool_t retVal = TRUE; // True if OK

   // Check memory address
   if( memoryAddress > eeprom->memorySize ) {
      return FALSE;
   }

   uint8_t dataToWrite[3];

   // Memory address High
   dataToWrite[0] = EEPROM_ADDRESS_HIGH( memoryAddress );
   // Memory address Low
   dataToWrite[1] = EEPROM_ADDRESS_LOW( memoryAddress );

   // Byte to write
   dataToWrite[2] = (uint8_t)byteToWrite;

   /* uint8_t i2cNumber, uint8_t  i2cSlaveAddress,
      uint8_t* transmitDataBuffer, uint16_t transmitDataBufferSize,
      bool_t sendWriteStop */
   retVal = i2cWrite( eeprom->i2c,
                      eeprom24xx1025I2cAddress( eeprom, memoryAddress ),
                      dataToWrite, 3, TRUE );

   eeprom24xx1025DelayMs(5); // Twc - Write cycle time (byte or page)

   return retVal; // Byte writed
}


// Page Write
bool_t eeprom24xx1025WritePage( Eeprom24xx1025_t* eeprom, uint32_t page,
                                uint8_t* byteBuffer, uint32_t byteBufferSize )
{

   bool_t retVal = TRUE; // True if OK

   // Check valid buffer size
   if( byteBufferSize != eeprom->pageSize ) {
      return FALSE;
   }

   // Check valid page number
   if( page >= eeprom24xx1025AmountOfPagesGet(eeprom) ) {
      return FALSE;
   }

   uint16_t i=0;

   uint16_t memoryAddress = page * eeprom->pageSize;

   uint8_t dataToWrite[ byteBufferSize+2 ]; // 2 bytes more for memory address

   // Memory address High
   dataToWrite[0] = EEPROM_ADDRESS_HIGH( memoryAddress );
   // Memory address Low
   dataToWrite[1] = EEPROM_ADDRESS_LOW( memoryAddress );

   // Bytes to write
   for( i=0; i<byteBufferSize; i++ ) {
      dataToWrite[i+2] = byteBuffer[i];
   }

   // uint8_t i2cNumber, uint8_t  i2cSlaveAddress,
   // uint8_t* transmitDataBuffer, uint16_t transmitDataBufferSize,
   // bool_t sendWriteStop
   retVal = i2cWrite( eeprom->i2c,
                      eeprom24xx1025I2cAddress( eeprom, memoryAddress ),
                      dataToWrite, (byteBufferSize+2), TRUE );

   eeprom24xx1025DelayMs(5); // Twc - Write cycle time (byte or page)

   return retVal; // Byte writed
}


//-----------------------------------------------------------------------------
// READ OPERATIONS
//-----------------------------------------------------------------------------

// Current Address Read
bool_t eeprom24xx1025ReadCurrentAddress( Eeprom24xx1025_t* eeprom,
                                         uint32_t memoryAddress,
                                         uint8_t* readedByte )
{
   bool_t retVal = TRUE; // True if OK

   // uint8_t i2cNumber, uint8_t i2cSlaveAddress,
   // uint8_t* dataToReadBuffer, uint16_t dataToReadBufferSize,
   // bool_t sendWriteStop,
   // uint8_t* reciveDataBuffer, uint16_t reciveDataBufferSize,
   // bool_t sendReadStop
   retVal = i2cRead( eeprom->i2c,
                     eeprom24xx1025I2cAddress( eeprom, memoryAddress ),
                     (uint8_t*)0, 0, 
                     FALSE,
                     readedByte, 1, TRUE );

   return retVal; // read correct
}


// Random Read
bool_t eeprom24xx1025ReadRandom( Eeprom24xx1025_t* eeprom,
                                 uint32_t memoryAddress, uint8_t* readedByte )
{
   bool_t retVal = TRUE; // True if OK

   // Check memory address
   if( memoryAddress > eeprom->memorySize ) {
      return FALSE;
   }

   uint8_t addressToRead[ 2 ]; // 2 bytes for memory address

   // Memory address High
   addressToRead[0] = EEPROM_ADDRESS_HIGH( memoryAddress );
   // Memory address Low
   addressToRead[1] = EEPROM_ADDRESS_LOW( memoryAddress );

   // uint8_t i2cNumber, uint8_t i2cSlaveAddress,
   // uint8_t* dataToReadBuffer, uint16_t dataToReadBufferSize,
   // bool_t sendWriteStop,
   // uint8_t* reciveDataBuffer, uint16_t reciveDataBufferSize,
   // bool_t sendReadStop
   retVal = i2cRead( eeprom->i2c,
                     eeprom24xx1025I2cAddress( eeprom, memoryAddress ),
                     addressToRead, 2, FALSE,
                     readedByte, 1, TRUE );

   //eeprom24xx1025DelayMs(1); // ??? - Read cycle time (byte or page)

   return retVal; // read correct
}


// Sequential Read
bool_t eeprom24xx1025ReadSequential( Eeprom24xx1025_t* eeprom, uint32_t address,
                                     uint8_t* byteBuffer, uint32_t byteBufferSize )
{
   bool_t retVal = TRUE; // True if OK

   // Check valid buffer size
   if( byteBufferSize > eeprom->memorySize / 2 ) { 
      return FALSE;
   }

   // TODO: Check valid address
   //if( address >= eeprom24xx1025AmountOfPagesGet(eeprom) ) {
   //   return FALSE;
   //}

   uint8_t addressToRead[ 2 ]; // 2 bytes for memory address

   // Memory address High
   addressToRead[0] = EEPROM_ADDRESS_HIGH( address );
   // Memory address Low
   addressToRead[1] = EEPROM_ADDRESS_LOW( address );

   // uint8_t i2cNumber, uint8_t i2cSlaveAddress,
   // uint8_t* dataToReadBuffer, uint16_t dataToReadBufferSize,
   // bool_t sendWriteStop,
   // uint8_t* reciveDataBuffer, uint16_t reciveDataBufferSize,
   // bool_t sendReadStop
   retVal = i2cRead( eeprom->i2c,
                     eeprom24xx1025I2cAddress( eeprom, address ),
                     addressToRead, 2, FALSE,
                     byteBuffer, byteBufferSize, TRUE );

   return retVal; // Byte writed
}


//-----------------------------------------------------------------------------
// EEPROM WRITE PROTECTION
//-----------------------------------------------------------------------------

void eeprom24xx1025WriteProtectionSet( bool_t state )
{
   // TODO: WP PIN Set
}

bool_t eeprom24xx1025WriteProtectionGet( void )
{
   // TODO: WP PIN get
   return FALSE;
}


/**
 * EEPROM Test Write individual bytes and Read Random
 * @param uint8_t startingPage. The page where to start the test.
 * @param uint8_t* buff. The buffer to save in mamory.
 * @param uint32_t. The size of buffer (must be lower than page size).
 * @return bool_t. Whether the test was passed or not.
 */
bool_t eeprom24xx1025TestWriteBytes( uint32_t startingPage, uint8_t* buff, uint32_t buffSize )
{
   uint8_t readByte = 0;
   uint32_t i = 0;

   boardInit();

   debugPrintConfigUart( UART_USB, 115200 );
   debugPrintlnString( "\r\n--- EEPROM Test Write individual bytes and Read Random --\r\n" );
   
   i2cInit( I2C0, 100000 );
   debugPrintlnString( "I2C initialization complete." );

   Eeprom24xx1025_t eeprom24A1025I;

   eeprom24xx1025Init( &eeprom24A1025I,
                       I2C0, 0, 0,
                       -1, -1, -1, -1,
                       EEPROM24xx1025_PAGE_SIZE,
                       EEPROM_1024_K_BIT );
   debugPrintlnString( "EEPROM initialization complete." );

   if( buffSize > eeprom24A1025I.pageSize ) {
      debugPrintlnString( "\r\nEEPROM test Fail. Buffer size bigger than EEPROM page size." );
      return FALSE;
   }

   eeprom24xx1025PowerSet( &eeprom24A1025I, ON );
   debugPrintlnString( "\r\nEEPROM power on." );

   // Test writing to page

   uint32_t startingAddress = startingPage * eeprom24A1025I.pageSize;

   for( i=0; i<buffSize; i++ ) {
      if( eeprom24xx1025WriteByte( &eeprom24A1025I, startingAddress + i, buff[i] ) ) {
      } else {
         debugPrintString( "\r\nEEPROM test Fail. Error While trying to write at address: 0x" );
         debugPrintHex( i, 8 );
         debugPrintlnString( ".\r\n" );
         return FALSE;
      }
   }

   // Test reading from page

   for( i=0; i<buffSize; i++ ) {
      if( eeprom24xx1025ReadRandom( &eeprom24A1025I, startingAddress + i, &readByte) ) {
         if( readByte == buff[i] ) {
            debugPrintChar( readByte );
         } else {
            debugPrintString( "\r\nEEPROM test Fail. Error comparing byte: " );
            debugPrintChar( buff[i] );
            debugPrintString( " with: " );
            debugPrintChar( readByte );
            debugPrintString( ", at address: 0x" );
            debugPrintHex( i, 8 );
            debugPrintlnString( "." );

            eeprom24xx1025PowerSet( &eeprom24A1025I, OFF );
            debugPrintlnString( "EEPROM power off.\r\n" );

            return FALSE;
         }
      } else {
         debugPrintString( "\r\nEEPROM test Fail. Error trying to read at address: 0x" );
         debugPrintHex( i, 8 );
         debugPrintlnString( "." );

         eeprom24xx1025PowerSet( &eeprom24A1025I, OFF );
         debugPrintlnString( "EEPROM power off.\r\n" );

         return FALSE;
      }
   }

   debugPrintlnString( "\r\nEEPROM test complete sucessfull." );

   eeprom24xx1025PowerSet( &eeprom24A1025I, OFF );
   debugPrintlnString( "EEPROM power off.\r\n" );

   return TRUE;
}

/**
 * EEPROM Test Write a hole page
 * Performs a test on the 24xx1025 EEPROM memory.
 * @param uint8_t page. The page where save.
 * @param uint8_t* buff. The buffer to save in mamory.
 * @param uint32_t. The size of buffer (must be equal to page size).
 * @return bool_t. Whether the test was passed or not.
 */
bool_t eeprom24xx1025TestWritePage( uint32_t page, uint8_t* buff, uint32_t buffSize )
{
   uint8_t readByte = 0;
   uint32_t i = 0;

   boardInit();

   debugPrintConfigUart( UART_USB, 115200 );
   debugPrintlnString( "\r\n----------- EEPROM Test Write a hole page ---------------\r\n" );

   i2cInit( I2C0, 100000 );
   debugPrintlnString( "I2C initialization complete." );

   Eeprom24xx1025_t eeprom24A1025I;

   eeprom24xx1025Init( &eeprom24A1025I,
                       I2C0, 0, 0,
                       -1, -1, -1, -1,
                       EEPROM24xx1025_PAGE_SIZE,
                       EEPROM_1024_K_BIT );
   debugPrintlnString( "EEPROM initialization complete." );

   // Check if buffer size equal to EEPROM page size
   if( buffSize != eeprom24A1025I.pageSize ) {
      debugPrintlnString( "\r\nEEPROM test Fail. Buffer size non equal than EEPROM page size." );
      debugPrintString( "EEPROM page size: " );
      debugPrintlnInt( eeprom24A1025I.pageSize );
      debugPrintString( "Buffer page size: " );
      debugPrintlnInt( buffSize );
      debugPrintlnString( ".\r\n" );
      return FALSE;
   }

   eeprom24xx1025PowerSet( &eeprom24A1025I, ON );
   debugPrintlnString( "\r\nEEPROM power on." );

   // Test writing to page

   if( eeprom24xx1025WritePage( &eeprom24A1025I, page, buff, buffSize ) ) {
   } else {
      debugPrintString( "\r\nEEPROM test Fail. Error While trying to write at page: 0x" );
      debugPrintHex( page, 32 );
      debugPrintlnString( ".\r\n" );
      return FALSE;
   }

   // Test reading from page

   uint32_t startingAddress = page * eeprom24A1025I.pageSize;
   
   for( i=0; i<buffSize; i++ ) {
      if( eeprom24xx1025ReadByte( &eeprom24A1025I, startingAddress + i, &readByte) ) {
         if( readByte == buff[i] ) {
            debugPrintChar( readByte );
         } else {
            debugPrintString( "\r\nEEPROM test Fail. Error comparing byte: " );
            debugPrintChar( buff[i] );
            debugPrintString( " with: " );
            debugPrintChar( readByte );
            debugPrintString( ", at address: 0x" );
            debugPrintHex( i, 8 );
            debugPrintlnString( "." );

            eeprom24xx1025PowerSet( &eeprom24A1025I, OFF );
            debugPrintlnString( "EEPROM power off.\r\n" );

            return FALSE;
         }
      } else {
         debugPrintString( "\r\nEEPROM test Fail. Error trying to read at address: 0x" );
         debugPrintHex( i, 8 );
         debugPrintlnString( "." );

         eeprom24xx1025PowerSet( &eeprom24A1025I, OFF );
         debugPrintlnString( "EEPROM power off.\r\n" );

         return FALSE;
      }
   }

   debugPrintlnString( "\r\nEEPROM test complete sucessfull." );
   
   eeprom24xx1025PowerSet( &eeprom24A1025I, OFF );
   debugPrintlnString( "EEPROM power off.\r\n" );

   return TRUE;
}

/**
 * EEPROM Test Sequential read
 * Performs a test on the 24xx1025 EEPROM memory.
 */
bool_t eeprom24xx1025TestReadSequential( uint32_t address, 
                                         uint8_t* byteBuffer,
                                         uint32_t byteBufferSize )
{

   boardInit();

   debugPrintConfigUart( UART_USB, 115200 );
   debugPrintlnString( "\r\n----------- EEPROM Test Sequential read -----------------\r\n" );

   i2cInit( I2C0, 100000 );
   debugPrintlnString( "I2C initialization complete." );

   Eeprom24xx1025_t eeprom24A1025I;

   eeprom24xx1025Init( &eeprom24A1025I,
                       I2C0, 0, 0,
                       -1, -1, -1, -1,
                       EEPROM24xx1025_PAGE_SIZE,
                       EEPROM_1024_K_BIT );
   debugPrintlnString( "EEPROM initialization complete." );

   // Check if buffer size equal to EEPROM page size
   if( byteBufferSize > eeprom24A1025I.memorySize / 2 ) {
      debugPrintlnString( "\r\nEEPROM test Fail. Buffer more than half EEPROM memory size." );
      debugPrintString( "EEPROM memory size / 2: " );
      debugPrintlnInt( eeprom24A1025I.memorySize / 2 );
      debugPrintString( "Buffer page size: " );
      debugPrintlnInt( byteBufferSize );
      debugPrintlnString( ".\r\n" );
      return FALSE;
   }

   eeprom24xx1025PowerSet( &eeprom24A1025I, ON );
   debugPrintlnString( "\r\nEEPROM power on." );

   // Test Sequential read

   if( FALSE == eeprom24xx1025ReadSequential( &eeprom24A1025I, address,
                                              byteBuffer, byteBufferSize ) ) {
      debugPrintlnString( "\r\nEEPROM test Fail. Error trying to read sequential." );

      eeprom24xx1025PowerSet( &eeprom24A1025I, OFF );
      debugPrintlnString( "EEPROM power off.\r\n" );

      return FALSE;
   }

   debugPrintlnString( "Readed:" );
   debugPrintlnString( byteBuffer );
   debugPrintlnString( "EEPROM test complete sucessfull." );
   
   eeprom24xx1025PowerSet( &eeprom24A1025I, OFF );
   debugPrintlnString( "EEPROM power off.\r\n" );

   return TRUE;
}

/**
 * EEPROM Test Current Address Read
 */
bool_t eeprom24xx1025TestReadCurrentAddress( uint32_t dummyReadAddress, 
                                             uint8_t* byteBuffer )
{

   uint8_t readByte = 0;
   uint32_t i = 0;

   boardInit();

   debugPrintConfigUart( UART_USB, 115200 );
   debugPrintlnString( "\r\n-------- EEPROM Test Test Current Address Read ----------\r\n" );

   i2cInit( I2C0, 100000 );
   debugPrintlnString( "I2C initialization complete." );

   Eeprom24xx1025_t eeprom24A1025I;

   eeprom24xx1025Init( &eeprom24A1025I,
                       I2C0, 0, 0,
                       -1, -1, -1, -1,
                       EEPROM24xx1025_PAGE_SIZE,
                       EEPROM_1024_K_BIT );
   debugPrintlnString( "EEPROM initialization complete." );

   eeprom24xx1025PowerSet( &eeprom24A1025I, ON );
   debugPrintlnString( "\r\nEEPROM power on." );

   // Random read to set an initial address
   if( eeprom24xx1025ReadRandom( &eeprom24A1025I, dummyReadAddress, &readByte) ) {
      debugPrintChar( readByte );
   } else {
      debugPrintString( "\r\nEEPROM test Fail. Error First Read Random." );

      eeprom24xx1025PowerSet( &eeprom24A1025I, OFF );
      debugPrintlnString( "EEPROM power off.\r\n" );

      return FALSE;
   }

   // Test Current Address Read 10 times
   for( i=0; i<10; i++ ) {
      if( eeprom24xx1025ReadCurrentAddress( &eeprom24A1025I, 
                                            dummyReadAddress + i,
                                            &readByte ) ) {
         debugPrintChar( readByte );
      } else {
         debugPrintString( "\r\nEEPROM test Fail. Error Current Address Read." );

         eeprom24xx1025PowerSet( &eeprom24A1025I, OFF );
         debugPrintlnString( "EEPROM power off.\r\n" );

         return FALSE;
      }
   }

   debugPrintlnString( "\r\nEEPROM test complete sucessfull." );

   eeprom24xx1025PowerSet( &eeprom24A1025I, OFF );
   debugPrintlnString( "EEPROM power off.\r\n" );

   return TRUE;
}



/*==================[ISR external functions definition]======================*/

/*==================[end of file]============================================*/
