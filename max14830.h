/* 
 * File:   max14830.h
 * Author: root
 *
 * Created on 24 August 2021, 08:42
 */

#ifndef MAX14830_H
#define	MAX14830_H
//UART Write and Reads
#define MAX_UART0_WRITE         0xD8
#define MAX_UART0_READ          0xD9
#define MAX_UART1_WRITE         0xB8
#define MAX_UART1_READ          0xB9
#define MAX_UART2_WRITE         0x58
#define MAX_UART2_READ          0x59
#define MAX_UART3_WRITE         0x38
#define MAX_UART3_READ          0x39
//FIFO Data
#define MAX14830_RHR            0x00//
#define MAX14830_THR            0x00//
//Interrupts
#define MAX14830_IRQEN          0x01
#define MAX14830_ISR            0x02
#define MAX14830_LSRINTEN       0x03
#define MAX14830_LSR            0x04
#define MAX14830_SPCLCHRINTEN   0x05
#define MAX14830_SPCLCHARINT    0x06
#define MAX14830_STSINTEN       0x07
#define MAX14830_STSINT         0x08
//UART Modes
#define MAX14830_MODE1          0x09//
#define MAX14830_MODE2          0x0A//
#define MAX14830_LCR            0x0B//
#define MAX14830_RXTIMEOUT      0x0C//
#define MAX14830_HDPLXDELAY     0x0D//
#define MAX14830_IRDA           0x0E//
//FIFOs Control
#define MAX14830_FLOWLVL        0x0F
#define MAX14830_FIFOTRGLVL     0x10
#define MAX14830_TXFIFOLVL      0x11// read only
#define MAX14830_RXFIFOLVL      0x12//read only
//Flow Control
#define MAX14830_FLOWCTRL       0x13// Leave 0 for no flow control
#define MAX14830_XON1           0x14
#define MAX14830_XON2           0x15
#define MAX14830_XOFF1          0x16
#define MAX14830_XOFF2          0x17
//GPIOs
#define MAX14830_GPIOCONFG      0x18//
#define MAX14830_GPIODATA       0x19//
//Clock Config
#define MAX14830_PLLCONFIG      0x1A//
#define MAX14830_BRGCONFIG      0x1B//
#define MAX14830_DIVLSB         0x1C//
#define MAX14830_DIVMSB         0x1D//
#define MAX14830_CLKSOURCE      0x1E//
//Global
#define MAX14830_GLOBALIRQ      0x1F
#define MAX14830_GLOBALCOMND    0x1F
//Sync
#define MAX14830_TXSYNCH        0x20
#define MAX14830_SYNCHDELAY1    0x21
#define MAX14830_SYNCHDELAY2    0x22
//Timers
#define MAX14830_TIMER1         0x23
#define MAX14830_TIMER2         0x24
//Board Revision
#define MAX14830_REVID          0x25

#include "mcc_generated_files/i2c2.h"

#ifdef	__cplusplus
extern "C" {
#endif

void MAX_Init();


#ifdef	__cplusplus
}
#endif

#endif	/* MAX14830_H */

