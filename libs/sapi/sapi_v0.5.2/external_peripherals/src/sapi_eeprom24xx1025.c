/* Copyright 2017, Eric Pernia.
 */

/* Date: 2017-07-27 */

/*==================[inclusions]=============================================*/

#include "sapi_eeprom24xx1025.h"   // <= own header

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

   delay(5); // Twc - Write cycle time (byte or page)

   return retVal; // Byte writed
}


// Page Write - NOT WORKING YET!!!!!
bool_t eeprom24xx1025WritePage( Eeprom24xx1025_t* eeprom, uint32_t pageAddress,
                                uint8_t* byteBuffer, uint32_t byteBufferSize )
{

   bool_t retVal = TRUE; // True if OK

   // Check buffer size
   if( byteBufferSize > eeprom->pageSize ) {
      return FALSE;
   }

   // Check page address
   //if( pageAddress > (eeprom->memorySize / eeprom->pageSize) ) { // PAGE_AMOUNT = MEMORY_SIZE / PAGE_SIZE
   //   return FALSE;
   //}

   uint16_t i=0;

   uint16_t memoryAddress = pageAddress * eeprom->pageSize;

   uint8_t dataToWrite[ byteBufferSize+2 ]; // 2 bytes more for memory address

   // Memory address High
   dataToWrite[0] = EEPROM_ADDRESS_HIGH( memoryAddress );
   // Memory address Low
   dataToWrite[1] = EEPROM_ADDRESS_LOW( memoryAddress );

   // Bytes to write
   for( i=0; i<byteBufferSize; i++ ) {
      dataToWrite[i+2] = byteBuffer[i];
   }

   /* uint8_t i2cNumber, uint8_t  i2cSlaveAddress,
      uint8_t* transmitDataBuffer, uint16_t transmitDataBufferSize,
      bool_t sendWriteStop */
   retVal = i2cWrite( eeprom->i2c,
                      eeprom24xx1025I2cAddress( eeprom, memoryAddress ),
                      dataToWrite, (byteBufferSize+2), TRUE );

   delay(5); // Twc - Write cycle time (byte or page)

   return retVal; // Byte writed
}


//-----------------------------------------------------------------------------
// READ OPERATIONS
//-----------------------------------------------------------------------------

// Current Address Read
bool_t eeprom24xx1025ReadCurrentAddress( Eeprom24xx1025_t* eeprom,
      uint8_t* readedByte )
{
   // TODO: Complete!
   bool_t retVal = TRUE; // True if OK
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

   //delay(1); // ??? - Read cycle time (byte or page)

   return retVal; // read correct
}


// Sequential Read
bool_t eeprom24xx1025ReadSequential( Eeprom24xx1025_t* eeprom,
                                     uint8_t* byteBuffer,
                                     int32_t byteBufferSize )
{
   // TODO: Cmplete!
   bool_t retVal = TRUE; // True if OK
   return retVal; // read correct
}

/*---------------------------------------------------------------------------*/

/*
bool_t eeprom24xx1025Read( uint16_t memoryAddress,
                          uint8_t* byteBuffer, uint16_t byteBufferSize ){

   bool_t retVal = TRUE; // True if OK

   // Check memory address
   if( memoryAddress > EEPROM24xx1025_MEMORY_SIZE ){
      return FALSE;
   }

   // Check buffer size
   if( byteBufferSize > EEPROM24xx1025_MEMORY_SIZE ){
      return FALSE;
   }

   uint16_t i=0;

   uint8_t addressToRead[ 2 ]; // 2 bytes for memory address

   addressToRead[0] = (uint8_t)(memoryAddress>>1);        // Memory address High
   addressToRead[1] = (uint8_t)(memoryAddress&&(0x00FF)); // Memory address Low
*/
/* uint8_t i2cNumber, uint8_t i2cSlaveAddress,
   uint8_t* dataToReadBuffer, uint16_t dataToReadBufferSize,
   bool_t sendWriteStop,
   uint8_t* reciveDataBuffer, uint16_t reciveDataBufferSize,
   bool_t sendReadStop */
/*   retVal = i2cRead( EEPROM_I2C, eeprom24xx1025_i2cAddress,
                     addressToRead, 2, TRUE,
                     byteBuffer, byteBufferSize, TRUE );

   delay(5); // Twc - Write cycle time (byte or page)

   return retVal; // read correct
}
*/

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
 * EEPROM Test
 * Performs a test on the 24xx1025 EEPROM memory.
 * @param uint8_t startingPage. The page where to start the test.
 * @param uint8_t* buff. The buffer to save in mamory.
 * @param uint8_t. The size of buffer (must be lower than page size).
 * @return bool_t. Whether the test was passed or not.
 */
