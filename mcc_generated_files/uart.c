/**
  CamUART Generated Driver File 

  @Company
    Microchip Technology Inc.

  @File Name
    uart1.c

  @Summary
    This is the generated source file for the CamUART driver using PIC24 / dsPIC33 / PIC32MM MCUs

  @Description
    This source file provides APIs for driver for CamUART. 
    Generation Information : 
        Product Revision  :  PIC24 / dsPIC33 / PIC32MM MCUs - 1.170.0
        Device            :  PIC24FJ128GB204
    The generated drivers are tested against the following:
        Compiler          :  XC16 v1.61
        MPLAB             :  MPLAB X v5.45
*/

/*
    (c) 2020 Microchip Technology Inc. and its subsidiaries. You may use this
    software and any derivatives exclusively with Microchip products.

    THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER
    EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED
    WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A
    PARTICULAR PURPOSE, OR ITS INTERACTION WITH MICROCHIP PRODUCTS, COMBINATION
    WITH ANY OTHER PRODUCTS, OR USE IN ANY APPLICATION.

    IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE,
    INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND
    WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS
    BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO THE
    FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN
    ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF ANY,
    THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.

    MICROCHIP PROVIDES THIS SOFTWARE CONDITIONALLY UPON YOUR ACCEPTANCE OF THESE
    TERMS.
*/

/**
  Section: Included Files
*/
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include "xc.h"
#include "uart.h"
/**
  Section: Data Type Definitions
*/

/** UART Driver Queue Status

  @Summary
    Defines the object required for the status of the queue.
*/

static uint8_t * volatile rxTail;
static uint8_t *rxHead;
static uint8_t *txTail;
static uint8_t * volatile txHead;
static bool volatile rxOverflowed;

/** UART Driver Queue Length

  @Summary
    Defines the length of the Transmit and Receive Buffers

*/

/* We add one extra byte than requested so that we don't have to have a separate
 * bit to determine the difference between buffer full and buffer empty, but
 * still be able to hold the amount of data requested by the user.  Empty is
 * when head == tail.  So full will result in head/tail being off by one due to
 * the extra byte.
 */
#define CamUART_CONFIG_TX_BYTEQ_LENGTH (8+1)
#define CamUART_CONFIG_RX_BYTEQ_LENGTH (8+1)

/** UART Driver Queue

  @Summary
    Defines the Transmit and Receive Buffers

*/

static uint8_t txQueue[CamUART_CONFIG_TX_BYTEQ_LENGTH];
static uint8_t rxQueue[CamUART_CONFIG_RX_BYTEQ_LENGTH];

void (*CamUART_TxDefaultInterruptHandler)(void);
void (*CamUART_RxDefaultInterruptHandler)(void);

/**
  Section: Driver Interface
*/

void CamUART_Initialize(void)
{
    IEC0bits.U1TXIE = 0;
    IEC0bits.U1RXIE = 0;

    // STSEL 1; IREN disabled; PDSEL 8N; UARTEN enabled; RTSMD disabled; USIDL disabled; WAKE disabled; ABAUD disabled; LPBACK disabled; BRGH enabled; URXINV disabled; UEN TX_RX; 
    // Data Bits = 8; Parity = None; Stop Bits = 1;
    U1MODE = (0x8008 & ~(1<<15));  // disabling UART ON bit
    // UTXISEL0 TX_ONE_CHAR; UTXINV disabled; URXEN disabled; OERR NO_ERROR_cleared; URXISEL RX_ONE_CHAR; UTXBRK COMPLETED; UTXEN disabled; ADDEN disabled; 
    U1STA = 0x00;
    // BaudRate = 115200; Frequency = 12000000 Hz; U1BRG 25; 
    U1BRG = 0x19;
    // ADMADDR 0; ADMMASK 0; 
    U1ADMD = 0x00;
    // T0PD 1 ETU; PTRCL T0; TXRPT Retransmits the error byte once; CONV Direct; SCEN disabled; 
    U1SCCON = 0x00;
    // TXRPTIF disabled; TXRPTIE disabled; WTCIF disabled; WTCIE disabled; PARIE disabled; GTCIF disabled; GTCIE disabled; RXRPTIE disabled; RXRPTIF disabled; 
    U1SCINT = 0x00;
    // GTC 0; 
    U1GTC = 0x00;
    // WTCL 0; 
    U1WTCL = 0x00;
    // WTCH 0; 
    U1WTCH = 0x00;
    
    txHead = txQueue;
    txTail = txQueue;
    rxHead = rxQueue;
    rxTail = rxQueue;
   
    rxOverflowed = false;

    CamUART_SetTxInterruptHandler(&CamUART_Transmit_CallBack);

    CamUART_SetRxInterruptHandler(&CamUART_Receive_CallBack);

    IEC0bits.U1RXIE = 1;
    
    //Make sure to set LAT bit corresponding to TxPin as high before UART initialization
    U1MODEbits.UARTEN = 1;   // enabling UART ON bit
    U1STAbits.UTXEN = 1;
}

/**
    Maintains the driver's transmitter state machine and implements its ISR
*/

void CamUART_SetTxInterruptHandler(void (* interruptHandler)(void))
{
    if(interruptHandler == NULL)
    {
        CamUART_TxDefaultInterruptHandler = &CamUART_Transmit_CallBack;
    }
    else
    {
        CamUART_TxDefaultInterruptHandler = interruptHandler;
    }
} 

void __attribute__ ( ( interrupt, no_auto_psv ) ) _U1TXInterrupt ( void )
{
    (*CamUART_TxDefaultInterruptHandler)();

    if(txHead == txTail)
    {
        IEC0bits.U1TXIE = 0;
    }
    else
    {
        IFS0bits.U1TXIF = 0;

        while(!(U1STAbits.UTXBF == 1))
        {
            U1TXREG = *txHead++;

            if(txHead == (txQueue + CamUART_CONFIG_TX_BYTEQ_LENGTH))
            {
                txHead = txQueue;
            }

            // Are we empty?
            if(txHead == txTail)
            {
                break;
            }
        }
    }
}

void __attribute__ ((weak)) CamUART_Transmit_CallBack ( void )
{ 

}

void CamUART_SetRxInterruptHandler(void (* interruptHandler)(void))
{
    if(interruptHandler == NULL)
    {
        CamUART_RxDefaultInterruptHandler = &CamUART_Receive_CallBack;
    }
    else
    {
        CamUART_RxDefaultInterruptHandler = interruptHandler;
    }
}

