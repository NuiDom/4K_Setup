#include "timer.h"  
#include "mcc_generated_files/system.h"

void TMR1_Init()
{
    TMR1 = 0;
    PR1 = 0xFFFF;
//    IPC0bits.T1IP0 = 1;
//    IFS0bits.T1IF = 0;
//    IEC0bits.T1IE = 1;
    T1CON = 0x8030;
}

//void __attribute__((__interrupt__, __shadow__)) _T1Interruppt(void)
//{
//    IFS0bits.T1IF = 0;
//    TMR1 = 0;
//}
void delay_ms(int delay)
{
    float tmp = delay/1000;
    while(TMR1<SEC*tmp){}
    TMR1 = 0;
}