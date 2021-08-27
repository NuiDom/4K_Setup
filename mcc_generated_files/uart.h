/**
  CamUART Generated Driver API Header File 

  @Company
    Microchip Technology Inc.

  @File Name
    uart1.h

  @Summary
    This is the generated header file for the CamUART driver using PIC24 / dsPIC33 / PIC32MM MCUs

  @Description
    This header file provides APIs for driver for CamUART. 
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

#ifndef _CamUART_H
#define _CamUART_H

/**
 Section: Included Files
*/

#include <stdbool.h>
#include <stdint.h>

#ifdef __cplusplus  // Provide C++ Compatibility

    extern "C" {

#endif
/**
  Section: CamUART Driver Routines
*/

/**
  @Summary
    Initializes the UART instance : 1

  @Description
    This routine initializes the UART driver instance for : 1
    index.
    This routine must be called before any other UART routine is called.
    
  @Preconditions
    None.

  @Returns
    None.

  @Param
    None.

  @Comment
    None.    
 
  @Example
    None.

*/

void CamUART_Initialize(void);

/**
  @Summary
    Read a byte of data from the CamUART

  @Description
    This routine reads a byte of data from the CamUART.

  @Preconditions
    CamUART_Initialize function should have been called 
    before calling this function. The transfer status should be checked to see 
    if the receiver is not empty before calling this function.

  @Param
    None.

  @Returns
    A data byte received by the driver.

  @Example
    None.
*/

uint8_t CamUART_Read( void);

/**
  @Summary
    Writes a byte of data to the CamUART

  @Description
    This routine writes a byte of data to the CamUART.

  @Preconditions
    CamUART_Initialize function should have been called 
    before calling this function. The transfer status should be checked to see if
    transmitter is not full before calling this function.

  @Param
    byte         - Data byte to write to the CamUART

  @Returns
    None.

  @Example
    None.
*/

void CamUART_Write( uint8_t byte);


/**
  @Description
    Indicates of there is data available to read.

  @Returns
    true if byte can be read.
    false if byte can't be read right now.
*/
bool CamUART_IsRxReady(void);

/**
  @Description
    Indicates if a byte can be written.
 
 @Returns
    true if byte can be written.
    false if byte can't be written right now.
*/
bool CamUART_IsTxReady(void);

/**
  @Description
    Indicates if all bytes have been transferred.
 
 @Returns
    true if all bytes transfered.
    false if there is still data pending to transfer.
*/
bool CamUART_IsTxDone(void);

/**
  @Summary
    Assigns a function pointer with a transmit callback address.

  @Description
    This routine assigns a function pointer with a transmit callback address.

  @Param
    Address of the callback routine.

  @Returns
    None
 
  @Example 
    <code>
        CamUART_SetTxInterruptHandler(&CamUART_Transmit_CallBack);
    </code>
*/
void CamUART_SetTxInterruptHandler(void (* interruptHandler)(void));

/**
  @Summary
    Transmit callback routine.

  @Description
    This routine is a transmit callback function.

  @Param
    None.

  @Returns
    None
 
  @Example 
    <code>
        CamUART_SetTxInterruptHandler(&CamUART_Transmit_CallBack);
    </code>
*/
void CamUART_Transmit_CallBack(void);

/**
  @Summary
    Assigns a function pointer with a receive callback address.

  @Description
    This routine assigns a function pointer with a receive callback address.

  @Param
    Address of the callback routine.

  @Returns
    None
 
  @Example 
    <code>
        CamUART_SetRxInterruptHandler(&CamUART_Receive_CallBack);
    </code>
*/
void CamUART_SetRxInterruptHandler(void (* interruptHandler)(void));

/**
  @Summary
    Receive callback routine.

  @Description
    This routine is a receive callback function.

  @Param
    None.

  @Returns
    None
 
  @Example 
    <code>
        CamUART_SetTxInterruptHandler(&CamUART_Receive_CallBack);
    </code>
*/
void CamUART_Receive_CallBack(void);


/*******************************************************************************

  !!! Deprecated API and types !!!
  !!! These functions will not be supported in future releases !!!

*******************************************************************************/

/** CamUART Driver Hardware Flags

  @Summary
    Specifies the status of the hardware receive or transmit

  @Description
    This type specifies the status of the hardware receive or transmit.
    More than one of these values may be OR'd together to create a complete
    status value.  To test a value of this type, the bit of interest must be
    AND'ed with value and checked to see if the result is non-zero.
*/
typedef enum
{
    /* Indicates that Receive buffer has data, at least one more character can be read */
    CamUART_RX_DATA_AVAILABLE = (1 << 0),
    /* Indicates that Receive buffer has overflowed */
    CamUART_RX_OVERRUN_ERROR = (1 << 1),
    /* Indicates that Framing error has been detected for the current character */
    CamUART_FRAMING_ERROR = (1 << 2),
    /* Indicates that Parity error has been detected for the current character */
    CamUART_PARITY_ERROR = (1 << 3),
    /* Indicates that Receiver is Idle */
    CamUART_RECEIVER_IDLE = (1 << 4),
    /* Indicates that the last transmission has completed */
    CamUART_TX_COMPLETE = (1 << 8),
    /* Indicates that Transmit buffer is full */
    CamUART_TX_FULL = (1 << 9) 
}CamUART_STATUS;

/** CamUART Driver Transfer Flags

  @Summary
    Specifies the status of the receive or transmit

  @Description
    This type specifies the status of the receive or transmit operation.
    More than one of these values may be OR'd together to create a complete
    status value.  To test a value of this type, the bit of interest must be
    AND'ed with value and checked to see if the result is non-zero.
*/

typedef enum
{
    /* Indicates that the core driver buffer is full */
    CamUART_TRANSFER_STATUS_RX_FULL = (1 << 0) ,
    /* Indicates that at least one byte of Data has been received */
    CamUART_TRANSFER_STATUS_RX_DATA_PRESENT = (1 << 1) ,
    /* Indicates that the core driver receiver buffer is empty */
    CamUART_TRANSFER_STATUS_RX_EMPTY = (1 << 2) ,
    /* Indicates that the core driver transmitter buffer is full */
    CamUART_TRANSFER_STATUS_TX_FULL = (1 << 3) ,
    /* Indicates that the core driver transmitter buffer is empty */
    CamUART_TRANSFER_STATUS_TX_EMPTY = (1 << 4) 
} CamUART_TRANSFER_STATUS;

/**
  @Summary
    Returns the number of bytes read by the CamUART peripheral

  @Description
    This routine returns the number of bytes read by the Peripheral and fills the
    application read buffer with the read data.

  @Preconditions
    CamUART_Initialize function should have been called 
    before calling this function

  @Param
    buffer       - Buffer into which the data read from the CamUART

  @Param
    numbytes     - Total number of bytes that need to be read from the CamUART
                   (must be equal to or less than the size of the buffer)

  @Returns
    Number of bytes actually copied into the caller's buffer or -1 if there
    is an error.

  @Example
    <code>
    char                     myBuffer[MY_BUFFER_SIZE];
    unsigned int             numBytes;
    CamUART_TRANSFER_STATUS status ;

    // Pre-initialize myBuffer with MY_BUFFER_SIZE bytes of valid data.

    numBytes = 0;
    while( numBytes < MY_BUFFER_SIZE);
    {
        status = CamUART_TransferStatusGet ( ) ;
        if (status & CamUART_TRANSFER_STATUS_RX_FULL)
        {
            numBytes += CamUART_ReadBuffer( myBuffer + numBytes, MY_BUFFER_SIZE - numBytes )  ;
            if(numBytes < readbufferLen)
            {
                continue;
            }
            else
            {
                break;
            }
        }
        else
        {
            continue;
        }

        // Do something else...
    }
    </code>
*/
unsigned int __attribute__((deprecated)) CamUART_ReadBuffer( uint8_t *buffer ,  unsigned int numbytes);

/**
  @Summary
    Returns the number of bytes written into the internal buffer

  @Description
    This API transfers the data from application buffer to internal buffer and 
    returns the number of bytes added in that queue

  @Preconditions
    CamUART_Initialize function should have been called 
    before calling this function

  @Example
    <code>
    char                     myBuffer[MY_BUFFER_SIZE];
    unsigned int             numBytes;
    CamUART_TRANSFER_STATUS status ;

    // Pre-initialize myBuffer with MY_BUFFER_SIZE bytes of valid data.

    numBytes = 0;
    while( numBytes < MY_BUFFER_SIZE);
    {
        status = CamUART_TransferStatusGet ( ) ;
        if (status & CamUART_TRANSFER_STATUS_TX_EMPTY)
        {
            numBytes += CamUART_WriteBuffer ( myBuffer + numBytes, MY_BUFFER_SIZE - numBytes )  ;
            if(numBytes < writebufferLen)
            {
                continue;
            }
            else
            {
                break;
            }
        }
        else
        {
            continue;
        }

        // Do something else...
    }
    </code>
*/
unsigned int __attribute__((deprecated)) CamUART_WriteBuffer( uint8_t *buffer , unsigned int numbytes );

/**
  @Summary
    Returns the transmitter and receiver transfer status

  @Description
    This returns the transmitter and receiver transfer status.The returned status 
    may contain a value with more than one of the bits
    specified in the CamUART_TRANSFER_STATUS enumeration set.  
    The caller should perform an "AND" with the bit of interest and verify if the
    result is non-zero (as shown in the example) to verify the desired status
    bit.

  @Preconditions
    CamUART_Initialize function should have been called 
    before calling this function

  @Param
    None.

  @Returns
    A CamUART_TRANSFER_STATUS value describing the current status 
    of the transfer.

  @Example
    Refer to CamUART_ReadBuffer and CamUART_WriteBuffer for example

*/
CamUART_TRANSFER_STATUS __attribute__((deprecated)) CamUART_TransferStatusGet (void );

/**
  @Summary
    Returns the character in the read sequence at the offset provided, without
    extracting it

  @Description
    This routine returns the character in the read sequence at the offset provided,
    without extracting it
 
  @Param
    None.
    
  @Example 
    <code>
    uint8_t readBuffer[5];
    unsigned int data, numBytes = 0;
    unsigned int readbufferLen = sizeof(readBuffer);
    CamUART_Initialize();
    
    while(numBytes < readbufferLen)        
    {   
        CamUART_TasksReceive ( );
        //Check for data at a particular place in the buffer
        data = CamUART_Peek(3);
        if(data == 5)
        {
            //discard all other data if byte that is wanted is received.    
            //continue other operation
            numBytes += CamUART_ReadBuffer ( readBuffer + numBytes , readbufferLen ) ;
        }
        else
        {
            break;
        }
    }
    </code>
 
*/
uint8_t __attribute__((deprecated)) CamUART_Peek(uint16_t offset);

/**
  @Summary
    Returns the status of the receive buffer

  @Description
    This routine returns if the receive buffer is empty or not.

  @Param
    None.
 
  @Returns
    True if the receive buffer is empty
    False if the receive buffer is not empty
    
  @Example
    <code>
    char                     myBuffer[MY_BUFFER_SIZE];
    unsigned int             numBytes;
    CamUART_TRANSFER_STATUS status ;

    // Pre-initialize myBuffer with MY_BUFFER_SIZE bytes of valid data.

    numBytes = 0;
    while( numBytes < MY_BUFFER_SIZE);
    {
        status = CamUART_TransferStatusGet ( ) ;
        if (!CamUART_ReceiveBufferIsEmpty())
        {
            numBytes += CamUART_ReadBuffer( myBuffer + numBytes, MY_BUFFER_SIZE - numBytes )  ;
            if(numBytes < readbufferLen)
            {
                continue;
            }
            else
            {
                break;
            }
        }
        else
        {
            continue;
        }

        // Do something else...
    }
    </code>
 
*/
bool __attribute__((deprecated)) CamUART_ReceiveBufferIsEmpty (void);

/**
  @Summary
    Returns the status of the transmit buffer

  @Description
    This routine returns if the transmit buffer is full or not.

 @Param
    None.
 
 @Returns
    True if the transmit buffer is full
    False if the transmit buffer is not full

 @Example
    Refer to CamUART_Initialize() for example.
 
*/
bool __attribute__((deprecated)) CamUART_TransmitBufferIsFull (void);

/**
  @Summary
    Returns the transmitter and receiver status

  @Description
    This returns the transmitter and receiver status. The returned status may 
    contain a value with more than one of the bits
    specified in the CamUART_STATUS enumeration set.  
    The caller should perform an "AND" with the bit of interest and verify if the
    result is non-zero (as shown in the example) to verify the desired status
    bit.

  @Preconditions
    CamUART_Initialize function should have been called 
    before calling this function

  @Param
    None.

  @Returns
    A CamUART_STATUS value describing the current status 
    of the transfer.

  @Example
    <code>
        while(!(CamUART_StatusGet & CamUART_TX_COMPLETE ))
        {
           // Wait for the tranmission to complete
        }
    </code>
*/
uint16_t __attribute__((deprecated)) CamUART_StatusGet (void );

/**
  @Summary
    Allows setting of a the enable bit for the CamUART mode

  @Description
    This routine is used to enable the CamUART
  
  @Preconditions
    CamUART_Initialize() function should have been 
    called before calling this function.
 
  @Returns
    None

  @Param
    None
  
  @Example
    Refer to CamUART_Initialize(); for an example
*/

void __attribute__((deprecated)) CamUART_Enable(void);

/**
  @Summary
    Allows setting of a the disable bit for the CamUART mode

  @Description
    This routine is used to disable the CamUART
  
  @Preconditions
    CamUART_Initialize() function should have been 
    called before calling this function.
 
  @Returns
    None

  @Param
    None
  
  @Example
    Refer to CamUART_Initialize(); for an example
*/

void __attribute__((deprecated)) CamUART_Disable(void);

/**
  @Summary
    Returns the number of bytes remaining in the receive buffer

  @Description
    This routine returns the number of bytes remaining in the receive buffer.

  @Param
    None.

  @Returns
    Remaining size of receive buffer.
    
  @Example 
    <code>
    uint8_t readBuffer[MY_BUFFER_SIZE];
    unsigned int size, numBytes = 0;
    CamUART_Initialize();

    // Pre-initialize readBuffer with MY_BUFFER_SIZE bytes of valid data.
    
    while (size < MY_BUFFER_SIZE) {
        size = CamUART_ReceiveBufferSizeGet();
    }
    numBytes = CamUART_ReadBuffer(readBuffer, MY_BUFFER_SIZE);
    </code>
 
*/

unsigned int __attribute__((deprecated)) CamUART_ReceiveBufferSizeGet(void);

/**
  @Summary
    Returns the number of bytes remaining in the transmit buffer.

  @Description
    This routine returns the number of bytes remaining in the transmit buffer.

 @Param
    None.
 
 @Returns
    Remaining size of transmit buffer.

 @Example
    Refer to CamUART_Initialize(); for example.
*/

unsigned int __attribute__((deprecated)) CamUART_TransmitBufferSizeGet(void);

#ifdef __cplusplus  // Provide C++ Compatibility

    }