void __attribute__ ( ( interrupt, no_auto_psv ) ) _U1RXInterrupt( void )
{
    (*CamUART_RxDefaultInterruptHandler)();

    IFS0bits.U1RXIF = 0;
	
    while((U1STAbits.URXDA == 1))
    {
        *rxTail = U1RXREG;

        // Will the increment not result in a wrap and not result in a pure collision?
        // This is most often condition so check first
        if ( ( rxTail    != (rxQueue + CamUART_CONFIG_RX_BYTEQ_LENGTH-1)) &&
             ((rxTail+1) != rxHead) )
        {
            rxTail++;
        } 
        else if ( (rxTail == (rxQueue + CamUART_CONFIG_RX_BYTEQ_LENGTH-1)) &&
                  (rxHead !=  rxQueue) )
        {
            // Pure wrap no collision
            rxTail = rxQueue;
        } 
        else // must be collision
        {
            rxOverflowed = true;
        }
    }
}

void __attribute__ ((weak)) CamUART_Receive_CallBack(void)
{
//    if (CamUART_IsRxReady() == 1) { 
//        rxChar = CamUART_Read();
//        static int i = 0;
//
//        rxBuffer[i] = rxChar;
//        i++;
//        if (rxChar == '\n') {
//            new_command = true;
//            i = 0;
//        }
//    }
}

void __attribute__ ( ( interrupt, no_auto_psv ) ) _U1ErrInterrupt( void )
{
    if ((U1STAbits.OERR == 1))
    {
        U1STAbits.OERR = 0;
    }
    
    IFS4bits.U1ERIF = 0;
}

/**
  Section: UART Driver Client Routines
*/

uint8_t CamUART_Read( void)
{
    uint8_t data = 0;

    while (rxHead == rxTail )
    {
    }
    
    data = *rxHead;

    rxHead++;

    if (rxHead == (rxQueue + CamUART_CONFIG_RX_BYTEQ_LENGTH))
    {
        rxHead = rxQueue;
    }
    return data;
}

void CamUART_Write( uint8_t byte)
{
    while(CamUART_IsTxReady() == 0)
    {
    }

    *txTail = byte;

    txTail++;
    
    if (txTail == (txQueue + CamUART_CONFIG_TX_BYTEQ_LENGTH))
    {
        txTail = txQueue;
    }

    IEC0bits.U1TXIE = 1;
}

bool CamUART_IsRxReady(void)
{    
    return !(rxHead == rxTail);
}

bool CamUART_IsTxReady(void)
{
    uint16_t size;
    uint8_t *snapshot_txHead = (uint8_t*)txHead;
    
    if (txTail < snapshot_txHead)
    {
        size = (snapshot_txHead - txTail - 1);
    }
    else
    {
        size = ( CamUART_CONFIG_TX_BYTEQ_LENGTH - (txTail - snapshot_txHead) - 1 );
    }
    
    return (size != 0);
}

bool CamUART_IsTxDone(void)
{
    if(txTail == txHead)
    {
        return (bool)U1STAbits.TRMT;
    }
    
    return false;
}


/*******************************************************************************

  !!! Deprecated API !!!
  !!! These functions will not be supported in future releases !!!

*******************************************************************************/

static uint8_t CamUART_RxDataAvailable(void)
{
    uint16_t size;
    uint8_t *snapshot_rxTail = (uint8_t*)rxTail;
    
    if (snapshot_rxTail < rxHead) 
    {
        size = ( CamUART_CONFIG_RX_BYTEQ_LENGTH - (rxHead-snapshot_rxTail));
    }
    else
    {
        size = ( (snapshot_rxTail - rxHead));
    }
    
    if(size > 0xFF)
    {
        return 0xFF;
    }
    
    return size;
}

static uint8_t CamUART_TxDataAvailable(void)
{
    uint16_t size;
    uint8_t *snapshot_txHead = (uint8_t*)txHead;
    
    if (txTail < snapshot_txHead)
    {
        size = (snapshot_txHead - txTail - 1);
    }
    else
    {
        size = ( CamUART_CONFIG_TX_BYTEQ_LENGTH - (txTail - snapshot_txHead) - 1 );
    }
    
    if(size > 0xFF)
    {
        return 0xFF;
    }
    
    return size;
}

unsigned int __attribute__((deprecated)) CamUART_ReadBuffer( uint8_t *buffer ,  unsigned int numbytes)
{
    unsigned int rx_count = CamUART_RxDataAvailable();
    unsigned int i;
    
    if(numbytes < rx_count)
    {
        rx_count = numbytes;
    }
    
    for(i=0; i<rx_count; i++)
    {
        *buffer++ = CamUART_Read();
    }
    
    return rx_count;    
}

unsigned int __attribute__((deprecated)) CamUART_WriteBuffer( uint8_t *buffer , unsigned int numbytes )
{
    unsigned int tx_count = CamUART_TxDataAvailable();
    unsigned int i;
    
    if(numbytes < tx_count)
    {
        tx_count = numbytes;
    }
    
    for(i=0; i<tx_count; i++)
    {
        CamUART_Write(*buffer++);
    }
    
    return tx_count;  
}

CamUART_TRANSFER_STATUS __attribute__((deprecated)) CamUART_TransferStatusGet (void )
{
    CamUART_TRANSFER_STATUS status = 0;
    uint8_t rx_count = CamUART_RxDataAvailable();
    uint8_t tx_count = CamUART_TxDataAvailable();
    
    switch(rx_count)
    {
        case 0:
            status |= CamUART_TRANSFER_STATUS_RX_EMPTY;
            break;
        case CamUART_CONFIG_RX_BYTEQ_LENGTH:
            status |= CamUART_TRANSFER_STATUS_RX_FULL;
            break;
        default:
            status |= CamUART_TRANSFER_STATUS_RX_DATA_PRESENT;
            break;
    }
    
    switch(tx_count)
    {
        case 0:
            status |= CamUART_TRANSFER_STATUS_TX_FULL;
            break;
        case CamUART_CONFIG_RX_BYTEQ_LENGTH:
            status |= CamUART_TRANSFER_STATUS_TX_EMPTY;
            break;
        default:
            break;
    }

    return status;    
}

uint8_t __attribute__((deprecated)) CamUART_Peek(uint16_t offset)
{
    uint8_t *peek = rxHead + offset;
    
    while(peek > (rxQueue + CamUART_CONFIG_RX_BYTEQ_LENGTH))
    {
        peek -= CamUART_CONFIG_RX_BYTEQ_LENGTH;
    }
    
    return *peek;
}

