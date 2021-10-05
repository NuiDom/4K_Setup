/**
  Generated main.c file from MPLAB Code Configurator

  @Company
    Microchip Technology Inc.

  @File Name
    main.c

  @Summary
    This is the generated main.c using PIC24 / dsPIC33 / PIC32MM MCUs.

  @Description
    This source file provides main entry point for system initialization and application code development.
    Generation Information :
        Product Revision  :  PIC24 / dsPIC33 / PIC32MM MCUs - 1.170.0
        Device            :  PIC24FJ64GB202
    The generated drivers are tested against the following:
        Compiler          :  XC16 v1.61
        MPLAB 	          :  MPLAB X v5.45
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
#include "mcc_generated_files/system.h"
#include "mcc_generated_files/uart.h"
#include "max14830.h"
#include "timer.h"

void readMaxString(uint8_t slaveAddressW, uint8_t slaveAddressR);
/*
                         Main application
 */
int main(void)
{
    // initialize the device
    SYSTEM_Initialize();
    TMR1_Init();
    delay_ms(1000);
    if(PORTBbits.RB13 ==1)
    {
        MAX_Init();
    }
    uint8_t readData;
    while (1)
    {
        
//        maxWriteByte(MAX_UART0_WRITE, MAX14830_THR, 0x41);    // Successful test MAX TX                                        
//        delay_ms(200);

        
//        RingLightUART_Write(0x4B);
//        delay_ms(1000);
//        RingLightUART_Write(0x4B);
//        delay_ms(1000);
        
        
    if(PORTBbits.RB13 == 0)
    {
//        count++;
//        RingLightUART_Write(count);
//        maxReadByte(MAX_UART2_WRITE, MAX_UART2_READ, MAX14830_ISR);
        readData = maxReadByte(MAX_UART0_WRITE, MAX_UART0_READ, MAX14830_GLOBALIRQ);
        if(readData & 0x0E)
        {   
            readData = maxReadByte(MAX_UART0_WRITE, MAX_UART0_READ, MAX14830_ISR);                //Max Interrupt testing
            if(readData & 0x08)
            {
                readMaxString(MAX_UART0_WRITE, MAX_UART0_READ);
//                RingLightUART_Write(0x4B);
//                delay_ms(1500);
            }
        }
    }
    
    }
    return 1;
}

void readMaxString(uint8_t slaveAddressW, uint8_t slaveAddressR)
{
    uint8_t data;
    data = maxReadByte(slaveAddressW, slaveAddressR, MAX14830_RHR);
    RingLightUART_Write(data);
    while(data ='q'){
        data = maxReadByte(slaveAddressW, slaveAddressR, MAX14830_RHR);
        RingLightUART_Write(data);
    }
}
/**
 End of File
*/

