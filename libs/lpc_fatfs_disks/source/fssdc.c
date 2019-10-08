/*
    FatFS SD/MMC Low level code
    Copyright 2018 Santiago Germino (royconejo@gmail.com)
    Copyright 2007 Elm-ChaN, FatFs.

    Redistribution and use in source and binary forms, with or without
    modification, are permitted provided that the following conditions are met:

    1.  Redistributions of source code must retain the above copyright notice,
        this list of conditions and the following disclaimer.

    2.  Redistributions in binary form must reproduce the above copyright
        notice, this list of conditions and the following disclaimer in the
        documentation and/or other materials provided with the distribution.

    3.  Neither the name of the copyright holder nor the names of its
        contributors may be used to endorse or promote products derived from
        this software without specific prior written permission.

    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
    AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
    IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
    ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
    LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
    CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
    SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
    INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
    CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
    ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
    POSSIBILITY OF SUCH DAMAGE.
*/

#include "fssdc.h"

/*  ELM-Chan Says:
 
    On Initialization: Set SPI clock rate between 100 kHz and 400 kHz.
    After: The SCLK rate should be changed to fast as possible to maximize the 
    read/write performance. The TRAN_SPEED field in the CSD register indicates 
    the maximum clock rate of the card. The maximum clock rate is 20MHz for MMC,
    25MHz for SDC in most case.
    
    http://elm-chan.org/docs/mmc/mmc_e.html
*/

// #define FSSDC_SUPPORTS_HOT_INSERTION  

#ifndef FSSDC_CS_PIN
#define FSSDC_CS_PIN                    0
#endif

#ifndef FSSDC_CS_PORT
#define FSSDC_CS_PORT                   3
#endif 

#ifndef FSSDC_SPI_SLOW_CLOCK
#define FSSDC_SPI_SLOW_CLOCK            100000
#endif

#ifndef FSSDC_SPI_FAST_CLOCK
#define FSSDC_SPI_FAST_CLOCK            15000000
#endif


/* Definitions for MMC/SDC command */
#define CMD0_	(0x40+0)	 /* GO_IDLE_STATE */
#define CMD1	(0x40+1)	 /* SEND_OP_COND (MMC) */
#define	ACMD41	(0xC0+41)	 /* SEND_OP_COND (SDC) */
#define CMD8	(0x40+8)	 /* SEND_IF_COND */
#define CMD9	(0x40+9)	 /* SEND_CSD */
#define CMD10	(0x40+10)	 /* SEND_CID */
#define CMD12	(0x40+12)	 /* STOP_TRANSMISSION */
#define ACMD13	(0xC0+13)	 /* SD_STATUS (SDC) */
#define CMD16	(0x40+16)	 /* SET_BLOCKLEN */
#define CMD17	(0x40+17)	 /* READ_SINGLE_BLOCK */
#define CMD18	(0x40+18)	 /* READ_MULTIPLE_BLOCK */
#define CMD23	(0x40+23)	 /* SET_BLOCK_COUNT (MMC) */
#define ACMD23	(0xC0+23)	 /* SET_WR_BLK_ERASE_COUNT (SDC) */
#define CMD24	(0x40+24)	 /* WRITE_BLOCK */
#define CMD25	(0x40+25)	 /* WRITE_MULTIPLE_BLOCK */
#define CMD55	(0x40+55)	 /* APP_CMD */
#define CMD58	(0x40+58)	 /* READ_OCR */

#define BOOL 	bool

/* Port Controls  (Platform dependent) */
#define CS_LOW()    Chip_GPIO_SetPinOutLow(LPC_GPIO_PORT, FSSDC_CS_PORT, FSSDC_CS_PIN)
#define CS_HIGH()   Chip_GPIO_SetPinOutHigh(LPC_GPIO_PORT, FSSDC_CS_PORT, FSSDC_CS_PIN)

#define	FCLK_SLOW()	Chip_SSP_SetBitRate(LPC_SSP1, g_spiSlowClock);  /* Set slow clock (100k-400k) */
#define	FCLK_FAST()	Chip_SSP_SetBitRate(LPC_SSP1, g_spiFastClock);	/* Set fast clock (depends on the CSD) */