bool __attribute__((deprecated)) CamUART_ReceiveBufferIsEmpty (void)
{
    return (CamUART_RxDataAvailable() == 0);
}

bool __attribute__((deprecated)) CamUART_TransmitBufferIsFull(void)
{
    return (CamUART_TxDataAvailable() == 0);
}

uint16_t __attribute__((deprecated)) CamUART_StatusGet (void)
{
    return U1STA;
}

unsigned int __attribute__((deprecated)) CamUART_TransmitBufferSizeGet(void)
{
    if(CamUART_TxDataAvailable() != 0)
    { 
        if(txHead > txTail)
        {
            return((txHead - txTail) - 1);
        }
        else
        {
            return((CamUART_CONFIG_TX_BYTEQ_LENGTH - (txTail - txHead)) - 1);
        }
    }
    return 0;
}

unsigned int __attribute__((deprecated)) CamUART_ReceiveBufferSizeGet(void)
{
    if(CamUART_RxDataAvailable() != 0)
    {
        if(rxHead > rxTail)
        {
            return((rxHead - rxTail) - 1);
        }
        else
        {
            return((CamUART_CONFIG_RX_BYTEQ_LENGTH - (rxTail - rxHead)) - 1);
        } 
    }
    return 0;
}

void __attribute__((deprecated)) CamUART_Enable(void)
{
    U1MODEbits.UARTEN = 1;
    U1STAbits.UTXEN = 1;
}

void __attribute__((deprecated)) CamUART_Disable(void)
{
    U1MODEbits.UARTEN = 0;
    U1STAbits.UTXEN = 0;
}


//Ring Light UART
//.....................................................................................................................................................................



//


#define RingLightUART_CONFIG_TX_BYTEQ_LENGTH (8+1)
#define RingLightUART_CONFIG_RX_BYTEQ_LENGTH (8+1)

static uint8_t txQueue[RingLightUART_CONFIG_TX_BYTEQ_LENGTH];
static uint8_t rxQueue[RingLightUART_CONFIG_RX_BYTEQ_LENGTH];

void (*RingLightUART_TxDefaultInterruptHandler)(void);
void (*RingLightUART_RxDefaultInterruptHandler)(void);

void RingLightUART_Initialize(void)
{
    IEC1bits.U2TXIE = 0;
    IEC1bits.U2RXIE = 0;

    // STSEL 1; IREN disabled; PDSEL 8N; UARTEN enabled; RTSMD disabled; USIDL disabled; WAKE disabled; ABAUD disabled; LPBACK disabled; BRGH enabled; URXINV disabled; UEN TX_RX; 
    // Data Bits = 8; Parity = None; Stop Bits = 1;
    U2MODE = (0x8008 & ~(1<<15));  // disabling UART ON bit
    // UTXISEL0 TX_ONE_CHAR; UTXINV disabled; URXEN disabled; OERR NO_ERROR_cleared; URXISEL RX_ONE_CHAR; UTXBRK COMPLETED; UTXEN disabled; ADDEN disabled; 
    U2STA = 0x00;
    // BaudRate = 115200; Frequency = 12000000 Hz; U2BRG 25; 
    U2BRG = 0x19;
    // ADMADDR 0; ADMMASK 0; 
    U2ADMD = 0x00;
    // T0PD 1 ETU; PTRCL T0; TXRPT Retransmits the error byte once; CONV Direct; SCEN disabled; 
    U2SCCON = 0x00;
    // TXRPTIF disabled; TXRPTIE disabled; WTCIF disabled; WTCIE disabled; PARIE disabled; GTCIF disabled; GTCIE disabled; RXRPTIE disabled; RXRPTIF disabled; 
    U2SCINT = 0x00;
    // GTC 0; 
    U2GTC = 0x00;
    // WTCL 0; 
    U2WTCL = 0x00;
    // WTCH 0; 
    U2WTCH = 0x00;
    
    txHead = txQueue;
    txTail = txQueue;
    rxHead = rxQueue;
    rxTail = rxQueue;
   
    rxOverflowed = false;

    RingLightUART_SetTxInterruptHandler(&RingLightUART_Transmit_CallBack);

    RingLightUART_SetRxInterruptHandler(&RingLightUART_Receive_CallBack);

    IEC1bits.U2RXIE = 1;
    
    //Make sure to set LAT bit corresponding to TxPin as high before UART initialization
    U2MODEbits.UARTEN = 1;   // enabling UART ON bit
    U2STAbits.UTXEN = 1;
}

/**
    Maintains the driver's transmitter state machine and implements its ISR
*/

void RingLightUART_SetTxInterruptHandler(void (* interruptHandler)(void))
{
    if(interruptHandler == NULL)
    {
        RingLightUART_TxDefaultInterruptHandler = &RingLightUART_Transmit_CallBack;
    }
    else
    {
        RingLightUART_TxDefaultInterruptHandler = interruptHandler;
    }
} 

void __attribute__ ( ( interrupt, no_auto_psv ) ) _U2TXInterrupt ( void )
{
    (*RingLightUART_TxDefaultInterruptHandler)();

    if(txHead == txTail)
    {
        IEC1bits.U2TXIE = 0;
    }
    else
    {
        IFS1bits.U2TXIF = 0;

        while(!(U2STAbits.UTXBF == 1))
        {
            U2TXREG = *txHead++;

            if(txHead == (txQueue + RingLightUART_CONFIG_TX_BYTEQ_LENGTH))
            {
                txHead = txQueue;
            }

            // Are we empty?
            if(txHead == txTail)
            {
                break;
            }
        }
    }
}

void __attribute__ ((weak)) RingLightUART_Transmit_CallBack ( void )
{ 

}

void RingLightUART_SetRxInterruptHandler(void (* interruptHandler)(void))
{
    if(interruptHandler == NULL)
    {
        RingLightUART_RxDefaultInterruptHandler = &RingLightUART_Receive_CallBack;
    }
    else
    {
        RingLightUART_RxDefaultInterruptHandler = interruptHandler;
    }
}

void __attribute__ ( ( interrupt, no_auto_psv ) ) _U2RXInterrupt( void )
{
    (*RingLightUART_RxDefaultInterruptHandler)();

    IFS1bits.U2RXIF = 0;
	
    while((U2STAbits.URXDA == 1))
    {
        *rxTail = U2RXREG;

        // Will the increment not result in a wrap and not result in a pure collision?
        // This is most often condition so check first
        if ( ( rxTail    != (rxQueue + RingLightUART_CONFIG_RX_BYTEQ_LENGTH-1)) &&
             ((rxTail+1) != rxHead) )
        {
            rxTail++;
        } 
        else if ( (rxTail == (rxQueue + RingLightUART_CONFIG_RX_BYTEQ_LENGTH-1)) &&
                  (rxHead !=  rxQueue) )
        {
            // Pure wrap no collision
            rxTail = rxQueue;
        } 
        else // must be collision
        {
            rxOverflowed = true;
        }
    }
}

