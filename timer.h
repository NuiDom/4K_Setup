/* 
 * File:   timer.h
 * Author: root
 *
 * Created on August 27, 2021, 8:52 AM
 */

#ifndef TIMER_H
#define	TIMER_H

#define FOSC            24000000
#define FCY             FOSC/2
#define PRE_SCALER      256
#define SEC        FCY/PRE_SCALER

#ifdef	__cplusplus
extern "C" {
#endif

void TMR1_Init();
void delay_ms(int delay);

#ifdef	__cplusplus
}
#endif

#endif	/* TIMER_H */

