#include "max14830.h"
#include "timer.h"

void CLK_CONFIG_Init(uint8_t UART);
void UARTS_Mode_Init(uint8_t UART);
void GPIO_Init();

void MAX_Init()
{
    CLK_CONFIG_Init(MAX_UART0_WRITE);
    CLK_CONFIG_Init(MAX_UART1_WRITE);
    CLK_CONFIG_Init(MAX_UART2_WRITE);
    CLK_CONFIG_Init(MAX_UART3_WRITE);
    
    UARTS_Mode_Init(MAX_UART0_WRITE);
    UARTS_Mode_Init(MAX_UART1_WRITE);
    UARTS_Mode_Init(MAX_UART2_WRITE);
    UARTS_Mode_Init(MAX_UART3_WRITE);
    
    GPIO_Init();
            
    delay_ms(200);
}

void CLK_CONFIG_Init(uint8_t UART)
{
    maxWriteByte(MAX_UART0_WRITE, MAX14830_PLLCONFIG , 0x01);       //PreDIV0=1
    delay_ms(200);
    maxWriteByte(UART, MAX14830_BRGCONFIG, 0x00);         
    delay_ms(200);
    maxWriteByte(UART, MAX14830_DIVLSB, 0x02);                      //Div1=1
    delay_ms(200);
    maxWriteByte(UART, MAX14830_DIVMSB, 0x00);           
    delay_ms(200);
    maxWriteByte(MAX_UART0_WRITE, MAX14830_CLKSOURCE, 0x0A);        //PLLBypass=1, CrystalEn=1
    delay_ms(200);

}

void UARTS_Mode_Init(uint8_t UART)
{
    maxWriteByte(MAX_UART0_WRITE, MAX14830_MODE1 , 0x00);
    delay_ms(200);
//    maxWriteByte(MAX_UART0_WRITE, MAX14830_MODE2 , 0x02);
//    delay_ms(200);
//    maxWriteByte(MAX_UART0_WRITE, MAX14830_MODE2 , 0x00);
//    delay_ms(200);
    maxWriteByte(UART, MAX14830_LCR , 0x03);                        //LCR setup 8 bit word, 1 stop bit, no parity
    delay_ms(200);                                                     //StopBits=1, Length0=1
//    maxWriteByte(MAX_UART0_WRITE, MAX14830_RXTIMEOUT , 0x00);
//    delay_ms(200);
//    maxWriteByte(MAX_UART0_WRITE, MAX14830_HDPLXDELAY , 0x00);
//    delay_ms(200);
//    maxWriteByte(MAX_UART0_WRITE, MAX14830_IRDA , 0x00);
//    delay_ms(200);

}

void GPIO_Init()
{
    maxWriteByte(MAX_UART1_WRITE, MAX14830_GPIOCONFG, 0x08);        //GPIO7-OUT
    delay_ms(200);
    maxWriteByte(MAX_UART2_WRITE, MAX14830_GPIOCONFG, 0x0B);        //GPIO8-O, GPIO9-O, GPIO10-I, GPIO11-O
    delay_ms(200);
    maxWriteByte(MAX_UART3_WRITE, MAX14830_GPIOCONFG, 0x0E);        //GPIO13-0, GPIO14-O, GPIO15-O
    delay_ms(200);
}