/* MMC card type flags (MMC_GET_TYPE) */
#define CT_MMC		0x01                /* MMC ver 3 */
#define CT_SD1		0x02            	/* SD ver 1 */
#define CT_SD2		0x04                /* SD ver 2 */
#define CT_SDC		(CT_SD1|CT_SD2)     /* SD */
#define CT_BLOCK	0x08                /* Block addressing */


static volatile uint32_t g_spiSlowClock = FSSDC_SPI_SLOW_CLOCK;
static volatile uint32_t g_spiFastClock = FSSDC_SPI_FAST_CLOCK;

/* Disk status */
static volatile DSTATUS g_diskStats = STA_NOINIT | STA_NODISK;
/* 100Hz decrement timer */
static volatile BYTE Timer1, Timer2;
/* Card type flags */
static BYTE CardType;


static FSSDC_StatusUpdateCallback_Func  g_statusUpdateCallback = NULL;
static enum FSSDC_CardStatus            g_cardStatus = FSSDC_CardStatus_Removed;


static void newCardStatus (enum FSSDC_CardStatus cardStatus)
{
    g_cardStatus = cardStatus;
    if (g_statusUpdateCallback)
    {
        g_statusUpdateCallback (g_cardStatus);
    }
}


/*--------------------------------------------------------------------------

   Module Private Functions

---------------------------------------------------------------------------*/

static void SSPSend(uint8_t *buf, uint32_t Length)
{
    Chip_SSP_DATA_SETUP_T xferConfig;

	xferConfig.tx_data = buf;
	xferConfig.tx_cnt  = 0;
	xferConfig.rx_data = NULL;
	xferConfig.rx_cnt  = 0;
	xferConfig.length  = Length;

	Chip_SSP_RWFrames_Blocking(LPC_SSP1, &xferConfig);
}


void SSPReceive( uint8_t *buf, uint32_t Length )
{
    Chip_SSP_DATA_SETUP_T xferConfig;

	xferConfig.tx_data = NULL;
	xferConfig.tx_cnt  = 0;
	xferConfig.rx_data = buf;
	xferConfig.rx_cnt  = 0;
	xferConfig.length  = Length;

	Chip_SSP_RWFrames_Blocking(LPC_SSP1, &xferConfig);
}


/*-----------------------------------------------------------------------*/
/* Transmit a byte to MMC via SPI  (Platform dependent)                  */
/*-----------------------------------------------------------------------*/
//#define xmit_spi(dat) (SSPSend((uint8_t*)&(dat), 1))
static void xmit_spi(BYTE dat)
{
    SSPSend((uint8_t*) &dat, 1);
}


/*-----------------------------------------------------------------------*/
/* Receive a byte from MMC via SPI  (Platform dependent)                 */
/*-----------------------------------------------------------------------*/
static
BYTE rcvr_spi (void)
{
    BYTE data = 0;

    SSPReceive(&data, 1);

    return data;
}


/* Alternative macro to receive data fast */
#define rcvr_spi_m(dst) \
    do { \
        SSPReceive((uint8_t*)(dst), 1); \
    } while(0)


/*-----------------------------------------------------------------------*/
/* Wait for card ready                                                   */
/*-----------------------------------------------------------------------*/
static
BYTE wait_ready (void)
{
	BYTE res;

	Timer2 = 50;	/* Wait for ready in timeout of 500ms */
	rcvr_spi();
	do
		res = rcvr_spi();
	while ((res != 0xFF) && Timer2);

	return res;
}


/*-----------------------------------------------------------------------*/
/* Deselect the card and release SPI bus                                 */
/*-----------------------------------------------------------------------*/
static
void deselect (void)
{
	CS_HIGH();
	rcvr_spi();
}


/*-----------------------------------------------------------------------*/
/* Select the card and wait ready                                        */
/*-----------------------------------------------------------------------*/
static
BOOL select_ (void)	/* TRUE:Successful, FALSE:Timeout */
{
	CS_LOW();
	if (wait_ready() != 0xFF) {
		deselect();
		return FALSE;
	}
	return TRUE;
}


