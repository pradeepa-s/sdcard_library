/*-----------------------------------------------------------------------*/
/* Low level disk I/O module skeleton for FatFs     (C)ChaN, 2016        */
/*-----------------------------------------------------------------------*/
/* If a working storage control module is available, it should be        */
/* attached to the FatFs via a glue function rather than modifying it.   */
/* This is an example of glue functions to attach various exsisting      */
/* storage control modules to the FatFs module with a defined API.       */
/*-----------------------------------------------------------------------*/

#include <stdint.h>
#include <stdbool.h>
#include "sys_common.h"
#include "gio.h"
#include "spi.h"
#include "diskio.h"
#include <assert.h>

#define _READONLY   (0)

/* Definitions for MMC/SDC command */
#define CMD0    (0x40+0)    /* GO_IDLE_STATE */
#define CMD1    (0x40+1)    /* SEND_OP_COND */
#define CMD8    (0x40+8)    /* SEND_IF_COND */
#define CMD9    (0x40+9)    /* SEND_CSD */
#define CMD10    (0x40+10)    /* SEND_CID */
#define CMD12    (0x40+12)    /* STOP_TRANSMISSION */
#define CMD16    (0x40+16)    /* SET_BLOCKLEN */
#define CMD17    (0x40+17)    /* READ_SINGLE_BLOCK */
#define CMD18    (0x40+18)    /* READ_MULTIPLE_BLOCK */
#define CMD23    (0x40+23)    /* SET_BLOCK_COUNT */
#define CMD24    (0x40+24)    /* WRITE_BLOCK */
#define CMD25    (0x40+25)    /* WRITE_MULTIPLE_BLOCK */
#define CMD41    (0x40+41)    /* SEND_OP_COND (ACMD) */
#define CMD55    (0x40+55)    /* APP_CMD */
#define CMD58    (0x40+58)    /* READ_OCR */

gioPORT_t *_spiPORT = 0;
mibspiBASE_t *_spiREG = 0;
uint32_t _tf_group = 0;

static void DESELECT (void);
static void SELECT (void);
static unsigned char SPI_send (unsigned char outb);
static void xmit_spi(BYTE dat);
static BYTE rcvr_spi (void);
static void rcvr_spi_m (BYTE *dst);
static BYTE wait_ready (void);
static void send_initial_clock_train(void);
static void power_on (void);
static void set_max_speed(void);
static void power_off (void);
static int chk_power(void);
static bool rcvr_datablock (BYTE buff[], UINT btr);
static bool xmit_datablock (const BYTE *buff, BYTE token);
static BYTE send_cmd (BYTE cmd, DWORD arg);
static BYTE send_cmd12 (void);
void disk_timerproc (void);

void mmcSelectSpi(gioPORT_t *port, mibspiBASE_t *reg, uint32_t tf_group) {
    _spiPORT = port;
    _spiREG = reg;
    _tf_group = tf_group;
}

/* asserts the CS pin to the card */
static
void DESELECT (void)
{
    _spiPORT->DSET = 0x01U;        /* SCS[0] = high */
}

/* de-asserts the CS pin to the card */
static
void SELECT (void)
{
    assert(_spiPORT); /* call mmcSelectSpi(gioPORT_t *port, spiBASE_t *reg) first */
    assert(_spiREG); /* call mmcSelectSpi(gioPORT_t *port, spiBASE_t *reg) first */
    _spiPORT-> DCLR = 0x01U;       /* SCS[0] = low */
}

/*------------------------------------------------------------------------------
  Write and Read a byte on SPI interface
------------------------------------------------------------------------------*/
static unsigned char SPI_send (unsigned char outb) {

    uint16_t data = outb;

    while ((_spiREG->FLG & (uint32_t)0x0200) == 0U){ /* Wait until TXINTFLG is set for previous transmission */
    }

    mibspiSetData(_spiREG, _tf_group, &data);
    mibspiTransfer(_spiREG,_tf_group);

    while(!(mibspiIsTransferComplete(_spiREG, _tf_group))){

    }

    /*while ((_spiREG->FLG & 0x0100) == 0);  Wait until RXINTFLG is set when new value is received */
    return((unsigned char)_spiREG->BUF);  /* Return received value */
}
/*--------------------------------------------------------------------------
  Module Private Functions
---------------------------------------------------------------------------*/

static volatile
DSTATUS Stat = STA_NOINIT;              /* Disk status */