#endif
    
#endif  // _CamUART_H
    

#ifndef _RingLightUART_H
#define _RingLightUART_H

#include <stdbool.h>
#include <stdint.h>

#ifdef __cplusplus  // Provide C++ Compatibility

    extern "C" {

#endif

void RingLightUART_Initialize(void);
uint8_t RingLightUART_Read( void);
void RingLightUART_Write( uint8_t byte);
bool RingLightUART_IsRxReady(void);
bool RingLightUART_IsTxReady(void);
bool RingLightUART_IsTxDone(void);
void RingLightUART_SetTxInterruptHandler(void (* interruptHandler)(void));
void RingLightUART_Transmit_CallBack(void);
void RingLightUART_SetRxInterruptHandler(void (* interruptHandler)(void));
void RingLightUART_Receive_CallBack(void);

typedef enum
{
    /* Indicates that Receive buffer has data, at least one more character can be read */
    RingLightUART_RX_DATA_AVAILABLE = (1 << 0),
    /* Indicates that Receive buffer has overflowed */
    RingLightUART_RX_OVERRUN_ERROR = (1 << 1),
    /* Indicates that Framing error has been detected for the current character */
    RingLightUART_FRAMING_ERROR = (1 << 2),
    /* Indicates that Parity error has been detected for the current character */
    RingLightUART_PARITY_ERROR = (1 << 3),
    /* Indicates that Receiver is Idle */
    RingLightUART_RECEIVER_IDLE = (1 << 4),
    /* Indicates that the last transmission has completed */
    RingLightUART_TX_COMPLETE = (1 << 8),
    /* Indicates that Transmit buffer is full */
    RingLightUART_TX_FULL = (1 << 9) 
}RingLightUART_STATUS;

typedef enum
{
    /* Indicates that the core driver buffer is full */
    RingLightUART_TRANSFER_STATUS_RX_FULL = (1 << 0) ,
    /* Indicates that at least one byte of Data has been received */
    RingLightUART_TRANSFER_STATUS_RX_DATA_PRESENT = (1 << 1) ,
    /* Indicates that the core driver receiver buffer is empty */
    RingLightUART_TRANSFER_STATUS_RX_EMPTY = (1 << 2) ,
    /* Indicates that the core driver transmitter buffer is full */
    RingLightUART_TRANSFER_STATUS_TX_FULL = (1 << 3) ,
    /* Indicates that the core driver transmitter buffer is empty */
    RingLightUART_TRANSFER_STATUS_TX_EMPTY = (1 << 4) 
} RingLightUART_TRANSFER_STATUS;

unsigned int __attribute__((deprecated)) RingLightUART_ReadBuffer( uint8_t *buffer ,  unsigned int numbytes);
unsigned int __attribute__((deprecated)) RingLightUART_WriteBuffer( uint8_t *buffer , unsigned int numbytes );

RingLightUART_TRANSFER_STATUS __attribute__((deprecated)) RingLightUART_TransferStatusGet (void );
uint8_t __attribute__((deprecated)) RingLightUART_Peek(uint16_t offset);
bool __attribute__((deprecated)) RingLightUART_ReceiveBufferIsEmpty (void);
bool __attribute__((deprecated)) RingLightUART_TransmitBufferIsFull (void);
uint16_t __attribute__((deprecated)) RingLightUART_StatusGet (void );
void __attribute__((deprecated)) RingLightUART_Enable(void);
void __attribute__((deprecated)) RingLightUART_Disable(void);
unsigned int __attribute__((deprecated)) RingLightUART_ReceiveBufferSizeGet(void);
unsigned int __attribute__((deprecated)) RingLightUART_TransmitBufferSizeGet(void);

#ifdef __cplusplus  // Provide C++ Compatibility

    }

