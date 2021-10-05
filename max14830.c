#include "max14830.h"
#include "timer.h"

void CLK_CONFIG_Init(uint8_t UART);
void UARTS_Mode_Init(uint8_t UART);
void GPIO_Init();
void INTERRUPT_Init();
void FIFO_CONTROL_Init(uint8_t UART);

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
    
    INTERRUPT_Init();
    
    FIFO_CONTROL_Init(MAX_UART0_WRITE);
}

void CLK_CONFIG_Init(uint8_t UART)
{
    maxWriteByte(UART, MAX14830_PLLCONFIG , 0x01);                  //PreDIV0=1
    maxWriteByte(UART, MAX14830_BRGCONFIG, 0x03);         
    maxWriteByte(UART, MAX14830_DIVLSB, 0x02);                      //Div1=1
    maxWriteByte(UART, MAX14830_DIVMSB, 0x00);           
    maxWriteByte(UART, MAX14830_CLKSOURCE, 0x0A);                   //PLLBypass=1, CrystalEn=1
}

void UARTS_Mode_Init(uint8_t UART)
{
    maxWriteByte(UART, MAX14830_MODE1 , 0x80);
    maxWriteByte(UART, MAX14830_MODE2 , 0x02);           //Clears FIFO's on boot 
    maxWriteByte(UART, MAX14830_MODE2 , 0x00);           //Set FIFORst bit back to 0
    maxWriteByte(UART, MAX14830_LCR , 0x03);                        //LCR setup 8 bit word, 1 stop bit, no parity
    maxWriteByte(UART, MAX14830_RXTIMEOUT , 0x00);
    maxWriteByte(UART, MAX14830_HDPLXDELAY , 0x00);
    maxWriteByte(UART, MAX14830_IRDA , 0x00);
}

void GPIO_Init()
{
    maxWriteByte(MAX_UART1_WRITE, MAX14830_GPIOCONFG, 0x08);        //GPIO7-OUT
    maxWriteByte(MAX_UART2_WRITE, MAX14830_GPIOCONFG, 0x0B);        //GPIO8-O, GPIO9-O, GPIO10-I, GPIO11-O
    maxWriteByte(MAX_UART3_WRITE, MAX14830_GPIOCONFG, 0x0E);        //GPIO13-0, GPIO14-O, GPIO15-O
}

void INTERRUPT_Init()
{
    maxWriteByte(MAX_UART0_WRITE, MAX14830_IRQEN, 0x08);
    maxWriteByte(MAX_UART2_WRITE, MAX14830_IRQEN, 0x04);
    maxWriteByte(MAX_UART2_WRITE, MAX14830_STSINTEN, 0x04);     //enable interrupt for POWER_BUTTON_STAT GPIO
}

void FIFO_CONTROL_Init(uint8_t UART)
{
    //Can only set Rx trig lvl to 8 bytes minimum, if trglvl = 0 then interrupt is constantly fired
    maxWriteByte(UART, MAX14830_FIFOTRGLVL, 0x10);
}