static volatile
BYTE Timer1, Timer2;                    /* 100Hz decrement timer */

static
BYTE CardType;                          /* b0:MMC, b1:SDC, b2:Block addressing */


static
BYTE PowerFlag = 0;                     /* indicates if "power" is on */

/*-----------------------------------------------------------------------*/
/* Transmit a byte to MMC via SPI  (Platform dependent)                  */
/*-----------------------------------------------------------------------*/

static
void xmit_spi(BYTE dat)
{
    uint32_t ui32RcvDat;
    uint16_t data = 0;

    data = dat; /* | 0x100D0000; */

    while ((_spiREG->FLG & (uint32_t)0x0200) == 0U){           /* Wait until TXINTFLG is set for previous transmission */

    }

    mibspiSetData(_spiREG, _tf_group, &data);
    mibspiTransfer(_spiREG,_tf_group);

    while(!(mibspiIsTransferComplete(_spiREG, _tf_group))){

    }

    /*_spiREG->DAT1 = dat | 0x100D0000;             * transmit register address */

    /* while ((_spiREG->FLG & 0x0100) == 0); * Wait until RXINTFLG is set when new value is received */
    ui32RcvDat = _spiREG->BUF;                      /* to get received value */
}

/*
 * Receive a byte from MMC via SPI  (Platform dependent
 */
static
BYTE rcvr_spi (void)
{
    uint16_t rx_data=0;
    uint16_t tx_data = 0xffff;

    while ((mibspiREG5->FLG & (uint32_t)0x0200) == 0U){                /* Wait until TXINTFLG is set for previous transmission */

    }

    mibspiSetData(_spiREG, _tf_group, &tx_data);
    mibspiTransfer(_spiREG,_tf_group);
    /*mibspiREG5->DAT1 = 0xFF | 0x100D0000;    * transmit register address */

    while(!(mibspiIsTransferComplete(_spiREG, _tf_group))){

    }

    mibspiGetData(_spiREG,_tf_group,&rx_data);

    /*while ((mibspiREG5->FLG & 0x0100) == 0);              * Wait until RXINTFLG is set when new value is received */
    /*return((unsigned char)mibspiREG5->BUF);               * Return received value */
    return((unsigned char)rx_data);                         /* Return received value */
}

static
void rcvr_spi_m (BYTE *dst)
{
    *dst = rcvr_spi();
}

/*-----------------------------------------------------------------------*/
/* Wait for card ready                                                  */
/*-----------------------------------------------------------------------*/

static
BYTE wait_ready (void)
{
    BYTE res;
    Timer2 = 50U;                /* Wait for ready in timeout of 500ms */
    rcvr_spi();
    do{
        res = rcvr_spi();
    }while ((res != 0xFFU) && Timer2);

    return res;
}

/*-----------------------------------------------------------------------*/
/* Send 80 or so clock transitions with CS and DI held high. This is     */
/* required after card power up to get it into SPI mode                  */
/*-----------------------------------------------------------------------*/
static
void send_initial_clock_train(void)
{
    uint32_t i;

    /* Ensure CS is held high. */
    DESELECT();

    /* Send 10 bytes over the SSI. This causes the clock to wiggle the */
    /* required number of times. */
    for(i = 0U ; i < 10U ; i++)
    {
        /* Write DUMMY data */
        /* FIFO. */
        SPI_send (0xFFU);
    }
}

/*-----------------------------------------------------------------------*/
/* Power Control  (Platform dependent)                                  */
/*-----------------------------------------------------------------------*/
/* When the target system does not support socket power control, there  */
/* is nothing to do in these functions and chk_power always returns 1.  */

static
void power_on (void)
{
    /*
     * This doesn't really turn the power on, but initializes the
     * SPI port and pins needed to talk to the card.
     */
    /*mibspiInit(); */

    /* Set DI and CS high and apply more than 74 pulses to SCLK for the card */
    /* to be able to accept a native command. */
    send_initial_clock_train();

    PowerFlag = 1U;
}

/* set the SPI speed to the max setting */
static
void set_max_speed(void)
{
    /* todo jc 20151004 - check if this is portable between hercules controllers/clock speeds */
    mibspiREG5->FMT0 &= 0xFFFF00FFU;                 /* mask out baudrate prescaler */
    /* Max. 5 MBit used for Data Transfer. */
    mibspiREG5->FMT0 |= (uint32_t)5U << 8U;                     /* baudrate prescale 10MHz / (1+1) = 5MBit */
}