void __attribute__ ((weak)) RingLightUART_Receive_CallBack(void)
{
//    if (RingLightUART_IsRxReady() == 1) { 
//        rxChar = RingLightUART_Read();
//        static int i = 0;
//
//        rxBuffer[i] = rxChar;
//        i++;
//        if (rxChar == '\n') {
//            new_command = true;
//            i = 0;
//        }
//    }
}

void __attribute__ ( ( interrupt, no_auto_psv ) ) _U2ErrInterrupt( void )
{
    if ((U2STAbits.OERR == 1))
    {
        U2STAbits.OERR = 0;
    }
    
    IFS4bits.U2ERIF = 0;
}

/**
  Section: UART Driver Client Routines
*/

uint8_t RingLightUART_Read( void)
{
    uint8_t data = 0;

    while (rxHead == rxTail )
    {
    }
    
    data = *rxHead;

    rxHead++;

    if (rxHead == (rxQueue + RingLightUART_CONFIG_RX_BYTEQ_LENGTH))
    {
        rxHead = rxQueue;
    }
    return data;
}

void RingLightUART_Write( uint8_t byte)
{
    while(RingLightUART_IsTxReady() == 0)
    {
    }

    *txTail = byte;

    txTail++;
    
    if (txTail == (txQueue + RingLightUART_CONFIG_TX_BYTEQ_LENGTH))
    {
        txTail = txQueue;
    }

    IEC1bits.U2TXIE = 1;
}

bool RingLightUART_IsRxReady(void)
{    
    return !(rxHead == rxTail);
}

bool RingLightUART_IsTxReady(void)
{
    uint16_t size;
    uint8_t *snapshot_txHead = (uint8_t*)txHead;
    
    if (txTail < snapshot_txHead)
    {
        size = (snapshot_txHead - txTail - 1);
    }
    else
    {
        size = ( RingLightUART_CONFIG_TX_BYTEQ_LENGTH - (txTail - snapshot_txHead) - 1 );
    }
    
    return (size != 0);
}

bool RingLightUART_IsTxDone(void)
{
    if(txTail == txHead)
    {
        return (bool)U2STAbits.TRMT;
    }
    
    return false;
}


/*******************************************************************************

  !!! Deprecated API !!!
  !!! These functions will not be supported in future releases !!!

*******************************************************************************/

static uint8_t RingLightUART_RxDataAvailable(void)
{
    uint16_t size;
    uint8_t *snapshot_rxTail = (uint8_t*)rxTail;
    
    if (snapshot_rxTail < rxHead) 
    {
        size = ( RingLightUART_CONFIG_RX_BYTEQ_LENGTH - (rxHead-snapshot_rxTail));
    }
    else
    {
        size = ( (snapshot_rxTail - rxHead));
    }
    
    if(size > 0xFF)
    {
        return 0xFF;
    }
    
    return size;
}

static uint8_t RingLightUART_TxDataAvailable(void)
{
    uint16_t size;
    uint8_t *snapshot_txHead = (uint8_t*)txHead;
    
    if (txTail < snapshot_txHead)
    {
        size = (snapshot_txHead - txTail - 1);
    }
    else
    {
        size = ( RingLightUART_CONFIG_TX_BYTEQ_LENGTH - (txTail - snapshot_txHead) - 1 );
    }
    
    if(size > 0xFF)
    {
        return 0xFF;
    }
    
    return size;
}

unsigned int __attribute__((deprecated)) RingLightUART_ReadBuffer( uint8_t *buffer ,  unsigned int numbytes)
{
    unsigned int rx_count = RingLightUART_RxDataAvailable();
    unsigned int i;
    
    if(numbytes < rx_count)
    {
        rx_count = numbytes;
    }
    
    for(i=0; i<rx_count; i++)
    {
        *buffer++ = RingLightUART_Read();
    }
    
    return rx_count;    
}

unsigned int __attribute__((deprecated)) RingLightUART_WriteBuffer( uint8_t *buffer , unsigned int numbytes )
{
    unsigned int tx_count = RingLightUART_TxDataAvailable();
    unsigned int i;
    
    if(numbytes < tx_count)
    {
        tx_count = numbytes;
    }
    
    for(i=0; i<tx_count; i++)
    {
        RingLightUART_Write(*buffer++);
    }
    
    return tx_count;  
}

RingLightUART_TRANSFER_STATUS __attribute__((deprecated)) RingLightUART_TransferStatusGet (void )
{
    RingLightUART_TRANSFER_STATUS status = 0;
    uint8_t rx_count = RingLightUART_RxDataAvailable();
    uint8_t tx_count = RingLightUART_TxDataAvailable();
    
    switch(rx_count)
    {
        case 0:
            status |= RingLightUART_TRANSFER_STATUS_RX_EMPTY;
            break;
        case RingLightUART_CONFIG_RX_BYTEQ_LENGTH:
            status |= RingLightUART_TRANSFER_STATUS_RX_FULL;
            break;
        default:
            status |= RingLightUART_TRANSFER_STATUS_RX_DATA_PRESENT;
            break;
    }
    
    switch(tx_count)
    {
        case 0:
            status |= RingLightUART_TRANSFER_STATUS_TX_FULL;
            break;
        case RingLightUART_CONFIG_RX_BYTEQ_LENGTH:
            status |= RingLightUART_TRANSFER_STATUS_TX_EMPTY;
            break;
        default:
            break;
    }

    return status;    
}

uint8_t __attribute__((deprecated)) RingLightUART_Peek(uint16_t offset)
{
    uint8_t *peek = rxHead + offset;
    
    while(peek > (rxQueue + RingLightUART_CONFIG_RX_BYTEQ_LENGTH))
    {
        peek -= RingLightUART_CONFIG_RX_BYTEQ_LENGTH;
    }
    
    return *peek;
}

bool __attribute__((deprecated)) RingLightUART_ReceiveBufferIsEmpty (void)
{
    return (RingLightUART_RxDataAvailable() == 0);
}

bool __attribute__((deprecated)) RingLightUART_TransmitBufferIsFull(void)
{
    return (RingLightUART_TxDataAvailable() == 0);
}

