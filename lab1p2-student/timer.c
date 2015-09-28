/*
 * File:   timer.c
 * Authors:
 *
 * Created on December 30, 2014, 8:07 PM
 */

#include <xc.h>
#include "timer.h"
#define us 14 //microsecond
//Uses timer 2
void delayUs(unsigned int delay){
    
    TMR2 = 0; //reset Timer2
    PR2 = delay * us; //set the period
    T2CONbits.TCKPS = 0; //pre-scalar = 1
    IFS0bits.T2IF = 0; //put Timer2 interrupt flag down
    T2CONbits.TON = 1; //turn on Timer2

    while(IFS0bits.T2IF == 0);    //while the flag is down, wait

    IFS0bits.T2IF = 0; //put the flag down afterwards.
    T2CONbits.TON = 0; //turn off Timer2
    
}

void initTimer2()
{
    TMR2=0; //resets timer
    T2CONbits.TCKPS=6; //set pre-scaler to 64 
    T2CONbits.TCS = 0; //configures oscillator
    IFS0bits.T2IF = 0; //interrupt flag
}