bool_t eeprom24xx1025Test( uint32_t startingPage, uint8_t* buff, uint8_t buffSize )
{
   uint8_t readByte = 0;
   uint32_t i = 0;

   boardInit();

   debugPrintConfigUart( UART_USB, 115200 );

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
      debugPrintlnString( "EEPROM test Fail. Buffer size bigger than EEPROM page size." );
      return FALSE;
   }

   eeprom24xx1025PowerSet( &eeprom24A1025I, ON );
   debugPrintlnString( "EEPROM power on." );

   // Test writing to page

   uint32_t startingAddress = startingPage * eeprom24A1025I.pageSize;

   for( i=0; i<buffSize; i++ ) {
      if( eeprom24xx1025WriteByte( &eeprom24A1025I, startingAddress + i, buff[i] ) ) {
      } else {
         debugPrintString( "EEPROM test Fail. Error While trying to write at address: 0x" );
         debugPrintHex( i, 8 );
         debugPrintlnString( "." );
         return FALSE;
      }
   }

   // Test reading from page

   for( i=0; i<buffSize; i++ ) {
      if( eeprom24xx1025ReadByte( &eeprom24A1025I, startingAddress + i, &readByte) ) {
         if( readByte == buff[i] ) {
            debugPrintChar( readByte );
         } else {
            debugPrintString( "EEPROM test Fail. Error comparing byte: " );
            debugPrintChar( buff[i] );
            debugPrintString( " with: " );
            debugPrintChar( readByte );
            debugPrintString( ", at address: 0x" );
            debugPrintHex( i, 8 );
            debugPrintlnString( "." );

            eeprom24xx1025PowerSet( &eeprom24A1025I, OFF );
            debugPrintlnString( "EEPROM power off." );

            return FALSE;
         }
      } else {
         debugPrintString( "EEPROM test Fail. Error trying to read at address: 0x" );
         debugPrintHex( i, 8 );
         debugPrintlnString( "." );

         eeprom24xx1025PowerSet( &eeprom24A1025I, OFF );
         debugPrintlnString( "EEPROM power off." );

         return FALSE;
      }
   }

   // Test writing to higher part

   // Test reading from higher part

   eeprom24xx1025PowerSet( &eeprom24A1025I, OFF );
   debugPrintlnString( "EEPROM power off." );

   debugPrintlnString( "EEPROM test complete sucessfull." );

   return TRUE;

   /*
      magPowerSet( OFF ); // Jode la sguiente lectura:

      // Random Read (Byte read)
      myByte = 0;
      if( eeprom24xx1025ReadRandom( 0x00FE, &myByte ) ){
         consolePrintString( "Se leyo: 0x" );
         consolePrintHex( myByte );
         consolePrintlnString( ", en la direccion: 0x00FE." );
      } else{
         consolePrintlnString( "No se pudo leer EEPROM." );
      }

      magPowerSet( ON );// La proxima lectura lee bien:

      // Random Read (Byte read)
      myByte = 0;
      if( eeprom24xx1025ReadRandom( 0x00FE, &myByte ) ){
         consolePrintString( "Se leyo: 0x" );
         consolePrintHex( myByte );
         consolePrintlnString( ", en la direccion: 0x00FE." );
      } else{
         consolePrintlnString( "No se pudo leer EEPROM." );
      }

      consolePrintEnter();
   */
}

/*---------------------------------------------------------------------------*/

/*

void eeprom24xx1025Read( uint16_t address,
                        int theByteCount,
                        uint8_t* theByteArray ){

   uint16_t i=0;

   for( i = 0; i < theByteCount; i++ ){
      Wire.beginTransmission(theDeviceAddress);
      Wire.send((uint8_t)((address + i) >> 8));
      Wire.send((uint8_t)((address + i) >> 0));
      Wire.endTransmission();
      delay(5);
      Wire.requestFrom(theDeviceAddress, sizeof(uint8_t));
      theByteArray[i] = Wire.receive();
   }
}


uint8_t eeprom24xx1025ReadByte( int theDeviceAddress, unsigned int theMemoryAddress ){
   uint8_t theByteArray[sizeof(uint8_t)];
   eeprom24xx1025Read(theDeviceAddress, theMemoryAddress, sizeof(uint8_t), theByteArray);
   return (uint8_t)(((theByteArray[0] << 0)));
}

void eeprom24xx1025WriteByte( int theDeviceAddress, unsigned int theMemoryAddress, uint8_t theByte ){
   uint8_t theByteArray[sizeof(uint8_t)] = {(uint8_t)(theByte >> 0)};
   eeprom24xx1025Write(theDeviceAddress, theMemoryAddress, sizeof(uint8_t), theByteArray);
}
*/

/*
int eeprom24xx1025ReadInt(int theDeviceAddress, unsigned int theMemoryAddress) {
   byte theByteArray[sizeof(int)];
   WireEepromRead(theDeviceAddress, theMemoryAddress, sizeof(int), theByteArray);
   return (int)(((theByteArray[0] << 8)) | (int)((theByteArray[1] << 0)));
}
*/
/*
void WireEepromWriteInt(int theDeviceAddress, unsigned int theMemoryAddress, int theInt) {
   byte theByteArray[sizeof(int)] = {(byte)(theInt >> 8), (byte)(theInt >> 0)};
   eeprom24xx1025Write(theDeviceAddress, theMemoryAddress, sizeof(int), theByteArray);
}
*/


/*---------------------------------------------------------------------------*/


/*==================[ISR external functions definition]======================*/

/*==================[end of file]============================================*/