/*-----------------------------------------------------------------------*/
/* Power Control  (Platform dependent)                                   */
/*-----------------------------------------------------------------------*/
/* When the target system does not support socket power control, there   */
/* is nothing to do in these functions and chk_power always returns 1.   */
static
void power_on (void)
{
}


static
void power_off (void)
{
}


static
int chk_power(void)		/* Socket power state: 0=off, 1=on */
{
	return 1;
}


/*-----------------------------------------------------------------------*/
/* Receive a data packet from MMC                                        */
/*-----------------------------------------------------------------------*/
static
BOOL rcvr_datablock (
	BYTE *buff,			/* Data buffer to store received data */
	UINT btr			/* Byte count (must be multiple of 4) */
)
{
	BYTE token;

	Timer1 = 20;
	do                              /* Wait for data packet in timeout of 200ms */
    {
		token = rcvr_spi();
	} 
    while ((token == 0xFF) && Timer1);
    
	if (token != 0xFE) 
    {
        return FALSE;               /* If not valid data token, retutn with error */
    }

	do                              /* Receive the data block into buffer */
    {							
		rcvr_spi_m(buff++);
		rcvr_spi_m(buff++);
		rcvr_spi_m(buff++);
		rcvr_spi_m(buff++);
	}
    while (btr -= 4);
    
	rcvr_spi();						/* Discard CRC */
	rcvr_spi();

	return TRUE;					/* Return with success */
}


/*-----------------------------------------------------------------------*/
/* Send a data packet to MMC                                             */
/*-----------------------------------------------------------------------*/
#if !FF_FS_READONLY
static
BOOL xmit_datablock (
	const BYTE *buff,	/* 512 byte data block to be transmitted */
	BYTE token			/* Data/Stop token */
)
{
	BYTE resp, wc;

	if (wait_ready() != 0xFF) 
    {
        return FALSE;
    }

	xmit_spi(token);					/* Xmit data token */
	if (token != 0xFD)                  /* Is data token */
    {                                   
		wc = 0;
		do                              /* Xmit the 512 byte data block to MMC */
        {							
			xmit_spi(*buff++);
			xmit_spi(*buff++);
		}
        while (--wc);
        
		xmit_spi(0xFF);					/* CRC (Dummy) */
		xmit_spi(0xFF);
        
		resp = rcvr_spi();				/* Reveive data response */
        
		if ((resp & 0x1F) != 0x05)		/* If not accepted, return with error */
        {
			return FALSE;
        }
	}

	return TRUE;
}
#endif /* !FF_FS_READONLY */


/*-----------------------------------------------------------------------*/
/* Send a command packet to MMC                                          */
/*-----------------------------------------------------------------------*/
static
BYTE send_cmd (
	BYTE cmd,		/* Command byte */
	DWORD arg		/* Argument */
)
{
	BYTE n, res;


    if (cmd & 0x80)                     /* ACMD<n> is the command sequense of CMD55-CMD<n> */
    {
		cmd &= 0x7F;
		res = send_cmd(CMD55, 0);
		if (res > 1) return res;
	}

	/* Select the card and wait for ready */
	deselect();
	if (!select_()) 
    {
        return 0xFF;
    }

	/* Send command packet */
	xmit_spi(cmd);						/* Start + Command index */
	xmit_spi((BYTE)(arg >> 24));		/* Argument[31..24] */
	xmit_spi((BYTE)(arg >> 16));		/* Argument[23..16] */
	xmit_spi((BYTE)(arg >> 8));			/* Argument[15..8] */
	xmit_spi((BYTE)arg);				/* Argument[7..0] */
	n = 0x01;							/* Dummy CRC + Stop */
    
	if (cmd == CMD0_) 
    {
        n = 0x95;                       /* Valid CRC for CMD0_(0) */
    }
    
	if (cmd == CMD8)
    {
        n = 0x87;                       /* Valid CRC for CMD8(0x1AA) */
    }
    
	xmit_spi(n);

	/* Receive command response */
	if (cmd == CMD12) 
    {
        rcvr_spi();                     /* Skip a stuff byte when stop reading */
    }
    
	n = 10;								/* Wait for a valid response in timeout of 10 attempts */
	do
    {
		res = rcvr_spi();
    }
	while ((res & 0x80) && --n);

	return res;			/* Return with the response value */
}