uint16_t __attribute__((deprecated)) RingLightUART_StatusGet (void)
{
    return U2STA;
}

unsigned int __attribute__((deprecated)) RingLightUART_TransmitBufferSizeGet(void)
{
    if(RingLightUART_TxDataAvailable() != 0)
    { 
        if(txHead > txTail)
        {
            return((txHead - txTail) - 1);
        }
        else
        {
            return((RingLightUART_CONFIG_TX_BYTEQ_LENGTH - (txTail - txHead)) - 1);
        }
    }
    return 0;
}

unsigned int __attribute__((deprecated)) RingLightUART_ReceiveBufferSizeGet(void)
{
    if(RingLightUART_RxDataAvailable() != 0)
    {
        if(rxHead > rxTail)
        {
            return((rxHead - rxTail) - 1);
        }
        else
        {
            return((RingLightUART_CONFIG_RX_BYTEQ_LENGTH - (rxTail - rxHead)) - 1);
        } 
    }
    return 0;
}

void __attribute__((deprecated)) RingLightUART_Enable(void)
{
    U2MODEbits.UARTEN = 1;
    U2STAbits.UTXEN = 1;
}

void __attribute__((deprecated)) RingLightUART_Disable(void)
{
    U2MODEbits.UARTEN = 0;
    U2STAbits.UTXEN = 0;
}


//Front Face UART
//.....................................................................................................................................................................



//


#define FrontFaceUART_CONFIG_TX_BYTEQ_LENGTH (8+1)
#define FrontFaceUART_CONFIG_RX_BYTEQ_LENGTH (8+1)

static uint8_t txQueue[FrontFaceUART_CONFIG_TX_BYTEQ_LENGTH];
static uint8_t rxQueue[FrontFaceUART_CONFIG_RX_BYTEQ_LENGTH];

void (*FrontFaceUART_TxDefaultInterruptHandler)(void);
void (*FrontFaceUART_RxDefaultInterruptHandler)(void);

void FrontFaceUART_Initialize(void)
{
    IEC5bits.U3TXIE = 0;
    IEC5bits.U3RXIE = 0;

    // STSEL 1; IREN disabled; PDSEL 8N; UARTEN enabled; RTSMD disabled; USIDL disabled; WAKE disabled; ABAUD disabled; LPBACK disabled; BRGH enabled; URXINV disabled; UEN TX_RX; 
    // Data Bits = 8; Parity = None; Stop Bits = 1;
    U3MODE = (0x8008 & ~(1<<15));  // disabling UART ON bit
    // UTXISEL0 TX_ONE_CHAR; UTXINV disabled; URXEN disabled; OERR NO_ERROR_cleared; URXISEL RX_ONE_CHAR; UTXBRK COMPLETED; UTXEN disabled; ADDEN disabled; 
    U3STA = 0x00;
    // BaudRate = 115200; Frequency = 12000000 Hz; U3BRG 25; 
    U3BRG = 0x19;
    // ADMADDR 0; ADMMASK 0; 
    U3ADMD = 0x00;
    
    txHead = txQueue;
    txTail = txQueue;
    rxHead = rxQueue;
    rxTail = rxQueue;
   
    rxOverflowed = false;

    FrontFaceUART_SetTxInterruptHandler(&FrontFaceUART_Transmit_CallBack);

    FrontFaceUART_SetRxInterruptHandler(&FrontFaceUART_Receive_CallBack);

    IEC5bits.U3RXIE = 1;
    
    //Make sure to set LAT bit corresponding to TxPin as high before UART initialization
    U3MODEbits.UARTEN = 1;   // enabling UART ON bit
    U3STAbits.UTXEN = 1;
}

/**
    Maintains the driver's transmitter state machine and implements its ISR
*/

void FrontFaceUART_SetTxInterruptHandler(void (* interruptHandler)(void))
{
    if(interruptHandler == NULL)
    {
        FrontFaceUART_TxDefaultInterruptHandler = &FrontFaceUART_Transmit_CallBack;
    }
    else
    {
        FrontFaceUART_TxDefaultInterruptHandler = interruptHandler;
    }
} 

void __attribute__ ( ( interrupt, no_auto_psv ) ) _U3TXInterrupt ( void )
{
    (*FrontFaceUART_TxDefaultInterruptHandler)();

    if(txHead == txTail)
    {
        IEC5bits.U3TXIE = 0;
    }
    else
    {
        IFS5bits.U3TXIF = 0;

        while(!(U3STAbits.UTXBF == 1))
        {
            U3TXREG = *txHead++;

            if(txHead == (txQueue + FrontFaceUART_CONFIG_TX_BYTEQ_LENGTH))
            {
                txHead = txQueue;
            }

            // Are we empty?
            if(txHead == txTail)
            {
                break;
            }
        }
    }
}

void __attribute__ ((weak)) FrontFaceUART_Transmit_CallBack ( void )
{ 

}

void FrontFaceUART_SetRxInterruptHandler(void (* interruptHandler)(void))
{
    if(interruptHandler == NULL)
    {
        FrontFaceUART_RxDefaultInterruptHandler = &FrontFaceUART_Receive_CallBack;
    }
    else
    {
        FrontFaceUART_RxDefaultInterruptHandler = interruptHandler;
    }
}

void __attribute__ ( ( interrupt, no_auto_psv ) ) _U3RXInterrupt( void )
{
    (*FrontFaceUART_RxDefaultInterruptHandler)();

    IFS5bits.U3RXIF = 0;
	
    while((U3STAbits.URXDA == 1))
    {
        *rxTail = U3RXREG;

        // Will the increment not result in a wrap and not result in a pure collision?
        // This is most often condition so check first
        if ( ( rxTail    != (rxQueue + FrontFaceUART_CONFIG_RX_BYTEQ_LENGTH-1)) &&
             ((rxTail+1) != rxHead) )
        {
            rxTail++;
        } 
        else if ( (rxTail == (rxQueue + FrontFaceUART_CONFIG_RX_BYTEQ_LENGTH-1)) &&
                  (rxHead !=  rxQueue) )
        {
            // Pure wrap no collision
            rxTail = rxQueue;
        } 
        else // must be collision
        {
            rxOverflowed = true;
        }
    }
}

void __attribute__ ((weak)) FrontFaceUART_Receive_CallBack(void)
{
//    if (FrontFaceUART_IsRxReady() == 1) { 
//        rxChar = FrontFaceUART_Read();
//        static int i = 0;
//
//        rxBuffer[i] = rxChar;
//        i++;
//        if (rxChar == '\n') {
//            new_command = true;
//            i = 0;
//        }
//    }
}