static
void power_off (void)
{
    PowerFlag = 0U;
}

static
int32_t chk_power(void)                                 /* Socket power state: 0=off, 1=on */
{
    return (int32_t)PowerFlag;
}

/*-----------------------------------------------------------------------*/
/* Receive a data packet from MMC                                        */
/*-----------------------------------------------------------------------*/

static
bool rcvr_datablock (
        BYTE buff[],                                 /* Data buffer to store received data */
        UINT btr                                    /* Byte count (must be even number) */
)
{
    bool ret = TRUE;
    BYTE token;
    uint32_t i = 0U;

    Timer1 = 100U;
    do {                                            /* Wait for data packet in timeout of 100ms */
        token = rcvr_spi();
    } while ((token == 0xFFU) && Timer1);

    if(token != 0xFEU){
        ret = (bool)FALSE;                 /* If not valid data token, retutn with error */
    }
    else{
        do {                                            /* Receive the data block into buffer */
            rcvr_spi_m(&buff[i++]);
            rcvr_spi_m(&buff[i++]);
            btr -= 2U;
        } while (btr != 0U);
        rcvr_spi();                                     /* Discard CRC */
        rcvr_spi();
    }

    return ret;                                    /* Return with success */
}

/*-----------------------------------------------------------------------*/
/* Send a data packet to MMC                                            */
/*-----------------------------------------------------------------------*/

#if _READONLY == 0

static
bool xmit_datablock (
        const BYTE buff[],                       /* 512 byte data block to be transmitted */
        BYTE token                              /* Data/Stop token */
)
{
    bool ret = (bool)TRUE;
    BYTE resp, wc;
    uint32_t i = 0U;

    if (wait_ready() != 0xFFU){
        ret = (bool)FALSE;
    }
    else{
        xmit_spi(token);                            /* Xmit data token */

        if (token != 0xFDU)
        {                                           /* Is data token */
            wc = 0U;

            do {                                    /* Xmit the 512 byte data block to MMC */
                xmit_spi(buff[i++]);
                xmit_spi(buff[i++]);
            } while (--wc);

            xmit_spi(0xFFU);                         /* CRC (Dummy) */
            xmit_spi(0xFFU);
            resp = rcvr_spi();                      /* Reveive data response */

            if ((resp & 0x1FU) != 0x05U){              /* If not accepted, return with error */
                ret = (bool)FALSE;
            }
        }
    }

    return ret;
}

#endif /* _READONLY */

/*-----------------------------------------------------------------------*/
/* Send a command packet to MMC                                          */
/*-----------------------------------------------------------------------*/

static
BYTE send_cmd (
        BYTE cmd,                   /* Command byte */
        DWORD arg                   /* Argument */
)
{
    BYTE n, res;

    if (wait_ready() != 0xFFU){
        res = 0xFFU;
    }
    else{
        /* Send command packet */
       xmit_spi(cmd);                          /* Command */
       xmit_spi((BYTE)(arg >> 24));            /* Argument[31..24] */
       xmit_spi((BYTE)(arg >> 16));            /* Argument[23..16] */
       xmit_spi((BYTE)(arg >> 8));             /* Argument[15..8] */
       xmit_spi((BYTE)arg);                    /* Argument[7..0] */

       n = 0xffU;

       if (cmd == (BYTE)CMD0){
           n = 0x95U;              /* CRC for CMD0(0) */
       }

       if (cmd == (BYTE)CMD8){
           n = 0x87U;              /* CRC for CMD8(0x1AA) */
       }

       xmit_spi(n);

       /* Receive command response */
       if (cmd == (BYTE)CMD12){
           rcvr_spi();           /* Skip a stuff byte when stop reading */
       }

       n = 10U;                                 /* Wait for a valid response in timeout of 10 attempts */

       do{
           res = rcvr_spi();
           --n;
       }while ((res & 0x80U) && (n));
    }

    return res;                             /* Return with the response value */
}

/*-----------------------------------------------------------------------*
 * Send the special command used to terminate a multi-sector read.
 *
 * This is the only command which can be sent while the SDCard is sending
 * data. The SDCard spec indicates that the data transfer will stop 2 bytes
 * after the 6 byte CMD12 command is sent and that the card will then send
 * 0xFF for between 2 and 6 more bytes before the R1 response byte.  This
 * response will be followed by another 0xFF byte.  In testing, however, it
 * seems that some cards don't send the 2 to 6 0xFF bytes between the end of
 * data transmission and the response code.  This function, therefore, merely
 * reads 10 bytes and, if the last one read is 0xFF, returns the value of the
 * latest non-0xFF byte as the response code.
 *
 *-----------------------------------------------------------------------*/