/*--------------------------------------------------------------------------

   Public Functions

---------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------*/
/* Get Disk Status                                                       */
/*-----------------------------------------------------------------------*/

DSTATUS FSSDC_FatFs_DiskStatus ()
{
	return g_diskStats;
}


/*-----------------------------------------------------------------------*/
/* Initialize Disk Drive                                                 */
/*-----------------------------------------------------------------------*/

DSTATUS FSSDC_FatFs_DiskInitialize ()
{
	BYTE n, cmd, ty, ocr[4];

    if (g_diskStats & STA_NODISK)
    {
        Board_UARTPutSTR ("FSSDC: [Init] No card in socket.\r\n");
        return g_diskStats;                 /* No card in the socket */
    }
    
    if (!(g_diskStats & STA_NOINIT))        /* Already initialized */
    {
        return g_diskStats;
    }
    
    Board_UARTPutSTR ("FSSDC: [Init] Initialization begins.\r\n");
    
	power_on();                             /* Force socket power on */
	FCLK_SLOW();
    
	for (n = 10; n; n--) 
    {
        rcvr_spi();	/* 80 dummy clocks */
    }
    
    Board_UARTPutSTR ("FSSDC: [Init] New card status: Native Mode.\r\n");
    newCardStatus (FSSDC_CardStatus_NativeMode);    

	ty = 0;
	if (send_cmd(CMD0_, 0) == 1)                                /* Enter Idle state */
    {
        Board_UARTPutSTR ("FSSDC: [Init] New card status: Initializing.\r\n");        
        newCardStatus (FSSDC_CardStatus_Initializing);
        
		Timer1 = 100;                                           /* Initialization timeout of 1000 msec */
		if (send_cmd(CMD8, 0x1AA) == 1)                         /* SDHC */
        {                                                       
			for (n = 0; n < 4; n++) 
            {
                ocr[n] = rcvr_spi();                            /* Get trailing return value of R7 resp */
            }

			if (ocr[2] == 0x01 && ocr[3] == 0xAA)               /* The card can work at vdd range of 2.7-3.6V */ 
            {				
				while (Timer1 && send_cmd(ACMD41, 1UL << 30));	/* Wait for leaving idle state (ACMD41 with HCS bit) */

                if (Timer1 && send_cmd(CMD58, 0) == 0) 
                {		
					for (n = 0; n < 4; n++)                     /* Check CCS bit in the OCR */
                    {
                        ocr[n] = rcvr_spi();
                    }
					ty = (ocr[0] & 0x40) ? CT_SD2 | CT_BLOCK : CT_SD2;	/* SDv2 */
				}
			}
		} 
        else 
        {                                                       /* SDSC or MMC */
			if (send_cmd(ACMD41, 0) <= 1)
            {
				ty = CT_SD1; cmd = ACMD41;                      /* SDv1 */
			} else {
				ty = CT_MMC; cmd = CMD1;                        /* MMCv3 */
			}
            
			while (Timer1 && send_cmd(cmd, 0));			/* Wait for leaving idle state */
            
			if (!Timer1 || send_cmd(CMD16, 512) != 0)	/* Set R/W block length to 512 */
            {
				ty = 0;
            }
		}
	}
	CardType = ty;
	deselect();

    if (ty)                                             /* Initialization succeded */
    {
		g_diskStats &= ~STA_NOINIT;                     /* Clear STA_NOINIT */
		FCLK_FAST();
        Board_UARTPutSTR ("FSSDC: [Init] New card status: Ready (Fast Clock).\r\n");        
        newCardStatus (FSSDC_CardStatus_Ready);        
	} 
    else                                                /* Initialization failed */ 
    {
		power_off();
        Board_UARTPutSTR ("FSSDC: [Init] New card status: Error (Not Initialized).\r\n");        
        newCardStatus (FSSDC_CardStatus_Error); 
	}

	return g_diskStats;
}