void __attribute__ ( ( interrupt, no_auto_psv ) ) _U3ErrInterrupt( void )
{
    if ((U3STAbits.OERR == 1))
    {
        U3STAbits.OERR = 0;
    }
    
    IFS5bits.U3ERIF = 0;
}

/**
  Section: UART Driver Client Routines
*/

uint8_t FrontFaceUART_Read( void)
{
    uint8_t data = 0;

    while (rxHead == rxTail )
    {
    }
    
    data = *rxHead;

    rxHead++;

    if (rxHead == (rxQueue + FrontFaceUART_CONFIG_RX_BYTEQ_LENGTH))
    {
        rxHead = rxQueue;
    }
    return data;
}

void FrontFaceUART_Write( uint8_t byte)
{
    while(FrontFaceUART_IsTxReady() == 0)
    {
    }

    *txTail = byte;

    txTail++;
    
    if (txTail == (txQueue + FrontFaceUART_CONFIG_TX_BYTEQ_LENGTH))
    {
        txTail = txQueue;
    }

    IEC5bits.U3TXIE = 1;
}

bool FrontFaceUART_IsRxReady(void)
{    
    return !(rxHead == rxTail);
}

bool FrontFaceUART_IsTxReady(void)
{
    uint16_t size;
    uint8_t *snapshot_txHead = (uint8_t*)txHead;
    
    if (txTail < snapshot_txHead)
    {
        size = (snapshot_txHead - txTail - 1);
    }
    else
    {
        size = ( FrontFaceUART_CONFIG_TX_BYTEQ_LENGTH - (txTail - snapshot_txHead) - 1 );
    }
    
    return (size != 0);
}

bool FrontFaceUART_IsTxDone(void)
{
    if(txTail == txHead)
    {
        return (bool)U3STAbits.TRMT;
    }
    
    return false;
}


/*******************************************************************************

  !!! Deprecated API !!!
  !!! These functions will not be supported in future releases !!!

*******************************************************************************/

static uint8_t FrontFaceUART_RxDataAvailable(void)
{
    uint16_t size;
    uint8_t *snapshot_rxTail = (uint8_t*)rxTail;
    
    if (snapshot_rxTail < rxHead) 
    {
        size = ( FrontFaceUART_CONFIG_RX_BYTEQ_LENGTH - (rxHead-snapshot_rxTail));
    }
    else
    {
        size = ( (snapshot_rxTail - rxHead));
    }
    
    if(size > 0xFF)
    {
        return 0xFF;
    }
    
    return size;
}

static uint8_t FrontFaceUART_TxDataAvailable(void)
{
    uint16_t size;
    uint8_t *snapshot_txHead = (uint8_t*)txHead;
    
    if (txTail < snapshot_txHead)
    {
        size = (snapshot_txHead - txTail - 1);
    }
    else
    {
        size = ( FrontFaceUART_CONFIG_TX_BYTEQ_LENGTH - (txTail - snapshot_txHead) - 1 );
    }
    
    if(size > 0xFF)
    {
        return 0xFF;
    }
    
    return size;
}

unsigned int __attribute__((deprecated)) FrontFaceUART_ReadBuffer( uint8_t *buffer ,  unsigned int numbytes)
{
    unsigned int rx_count = FrontFaceUART_RxDataAvailable();
    unsigned int i;
    
    if(numbytes < rx_count)
    {
        rx_count = numbytes;
    }
    
    for(i=0; i<rx_count; i++)
    {
        *buffer++ = FrontFaceUART_Read();
    }
    
    return rx_count;    
}

unsigned int __attribute__((deprecated)) FrontFaceUART_WriteBuffer( uint8_t *buffer , unsigned int numbytes )
{
    unsigned int tx_count = FrontFaceUART_TxDataAvailable();
    unsigned int i;
    
    if(numbytes < tx_count)
    {
        tx_count = numbytes;
    }
    
    for(i=0; i<tx_count; i++)
    {
        FrontFaceUART_Write(*buffer++);
    }
    
    return tx_count;  
}

FrontFaceUART_TRANSFER_STATUS __attribute__((deprecated)) FrontFaceUART_TransferStatusGet (void )
{
    FrontFaceUART_TRANSFER_STATUS status = 0;
    uint8_t rx_count = FrontFaceUART_RxDataAvailable();
    uint8_t tx_count = FrontFaceUART_TxDataAvailable();
    
    switch(rx_count)
    {
        case 0:
            status |= FrontFaceUART_TRANSFER_STATUS_RX_EMPTY;
            break;
        case FrontFaceUART_CONFIG_RX_BYTEQ_LENGTH:
            status |= FrontFaceUART_TRANSFER_STATUS_RX_FULL;
            break;
        default:
            status |= FrontFaceUART_TRANSFER_STATUS_RX_DATA_PRESENT;
            break;
    }
    
    switch(tx_count)
    {
        case 0:
            status |= FrontFaceUART_TRANSFER_STATUS_TX_FULL;
            break;
        case FrontFaceUART_CONFIG_RX_BYTEQ_LENGTH:
            status |= FrontFaceUART_TRANSFER_STATUS_TX_EMPTY;
            break;
        default:
            break;
    }

    return status;    
}

uint8_t __attribute__((deprecated)) FrontFaceUART_Peek(uint16_t offset)
{
    uint8_t *peek = rxHead + offset;
    
    while(peek > (rxQueue + FrontFaceUART_CONFIG_RX_BYTEQ_LENGTH))
    {
        peek -= FrontFaceUART_CONFIG_RX_BYTEQ_LENGTH;
    }
    
    return *peek;
}

bool __attribute__((deprecated)) FrontFaceUART_ReceiveBufferIsEmpty (void)
{
    return (FrontFaceUART_RxDataAvailable() == 0);
}

bool __attribute__((deprecated)) FrontFaceUART_TransmitBufferIsFull(void)
{
    return (FrontFaceUART_TxDataAvailable() == 0);
}

uint16_t __attribute__((deprecated)) FrontFaceUART_StatusGet (void)
{
    return U3STA;
}

unsigned int __attribute__((deprecated)) FrontFaceUART_TransmitBufferSizeGet(void)
{
    if(FrontFaceUART_TxDataAvailable() != 0)
    { 
        if(txHead > txTail)
        {
            return((txHead - txTail) - 1);
        }
        else
        {
            return((FrontFaceUART_CONFIG_TX_BYTEQ_LENGTH - (txTail - txHead)) - 1);
        }
    }
    return 0;
}