#endif
    
#endif  // _RingLightUART_H
    
    
#ifndef _FrontFaceUART_H
#define _FrontFaceUART_H

#include <stdbool.h>
#include <stdint.h>

#ifdef __cplusplus  // Provide C++ Compatibility

    extern "C" {

#endif

void FrontFaceUART_Initialize(void);
uint8_t FrontFaceUART_Read( void);
void FrontFaceUART_Write( uint8_t byte);
bool FrontFaceUART_IsRxReady(void);
bool FrontFaceUART_IsTxReady(void);
bool FrontFaceUART_IsTxDone(void);
void FrontFaceUART_SetTxInterruptHandler(void (* interruptHandler)(void));
void FrontFaceUART_Transmit_CallBack(void);
void FrontFaceUART_SetRxInterruptHandler(void (* interruptHandler)(void));
void FrontFaceUART_Receive_CallBack(void);

typedef enum
{
    /* Indicates that Receive buffer has data, at least one more character can be read */
    FrontFaceUART_RX_DATA_AVAILABLE = (1 << 0),
    /* Indicates that Receive buffer has overflowed */
    FrontFaceUART_RX_OVERRUN_ERROR = (1 << 1),
    /* Indicates that Framing error has been detected for the current character */
    FrontFaceUART_FRAMING_ERROR = (1 << 2),
    /* Indicates that Parity error has been detected for the current character */
    FrontFaceUART_PARITY_ERROR = (1 << 3),
    /* Indicates that Receiver is Idle */
    FrontFaceUART_RECEIVER_IDLE = (1 << 4),
    /* Indicates that the last transmission has completed */
    FrontFaceUART_TX_COMPLETE = (1 << 8),
    /* Indicates that Transmit buffer is full */
    FrontFaceUART_TX_FULL = (1 << 9) 
}FrontFaceUART_STATUS;

typedef enum
{
    /* Indicates that the core driver buffer is full */
    FrontFaceUART_TRANSFER_STATUS_RX_FULL = (1 << 0) ,
    /* Indicates that at least one byte of Data has been received */
    FrontFaceUART_TRANSFER_STATUS_RX_DATA_PRESENT = (1 << 1) ,
    /* Indicates that the core driver receiver buffer is empty */
    FrontFaceUART_TRANSFER_STATUS_RX_EMPTY = (1 << 2) ,
    /* Indicates that the core driver transmitter buffer is full */
    FrontFaceUART_TRANSFER_STATUS_TX_FULL = (1 << 3) ,
    /* Indicates that the core driver transmitter buffer is empty */
    FrontFaceUART_TRANSFER_STATUS_TX_EMPTY = (1 << 4) 
} FrontFaceUART_TRANSFER_STATUS;

unsigned int __attribute__((deprecated)) FrontFaceUART_ReadBuffer( uint8_t *buffer ,  unsigned int numbytes);
unsigned int __attribute__((deprecated)) FrontFaceUART_WriteBuffer( uint8_t *buffer , unsigned int numbytes );
FrontFaceUART_TRANSFER_STATUS __attribute__((deprecated)) FrontFaceUART_TransferStatusGet (void );
uint8_t __attribute__((deprecated)) FrontFaceUART_Peek(uint16_t offset);
bool __attribute__((deprecated)) FrontFaceUART_ReceiveBufferIsEmpty (void);
bool __attribute__((deprecated)) FrontFaceUART_TransmitBufferIsFull (void);
uint16_t __attribute__((deprecated)) FrontFaceUART_StatusGet (void );
void __attribute__((deprecated)) FrontFaceUART_Enable(void);
void __attribute__((deprecated)) FrontFaceUART_Disable(void);
unsigned int __attribute__((deprecated)) FrontFaceUART_ReceiveBufferSizeGet(void);
unsigned int __attribute__((deprecated)) FrontFaceUART_TransmitBufferSizeGet(void);

#ifdef __cplusplus  // Provide C++ Compatibility

    }