/*-----------------------------------------------------------------------*/
/* Read Sector(s)                                                        */
/*-----------------------------------------------------------------------*/
DRESULT FSSDC_FatFs_DiskRead (BYTE *buff, DWORD sector, UINT count)
{   
	if (g_diskStats & STA_NOINIT)
    {
        Board_UARTPutSTR ("FSSDC: [Read] Card not initialized.\r\n");
        return RES_NOTRDY;
    }

	if (!(CardType & CT_BLOCK)) 
    {
        sector *= 512;                      /* Convert to byte address if needed */
    }

	if (count == 1) 
    {                                       /* Single block read */
		if ((send_cmd(CMD17, sector) == 0)	/* READ_SINGLE_BLOCK */
			&& rcvr_datablock(buff, 512))
			count = 0;
	}
	else 
    {                                       /* Multiple block read */
		if (send_cmd(CMD18, sector) == 0) 
        {                                   /* READ_MULTIPLE_BLOCK */
			do 
            {
				if (!rcvr_datablock(buff, 512)) 
                {
                    break;
                }
				buff += 512;
			} 
            while (--count);
			send_cmd(CMD12, 0);				/* STOP_TRANSMISSION */
		}
	}
	deselect();

	return count ? RES_ERROR : RES_OK;
}



/*-----------------------------------------------------------------------*/
/* Write Sector(s)                                                       */
/*-----------------------------------------------------------------------*/
#if !FF_FS_READONLY
DRESULT FSSDC_FatFs_DiskWrite (const BYTE *buff, DWORD sector, UINT count)
{
	if (g_diskStats & STA_NOINIT)
    {
        Board_UARTPutSTR ("FSSDC: [Write] Card not initialized.\r\n");
        return RES_NOTRDY;
    }
    
	if (g_diskStats & STA_PROTECT)
    {
        Board_UARTPutSTR ("FSSDC: [Write] Card is write protected.\r\n");
        return RES_WRPRT;
    }

	if (!(CardType & CT_BLOCK)) sector *= 512;	/* Convert to byte address if needed */

	if (count == 1) 
    {                                           /* Single block write */
		if ((send_cmd(CMD24, sector) == 0)      /* WRITE_BLOCK */
                    && xmit_datablock(buff, 0xFE))
        {
			count = 0;
        }
	}
	else 
    {                                           /* Multiple block write */
		if (CardType & CT_SDC) send_cmd(ACMD23, count);
		if (send_cmd(CMD25, sector) == 0) 
        {                                       /* WRITE_MULTIPLE_BLOCK */
			do 
            {
				if (!xmit_datablock(buff, 0xFC))
                {
                    break;
                }
				buff += 512;
			} 
            while (--count);
			if (!xmit_datablock(0, 0xFD))       /* STOP_TRAN token */
            {
				count = 1;
            }
		}
	}
	deselect();

	return count ? RES_ERROR : RES_OK;
}
#endif /* !FF_FS_READONLY */