unsigned int __attribute__((deprecated)) FrontFaceUART_ReceiveBufferSizeGet(void)
{
    if(FrontFaceUART_RxDataAvailable() != 0)
    {
        if(rxHead > rxTail)
        {
            return((rxHead - rxTail) - 1);
        }
        else
        {
            return((FrontFaceUART_CONFIG_RX_BYTEQ_LENGTH - (rxTail - rxHead)) - 1);
        } 
    }
    return 0;
}

void __attribute__((deprecated)) FrontFaceUART_Enable(void)
{
    U3MODEbits.UARTEN = 1;
    U3STAbits.UTXEN = 1;
}

void __attribute__((deprecated)) FrontFaceUART_Disable(void)
{
    U3MODEbits.UARTEN = 0;
    U3STAbits.UTXEN = 0;
}


//FTDI UART
//.....................................................................................................................................................................



//


#define FTDIUART_CONFIG_TX_BYTEQ_LENGTH (8+1)
#define FTDIUART_CONFIG_RX_BYTEQ_LENGTH (8+1)

/** UART Driver Queue

  @Summary
    Defines the Transmit and Receive Buffers

*/

static uint8_t txQueue[FTDIUART_CONFIG_TX_BYTEQ_LENGTH];
static uint8_t rxQueue[FTDIUART_CONFIG_RX_BYTEQ_LENGTH];

void (*FTDIUART_TxDefaultInterruptHandler)(void);
void (*FTDIUART_RxDefaultInterruptHandler)(void);

/**
  Section: Driver Interface
*/

void FTDIUART_Initialize(void)
{
    IEC5bits.U4TXIE = 0;
    IEC5bits.U4RXIE = 0;

    // STSEL 1; IREN disabled; PDSEL 8N; UARTEN enabled; RTSMD disabled; USIDL disabled; WAKE disabled; ABAUD disabled; LPBACK disabled; BRGH enabled; URXINV disabled; UEN TX_RX; 
    // Data Bits = 8; Parity = None; Stop Bits = 1;
    U4MODE = (0x8008 & ~(1<<15));  // disabling UART ON bit
    // UTXISEL0 TX_ONE_CHAR; UTXINV disabled; URXEN disabled; OERR NO_ERROR_cleared; URXISEL RX_ONE_CHAR; UTXBRK COMPLETED; UTXEN disabled; ADDEN disabled; 
    U4STA = 0x00;
    // BaudRate = 115200; Frequency = 12000000 Hz; U4BRG 25; 
    U4BRG = 0x19;
    // ADMADDR 0; ADMMASK 0; 
    U4ADMD = 0x00;
    
    txHead = txQueue;
    txTail = txQueue;
    rxHead = rxQueue;
    rxTail = rxQueue;
   
    rxOverflowed = false;

    FTDIUART_SetTxInterruptHandler(&FTDIUART_Transmit_CallBack);

    FTDIUART_SetRxInterruptHandler(&FTDIUART_Receive_CallBack);

    IEC5bits.U4RXIE = 1;
    
    //Make sure to set LAT bit corresponding to TxPin as high before UART initialization
    U4MODEbits.UARTEN = 1;   // enabling UART ON bit
    U4STAbits.UTXEN = 1;
}

/**
    Maintains the driver's transmitter state machine and implements its ISR
*/

void FTDIUART_SetTxInterruptHandler(void (* interruptHandler)(void))
{
    if(interruptHandler == NULL)
    {
        FTDIUART_TxDefaultInterruptHandler = &FTDIUART_Transmit_CallBack;
    }
    else
    {
        FTDIUART_TxDefaultInterruptHandler = interruptHandler;
    }
} 

void __attribute__ ( ( interrupt, no_auto_psv ) ) _U4TXInterrupt ( void )
{
    (*FTDIUART_TxDefaultInterruptHandler)();

    if(txHead == txTail)
    {
        IEC5bits.U4TXIE = 0;
    }
    else
    {
        IFS5bits.U4TXIF = 0;

        while(!(U4STAbits.UTXBF == 1))
        {
            U4TXREG = *txHead++;

            if(txHead == (txQueue + FTDIUART_CONFIG_TX_BYTEQ_LENGTH))
            {
                txHead = txQueue;
            }

            // Are we empty?
            if(txHead == txTail)
            {
                break;
            }
        }
    }
}

void __attribute__ ((weak)) FTDIUART_Transmit_CallBack ( void )
{ 

}

void FTDIUART_SetRxInterruptHandler(void (* interruptHandler)(void))
{
    if(interruptHandler == NULL)
    {
        FTDIUART_RxDefaultInterruptHandler = &FTDIUART_Receive_CallBack;
    }
    else
    {
        FTDIUART_RxDefaultInterruptHandler = interruptHandler;
    }
}

void __attribute__ ( ( interrupt, no_auto_psv ) ) _U4RXInterrupt( void )
{
    (*FTDIUART_RxDefaultInterruptHandler)();

    IFS5bits.U4RXIF = 0;
	
    while((U4STAbits.URXDA == 1))
    {
        *rxTail = U4RXREG;

        // Will the increment not result in a wrap and not result in a pure collision?
        // This is most often condition so check first
        if ( ( rxTail    != (rxQueue + FTDIUART_CONFIG_RX_BYTEQ_LENGTH-1)) &&
             ((rxTail+1) != rxHead) )
        {
            rxTail++;
        } 
        else if ( (rxTail == (rxQueue + FTDIUART_CONFIG_RX_BYTEQ_LENGTH-1)) &&
                  (rxHead !=  rxQueue) )
        {
            // Pure wrap no collision
            rxTail = rxQueue;
        } 
        else // must be collision
        {
            rxOverflowed = true;
        }
    }
}

void __attribute__ ((weak)) FTDIUART_Receive_CallBack(void)
{
//    if (FTDIUART_IsRxReady() == 1) { 
//        rxChar = FTDIUART_Read();
//        static int i = 0;
//
//        rxBuffer[i] = rxChar;
//        i++;
//        if (rxChar == '\n') {
//            new_command = true;
//            i = 0;
//        }
//    }
}

void __attribute__ ( ( interrupt, no_auto_psv ) ) _U4ErrInterrupt( void )
{
    if ((U4STAbits.OERR == 1))
    {
        U4STAbits.OERR = 0;
    }
    
    IFS5bits.U4ERIF = 0;
}

/**
  Section: UART Driver Client Routines
*/

uint8_t FTDIUART_Read( void)
{
    uint8_t data = 0;

    while (rxHead == rxTail )
    {
    }
    
    data = *rxHead;

    rxHead++;

    if (rxHead == (rxQueue + FTDIUART_CONFIG_RX_BYTEQ_LENGTH))
    {
        rxHead = rxQueue;
    }
    return data;
}