static
BYTE send_cmd12 (void)
{
    BYTE n, res, val;


    /* For CMD12, we don't wait for the card to be idle before we send
     * the new command.
     */

    /* Send command packet - the argument for CMD12 is ignored. */
    xmit_spi((BYTE)CMD12);
    xmit_spi(0U);
    xmit_spi(0U);
    xmit_spi(0U);
    xmit_spi(0U);
    xmit_spi(0U);

    /* Read up to 10 bytes from the card, remembering the value read if it's
  not 0xFF */
    for(n = 0U; n < 10U; n++)
    {
        val = rcvr_spi();
        if(val != 0xFFU)
        {
            res = val;
        }
    }

    return res;            /* Return with the response value */
}

/*--------------------------------------------------------------------------
  Public Functions
---------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------*/
/* Initialize Disk Drive                                                */
/*-----------------------------------------------------------------------*/

DSTATUS disk_initialize (
        BYTE drv        /* Physical drive nmuber (0) */
)
{
    DSTATUS ret = (DSTATUS)0U;
    BYTE n, ty, ocr[4];

    if (drv){
        ret = (DSTATUS)STA_NOINIT;            /* Supports only single drive */
    }
    else if (Stat & (DSTATUS)STA_NODISK){
        ret = Stat;    /* No card in the socket */
    }
    else{
        power_on();                            /* Force socket power on */
        send_initial_clock_train();            /* Ensure the card is in SPI mode */


        SELECT();                /* CS = L */
        ty = 0U;
        if (send_cmd((BYTE)CMD0, (DWORD)0) == 1U) {            /* Enter Idle state */
            Timer1 = 100U;                        /* Initialization timeout of 1000 msec */
            if (send_cmd((BYTE)CMD8, 0x1AAU) == 1U) {    /* SDC Ver2+ */
                for (n = 0U; n < 4U; n++){
                    ocr[n] = rcvr_spi();
                }

                if ((ocr[2] == 0x01U) && (ocr[3] == 0xAAU)) {    /* The card can work at vdd range of 2.7-3.6V */
                    do {

                        if ((send_cmd((BYTE)CMD55, 0U) <= 1U)){
                            if((send_cmd((BYTE)CMD41, ((DWORD)1UL << 30)) == 0U)){
                                break;    /* ACMD41 with HCS bit */
                            }
                        }

                    } while (Timer1);

                    if (Timer1){
                        if(send_cmd((BYTE)CMD58, 0U) == 0U){    /* Check CCS bit */

                            for (n = 0U; n < 4U; n++){
                                ocr[n] = rcvr_spi();
                            }

                            ty = (ocr[0] & 0x40U) ? 6U : 2U;
                        }
                    }
                }
            }
            else {                            /* SDC Ver1 or MMC */

                if(send_cmd((BYTE)CMD55, 0U) <= 1U){    /* SDC : MMC */

                    if(send_cmd((BYTE)CMD41, 0U) <= 1U){
                        ty = 2U;
                    }
                    else{
                        ty = 1U;
                    }
                }
                else{
                    ty = 1U;
                }

                do {
                    if (ty == 2U) {
                        if (send_cmd((BYTE)CMD55, 0U) <= 1U){
                            if(send_cmd((BYTE)CMD41, 0U) == 0U){
                                break;    /* ACMD41 */
                            }
                        }
                    } else {
                        if (send_cmd((BYTE)CMD1, 0U) == 0U){
                            break;                                /* CMD1 */
                        }
                    }
                } while (Timer1);

                /* Select R/W block length */
                if(Timer1 == 0U){
                    ty = 0U;
                }
                else if(send_cmd((BYTE)CMD16, 512U) != 0U){
                    ty = 0U;
                }
                else{

                }

/*
                if ((!Timer1) || (send_cmd((BYTE)CMD16, 512U) != 0U)){
                    ty = 0U;
                }
                */
            }
        }
        CardType = ty;
        DESELECT();            /* CS = H */
        rcvr_spi();            /* Idle (Release DO) */


        if (ty) {            /* Initialization succeded */
            Stat &= (DSTATUS)~STA_NOINIT;        /* Clear STA_NOINIT */
            set_max_speed();
        } else {            /* Initialization failed */
            power_off();
        }

       ret = Stat;
    }

    return ret;
}