/*-----------------------------------------------------------------------*/
/* Miscellaneous Functions                                               */
/*-----------------------------------------------------------------------*/
DRESULT FSSDC_FatFs_DiskIoCtl (BYTE cmd, void *buff)
{
	DRESULT res;
	BYTE n, csd[16], *ptr = buff;
	WORD csize;

	res = RES_ERROR;

	if (cmd == CTRL_POWER)
    {
		switch (*ptr) {
		case 0:                             /* Sub control code == 0 (POWER_OFF) */
			if (chk_power())
				power_off();                /* Power off */
			res = RES_OK;
			break;
		case 1:                             /* Sub control code == 1 (POWER_ON) */
			power_on();                     /* Power on */
			res = RES_OK;
			break;
		case 2:                             /* Sub control code == 2 (POWER_GET) */
			*(ptr+1) = (BYTE)chk_power();
			res = RES_OK;
			break;
		default :
			res = RES_PARERR;
		}
	}
	else 
    {
		if (g_diskStats & STA_NOINIT)
        {
            Board_UARTPutSTR ("FSSDC: [IOCtl] Card not initialized.\r\n");
            return RES_NOTRDY;
        }

		switch (cmd) {
		case CTRL_SYNC :                    /* Make sure that no pending write process. Do not remove this or written sector might not left updated. */
			if (select_()) 
            {
				res = RES_OK;
				deselect();
			}
			break;

		case GET_SECTOR_COUNT :             /* Get number of sectors on the disk (DWORD) */
			if ((send_cmd(CMD9, 0) == 0) 
                    && rcvr_datablock(csd, 16))
            {
				if ((csd[0] >> 6) == 1) 
                {                           /* SDC ver 2.00 */
					csize = csd[9] + ((WORD)csd[8] << 8) + 1;
					*(DWORD*)buff = (DWORD)csize << 10;
				} 
                else
                {                           /* SDC ver 1.XX or MMC*/
					n = (csd[5] & 15) + ((csd[10] & 128) >> 7) + ((csd[9] & 3) << 1) + 2;
					csize = (csd[8] >> 6) + ((WORD)csd[7] << 2) + ((WORD)(csd[6] & 3) << 10) + 1;
					*(DWORD*)buff = (DWORD)csize << (n - 9);
				}
				res = RES_OK;
			}
			break;

		case GET_SECTOR_SIZE :              /* Get R/W sector size (WORD) */
			*(WORD*)buff = 512;
			res = RES_OK;
			break;

		case GET_BLOCK_SIZE :               /* Get erase block size in unit of sector (DWORD) */
			if (CardType & CT_SD2) {                            /* SDC ver 2.00 */
				if (send_cmd(ACMD13, 0) == 0)                   /* Read SD status */
                {                 
					rcvr_spi();
					if (rcvr_datablock(csd, 16))                /* Read partial block */
                    {				
						for (n = 64 - 16; n; n--) 
                        {
                            rcvr_spi();                         /* Purge trailing data */
                        }
						*(DWORD*)buff = 16UL << (csd[10] >> 4);
						res = RES_OK;
					}
				}
			} 
            else 
            {                                                   /* SDC ver 1.XX or MMC */
				if ((send_cmd(CMD9, 0) == 0) 
                        && rcvr_datablock(csd, 16)) 
                {                                               /* Read CSD */
					if (CardType & CT_SD1)
                    {                                           /* SDC ver 1.XX */
						*(DWORD*)buff = (((csd[10] & 63) << 1) + ((WORD)(csd[11] & 128) >> 7) + 1) << ((csd[13] >> 6) - 1);
					} 
                    else 
                    {                                           /* MMC */
						*(DWORD*)buff = ((WORD)((csd[10] & 124) >> 2) + 1) * (((csd[11] & 3) << 3) + ((csd[11] & 224) >> 5) + 1);
					}
					res = RES_OK;
				}
			}
			break;

		case MMC_GET_TYPE :                 /* Get card type flags (1 byte) */
			*ptr = CardType;
			res = RES_OK;
			break;

		case MMC_GET_CSD :                  /* Receive CSD as a data block (16 bytes) */
			if (send_cmd(CMD9, 0) == 0		/* READ_CSD */
                    && rcvr_datablock(ptr, 16))
            {
				res = RES_OK;
            }
			break;

		case MMC_GET_CID :                  /* Receive CID as a data block (16 bytes) */
			if (send_cmd(CMD10, 0) == 0		/* READ_CID */
                    && rcvr_datablock(ptr, 16))
            {
				res = RES_OK;
            }
			break;

		case MMC_GET_OCR :                  /* Receive OCR as an R3 resp (4 bytes) */
			if (send_cmd(CMD58, 0) == 0) 
            {                               /* READ_OCR */
				for (n = 4; n; n--) 
                {
                    *ptr++ = rcvr_spi();
                }
				res = RES_OK;
			}
			break;

		case MMC_GET_SDSTAT :               /* Receive SD statsu as a data block (64 bytes) */
			if (send_cmd(ACMD13, 0) == 0) 
            {                               /* SD_STATUS */
				rcvr_spi();
				if (rcvr_datablock(ptr, 64))
                {
					res = RES_OK;
                }
			}
			break;

		default:
			res = RES_PARERR;
		}

		deselect();
	}
    
    if (res != RES_OK)
    {
        Board_UARTPutSTR ("FSSDC: [IOCtl] Returning Error.\r\n");        
    }

	return res;
}