void FTDIUART_Write( uint8_t byte)
{
    while(FTDIUART_IsTxReady() == 0)
    {
    }

    *txTail = byte;

    txTail++;
    
    if (txTail == (txQueue + FTDIUART_CONFIG_TX_BYTEQ_LENGTH))
    {
        txTail = txQueue;
    }

    IEC5bits.U4TXIE = 1;
}

bool FTDIUART_IsRxReady(void)
{    
    return !(rxHead == rxTail);
}

bool FTDIUART_IsTxReady(void)
{
    uint16_t size;
    uint8_t *snapshot_txHead = (uint8_t*)txHead;
    
    if (txTail < snapshot_txHead)
    {
        size = (snapshot_txHead - txTail - 1);
    }
    else
    {
        size = ( FTDIUART_CONFIG_TX_BYTEQ_LENGTH - (txTail - snapshot_txHead) - 1 );
    }
    
    return (size != 0);
}

bool FTDIUART_IsTxDone(void)
{
    if(txTail == txHead)
    {
        return (bool)U4STAbits.TRMT;
    }
    
    return false;
}


/*******************************************************************************

  !!! Deprecated API !!!
  !!! These functions will not be supported in future releases !!!

*******************************************************************************/

static uint8_t FTDIUART_RxDataAvailable(void)
{
    uint16_t size;
    uint8_t *snapshot_rxTail = (uint8_t*)rxTail;
    
    if (snapshot_rxTail < rxHead) 
    {
        size = ( FTDIUART_CONFIG_RX_BYTEQ_LENGTH - (rxHead-snapshot_rxTail));
    }
    else
    {
        size = ( (snapshot_rxTail - rxHead));
    }
    
    if(size > 0xFF)
    {
        return 0xFF;
    }
    
    return size;
}

static uint8_t FTDIUART_TxDataAvailable(void)
{
    uint16_t size;
    uint8_t *snapshot_txHead = (uint8_t*)txHead;
    
    if (txTail < snapshot_txHead)
    {
        size = (snapshot_txHead - txTail - 1);
    }
    else
    {
        size = ( FTDIUART_CONFIG_TX_BYTEQ_LENGTH - (txTail - snapshot_txHead) - 1 );
    }
    
    if(size > 0xFF)
    {
        return 0xFF;
    }
    
    return size;
}

unsigned int __attribute__((deprecated)) FTDIUART_ReadBuffer( uint8_t *buffer ,  unsigned int numbytes)
{
    unsigned int rx_count = FTDIUART_RxDataAvailable();
    unsigned int i;
    
    if(numbytes < rx_count)
    {
        rx_count = numbytes;
    }
    
    for(i=0; i<rx_count; i++)
    {
        *buffer++ = FTDIUART_Read();
    }
    
    return rx_count;    
}

unsigned int __attribute__((deprecated)) FTDIUART_WriteBuffer( uint8_t *buffer , unsigned int numbytes )
{
    unsigned int tx_count = FTDIUART_TxDataAvailable();
    unsigned int i;
    
    if(numbytes < tx_count)
    {
        tx_count = numbytes;
    }
    
    for(i=0; i<tx_count; i++)
    {
        FTDIUART_Write(*buffer++);
    }
    
    return tx_count;  
}

FTDIUART_TRANSFER_STATUS __attribute__((deprecated)) FTDIUART_TransferStatusGet (void )
{
    FTDIUART_TRANSFER_STATUS status = 0;
    uint8_t rx_count = FTDIUART_RxDataAvailable();
    uint8_t tx_count = FTDIUART_TxDataAvailable();
    
    switch(rx_count)
    {
        case 0:
            status |= FTDIUART_TRANSFER_STATUS_RX_EMPTY;
            break;
        case FTDIUART_CONFIG_RX_BYTEQ_LENGTH:
            status |= FTDIUART_TRANSFER_STATUS_RX_FULL;
            break;
        default:
            status |= FTDIUART_TRANSFER_STATUS_RX_DATA_PRESENT;
            break;
    }
    
    switch(tx_count)
    {
        case 0:
            status |= FTDIUART_TRANSFER_STATUS_TX_FULL;
            break;
        case FTDIUART_CONFIG_RX_BYTEQ_LENGTH:
            status |= FTDIUART_TRANSFER_STATUS_TX_EMPTY;
            break;
        default:
            break;
    }

    return status;    
}

uint8_t __attribute__((deprecated)) FTDIUART_Peek(uint16_t offset)
{
    uint8_t *peek = rxHead + offset;
    
    while(peek > (rxQueue + FTDIUART_CONFIG_RX_BYTEQ_LENGTH))
    {
        peek -= FTDIUART_CONFIG_RX_BYTEQ_LENGTH;
    }
    
    return *peek;
}

bool __attribute__((deprecated)) FTDIUART_ReceiveBufferIsEmpty (void)
{
    return (FTDIUART_RxDataAvailable() == 0);
}

bool __attribute__((deprecated)) FTDIUART_TransmitBufferIsFull(void)
{
    return (FTDIUART_TxDataAvailable() == 0);
}

uint16_t __attribute__((deprecated)) FTDIUART_StatusGet (void)
{
    return U4STA;
}

unsigned int __attribute__((deprecated)) FTDIUART_TransmitBufferSizeGet(void)
{
    if(FTDIUART_TxDataAvailable() != 0)
    { 
        if(txHead > txTail)
        {
            return((txHead - txTail) - 1);
        }
        else
        {
            return((FTDIUART_CONFIG_TX_BYTEQ_LENGTH - (txTail - txHead)) - 1);
        }
    }
    return 0;
}

unsigned int __attribute__((deprecated)) FTDIUART_ReceiveBufferSizeGet(void)
{
    if(FTDIUART_RxDataAvailable() != 0)
    {
        if(rxHead > rxTail)
        {
            return((rxHead - rxTail) - 1);
        }
        else
        {
            return((FTDIUART_CONFIG_RX_BYTEQ_LENGTH - (rxTail - rxHead)) - 1);
        } 
    }
    return 0;
}

void __attribute__((deprecated)) FTDIUART_Enable(void)
{
    U4MODEbits.UARTEN = 1;
    U4STAbits.UTXEN = 1;
}

void __attribute__((deprecated)) FTDIUART_Disable(void)
{
    U4MODEbits.UARTEN = 0;
    U4STAbits.UTXEN = 0;
}