/*-----------------------------------------------------------------------*/
/* Get Disk Status                                                      */
/*-----------------------------------------------------------------------*/

DSTATUS disk_status (
        BYTE drv        /* Physical drive nmuber (0) */
)
{
    DSTATUS ret;

    if (drv){
        ret = STA_NOINIT;        /* Supports only single drive */
    }
    else{
        ret = Stat;
    }

    return ret;
}


/*-----------------------------------------------------------------------*/
/* Read Sector(s)                                                        */
/*-----------------------------------------------------------------------*/

DRESULT disk_read (
        BYTE drv,            /* Physical drive nmuber (0) */
        BYTE *buff,            /* Pointer to the data buffer to store read data */
        DWORD sector,        /* Start sector number (LBA) */
        UINT count            /* Sector count (1..255) */
)
{
    DRESULT ret;

    if ((drv) || (!count)){
        ret = RES_PARERR;
    }
    else if (Stat & STA_NOINIT){
        ret = RES_NOTRDY;
    }
    else{
        if (!(CardType & 4U)){
            sector *= 512U;    /* Convert to byte address if needed */
        }

        SELECT();            /* CS = L */


        if (count == 1U) {    /* Single block read */
            if ((send_cmd((BYTE)CMD17, sector) == 0U)){ /* READ_SINGLE_BLOCK */

                if(rcvr_datablock(buff, 512U)){
                    count = 0U;
                }
            }
        }
        else {                /* Multiple block read */
            if (send_cmd((BYTE)CMD18, sector) == 0U) {    /* READ_MULTIPLE_BLOCK */
                do {
                    if (!rcvr_datablock(buff, 512U)){
                        break;
                    }
                    buff += 512U;
                } while (--count);
                send_cmd12();                /* STOP_TRANSMISSION */
            }
        }


        DESELECT();            /* CS = H */
        rcvr_spi();            /* Idle (Release DO) */

        ret = (DRESULT)(count ? RES_ERROR : RES_OK);
    }

    return ret;
}


/*-----------------------------------------------------------------------*/
/* Write Sector(s)                                                      */
/*-----------------------------------------------------------------------*/


#if _READONLY == 0
DRESULT disk_write (
        BYTE drv,            /* Physical drive nmuber (0) */
        const BYTE *buff,    /* Pointer to the data to be written */
        DWORD sector,        /* Start sector number (LBA) */
        UINT count            /* Sector count (1..255) */
)
{
    DRESULT ret;

    if (drv || (!count)){
        ret = RES_PARERR;
    }
    else if (Stat & STA_NOINIT){
        ret = RES_NOTRDY;
    }
    else if (Stat & STA_PROTECT){
        ret = RES_WRPRT;
    }
    else{
        if (!(CardType & 4U)){
            sector *= 512U;    /* Convert to byte address if needed */
        }

        SELECT();            /* CS = L */


        if (count == 1U) {    /* Single block write */

            if (send_cmd((BYTE)CMD24, sector) == 0U){    /* WRITE_BLOCK */

                if(xmit_datablock(buff, 0xFEU)){
                    count = 0U;
                }
            }
        }
        else {                /* Multiple block write */
            if (CardType & 2U) {
                send_cmd((BYTE)CMD55, 0U);
                send_cmd((BYTE)CMD23, count);    /* ACMD23 */
            }

            if (send_cmd((BYTE)CMD25, sector) == 0U) {    /* WRITE_MULTIPLE_BLOCK */
                do {
                    if (!xmit_datablock(buff, 0xFCU)) {
                        break;
                    }
                    buff += 512U;
                } while (--count);
                if (!xmit_datablock((BYTE*)0, 0xFDU)){    /* STOP_TRAN token */
                    count = 1U;
                }
            }
        }

        DESELECT();            /* CS = H */
        rcvr_spi();            /* Idle (Release DO) */

        ret = (DRESULT)(count ? RES_ERROR : RES_OK);
    }

    return ret;
}
#endif /* _READONLY */

/*-----------------------------------------------------------------------*/
/* Miscellaneous Functions                                              */
/*-----------------------------------------------------------------------*/