#endif
    
#endif  // _FrontFaceUART_H
    
    
#ifndef _FTDIUART_H
#define _FTDIUART_H

#include <stdbool.h>
#include <stdint.h>

#ifdef __cplusplus  // Provide C++ Compatibility

    extern "C" {

#endif
        
void FTDIUART_Initialize(void);
uint8_t FTDIUART_Read( void);
void FTDIUART_Write( uint8_t byte);
bool FTDIUART_IsRxReady(void);
bool FTDIUART_IsTxReady(void);
bool FTDIUART_IsTxDone(void);
void FTDIUART_SetTxInterruptHandler(void (* interruptHandler)(void));
void FTDIUART_Transmit_CallBack(void);
void FTDIUART_SetRxInterruptHandler(void (* interruptHandler)(void));
void FTDIUART_Receive_CallBack(void);

typedef enum
{
    /* Indicates that Receive buffer has data, at least one more character can be read */
    FTDIUART_RX_DATA_AVAILABLE = (1 << 0),
    /* Indicates that Receive buffer has overflowed */
    FTDIUART_RX_OVERRUN_ERROR = (1 << 1),
    /* Indicates that Framing error has been detected for the current character */
    FTDIUART_FRAMING_ERROR = (1 << 2),
    /* Indicates that Parity error has been detected for the current character */
    FTDIUART_PARITY_ERROR = (1 << 3),
    /* Indicates that Receiver is Idle */
    FTDIUART_RECEIVER_IDLE = (1 << 4),
    /* Indicates that the last transmission has completed */
    FTDIUART_TX_COMPLETE = (1 << 8),
    /* Indicates that Transmit buffer is full */
    FTDIUART_TX_FULL = (1 << 9) 
}FTDIUART_STATUS;

typedef enum
{
    /* Indicates that the core driver buffer is full */
    FTDIUART_TRANSFER_STATUS_RX_FULL = (1 << 0) ,
    /* Indicates that at least one byte of Data has been received */
    FTDIUART_TRANSFER_STATUS_RX_DATA_PRESENT = (1 << 1) ,
    /* Indicates that the core driver receiver buffer is empty */
    FTDIUART_TRANSFER_STATUS_RX_EMPTY = (1 << 2) ,
    /* Indicates that the core driver transmitter buffer is full */
    FTDIUART_TRANSFER_STATUS_TX_FULL = (1 << 3) ,
    /* Indicates that the core driver transmitter buffer is empty */
    FTDIUART_TRANSFER_STATUS_TX_EMPTY = (1 << 4) 
} FTDIUART_TRANSFER_STATUS;

unsigned int __attribute__((deprecated)) FTDIUART_ReadBuffer( uint8_t *buffer ,  unsigned int numbytes);
unsigned int __attribute__((deprecated)) FTDIUART_WriteBuffer( uint8_t *buffer , unsigned int numbytes );

FTDIUART_TRANSFER_STATUS __attribute__((deprecated)) FTDIUART_TransferStatusGet (void );
uint8_t __attribute__((deprecated)) FTDIUART_Peek(uint16_t offset);
bool __attribute__((deprecated)) FTDIUART_ReceiveBufferIsEmpty (void);
bool __attribute__((deprecated)) FTDIUART_TransmitBufferIsFull (void);
uint16_t __attribute__((deprecated)) FTDIUART_StatusGet (void );
void __attribute__((deprecated)) FTDIUART_Enable(void);
void __attribute__((deprecated)) FTDIUART_Disable(void);
unsigned int __attribute__((deprecated)) FTDIUART_ReceiveBufferSizeGet(void);
unsigned int __attribute__((deprecated)) FTDIUART_TransmitBufferSizeGet(void);

#ifdef __cplusplus  // Provide C++ Compatibility

    }

#endif
    
#endif  // _FTDIUART_H    