/*
--------------------------------------------------------------------------------
    WARNING: for card connectors without presence detection
    >>>> Only call this function when the card is inserted!
--------------------------------------------------------------------------------
*/
void FSSDC_InitSPI ()
{
    /*
        ELM-ChaN Says:
        
        the data is transferred in byte oriented serial communication
      
        The SPI mode 0 is defined for SDC. For the MMC, it is not the SPI spec, both
        latch and shift operations are defined with rising edge of the SCLK, but it
        seems to work at mode 0 at the SPI mode. Thus the SPI mode 0 (CPHA=0, CPOL=0)
        is the proper setting to control MMC/SDC
        
        http://elm-chan.org/docs/mmc/mmc_e.html#spimode
    */
    Chip_SSP_Init       (LPC_SSP1);
    Chip_SSP_Set_Mode   (LPC_SSP1, SSP_MODE_MASTER);
    Chip_SSP_SetFormat  (LPC_SSP1, SSP_BITS_8, SSP_FRAMEFORMAT_SPI,
                         SSP_CLOCK_CPHA0_CPOL0);
    FCLK_SLOW           ();
    Chip_SSP_Enable     (LPC_SSP1);
    
    // PLEASE NOTE: muxing and direction of CS signal on FSSDC_CS_{PIN/PORT}
    //              must be configured beforehand!
    deselect ();    
    
#ifndef FSSDC_SUPPORTS_HOT_INSERTION
    g_diskStats &= ~STA_NODISK;
    Board_UARTPutSTR ("FSSDC: [InitSPI] New card status: Inserted.\r\n");
    newCardStatus (FSSDC_CardStatus_Inserted);
    FSSDC_FatFs_DiskInitialize ();
#endif
}


uint32_t FSSDC_GetSlowClock ()
{
    return g_spiSlowClock;
}


uint32_t FSSDC_GetFastClock ()
{
    return g_spiFastClock;
}


void FSSDC_SetSlowClock (uint32_t hz)
{
    g_spiSlowClock = hz;
}


void FSSDC_SetFastClock (uint32_t hz)
{
    g_spiFastClock = hz;
}


enum FSSDC_CardStatus FSSDC_CardStatus ()
{
    return g_cardStatus;
}


void FSSDC_SetStatusUpdateCallback (FSSDC_StatusUpdateCallback_Func func)
{
    g_statusUpdateCallback = func;
}


/*-----------------------------------------------------------------------*/
/* Device Timer Interrupt Procedure  (Platform dependent)                */
/*-----------------------------------------------------------------------*/
/* This function must be called in period of 10ms                        */
void FSSDC_Update ()
{   
    if (Timer1)             /* 100Hz decrement timer */
    {
        -- Timer1;
    }
    
    if (Timer2)
    {
        -- Timer2;
    }

    // sgermino: not useful on Edu-CIAA; must be connected to the card detection 
    //           switch of a suitable SD card connector which is not present on
    //           a typical "arduino module".
    #if FSSDC_SUPPORTS_HOT_INSERTION
    static BYTE pv;
	BYTE n, s;    
    
	n = pv;
	//pv = SOCKPORT & (SOCKWP | SOCKINS);	/* Sample socket switch */

	/* TODO: Actualizar!!! */
	//pv = ((GPIO_ReadValue(2) & (1<<11)) != 0);
	pv = (Chip_GPIO_GetPinState(LPC_GPIO_PORT, 2, 11) != 0);

	if (n == pv) {					/* Have contacts stabled? */
		s = Stat;

		/* write protect NOT supported */

		/* check card detect */
		if (pv)			       /* (Socket empty) */
			s |= (STA_NODISK | STA_NOINIT);
		else				       /* (Card inserted) */
			s &= ~STA_NODISK;

		g_diskStats = s;
	}
    #endif
}