DRESULT disk_ioctl (
        BYTE drv,                               /* Physical drive nmuber (0) */
        BYTE ctrl,                              /* Control code */
        BYTE buff[]                              /* Buffer to send/receive control data */
)
{
    DRESULT res;
    BYTE n, csd[16] = {0}, *ptr = buff;
    WORD csize;

    if (drv){
        res = RES_PARERR;
    }
    else{
        res = RES_ERROR;

        if (ctrl == (BYTE)CTRL_POWER) {
            switch (*ptr) {
            case 0U:                             /* Sub control code == 0 (POWER_OFF) */
                if (chk_power()){
                    power_off();                /* Power off */
                }

                res = RES_OK;
                break;
            case 1U:                             /* Sub control code == 1 (POWER_ON) */
                power_on();                     /* Power on */
                res = RES_OK;
                break;
            case 2U:                                     /* Sub control code == 2 (POWER_GET) */
                buff[1] = (BYTE)chk_power();
                res = RES_OK;
                break;
            default :
                res = RES_PARERR;
                break;
            }
        }
        else {
            if (Stat & STA_NOINIT){
                res = RES_NOTRDY;
            }
            else{
                SELECT();                                   /* CS = L */

                switch (ctrl) {
                case GET_SECTOR_COUNT :                     /* Get number of sectors on the disk (DWORD) */
                    if (send_cmd((BYTE)CMD9, 0U) == 0U){
                        if(rcvr_datablock(csd, 16U)) {
                            if ((csd[0] >> 6) == 1U) {    /* SDC ver 2.00 */
                                csize = (WORD)csd[9] + (WORD)((WORD)csd[8] << 8) + 1U;
                                *(DWORD*)buff = (DWORD)csize << 10;
                            }
                            else {                    /* MMC or SDC ver 1.XX */
                                n = (BYTE)(csd[5] & 15U);
                                n = n + ((BYTE)(csd[10] & 128U) >> 7);
                                n = n + (BYTE)((csd[9] & 3U) << 1) + 2U;

                                csize = (WORD)((WORD)csd[8] >> 6);
                                csize = csize + (WORD)((WORD)csd[7] << 2);
                                csize = csize + (WORD)((WORD)((WORD)csd[6] & 3U) << 10) + 1U;

                                *(DWORD*)buff = (DWORD)((DWORD)csize << (n - 9U));
                            }
                            res = RES_OK;
                        }
                    }
                    break;


                case GET_SECTOR_SIZE :                  /* Get sectors on the disk (WORD) */
                    *(WORD*)buff = 512U;
                    res = RES_OK;
                    break;


                case CTRL_SYNC :                        /* Make sure that data has been written */
                    if (wait_ready() == 0xFFU){
                        res = RES_OK;
                    }
                    break;


                case MMC_GET_CSD :                  /* Receive CSD as a data block (16 bytes) */
                    if (send_cmd((BYTE)CMD9, 0U) == 0U){    /* READ_CSD */
                        if(rcvr_datablock(ptr, 16U)){
                            res = RES_OK;
                        }
                    }
                    break;


                case MMC_GET_CID :                  /* Receive CID as a data block (16 bytes) */
                    if (send_cmd((BYTE)CMD10, 0U) == 0U){        /* READ_CID */
                        if(rcvr_datablock(ptr, 16U)){
                            res = RES_OK;
                        }
                    }
                    break;


                case MMC_GET_OCR :                  /* Receive OCR as an R3 resp (4 bytes) */
                    if (send_cmd((BYTE)CMD58, 0U) == 0U) {    /* READ_OCR */
                        for (n = 0U; n < 4U; n++){
                            buff[n] = rcvr_spi();
                        }
                        res = RES_OK;
                    }
                    else{
                        res = RES_PARERR;
                    }
                    break;
                default:
                    res = RES_PARERR;
                    break;
                }

                DESELECT();                         /* CS = H */
                rcvr_spi();                         /* Idle (Release DO) */
            }


        }
    }
    return res;
}


/*-----------------------------------------------------------------------*/
/* Device Timer Interrupt Procedure  (Platform dependent)                */
/*-----------------------------------------------------------------------*/
/* This function must be called in period of 10ms                        */

void disk_timerproc (void)
{
    /*    BYTE n, s; */
    BYTE n;

    n = Timer1;                        /* 100Hz decrement timer */
    if (n){
        Timer1 = --n;
    }
    n = Timer2;
    if (n){
        Timer2 = --n;
    }

}

