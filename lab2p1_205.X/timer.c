/*
 * File:   timer.c
 * Authors:
 *
 * Created on December 30, 2014, 8:07 PM
 */

#include <xc.h>
#include <sys/attribs.h>
#include "timer.h"

#define us 14 //microsecond
#define fcy 8000000 
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

delayMs(unsigned int delay){
    
    TMR2 = 0; //reset the timer
    PR2 = delay*8; //delay in seconds
    IFS0bits.T2IF=0; //interrupt flag
    T2CONbits.ON=1; //turn timer on
    //while the flag is down, wait
    while(IFS0bits.T2IF == 0);
    T2CONbits.ON=0; //turn off Timer2
}


