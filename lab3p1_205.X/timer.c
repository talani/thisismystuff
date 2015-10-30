/*
 * File:   timer.c
 */

#include <sys/attribs.h>
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

void initTimer3()
{
    TMR3 = 0; //clear TMR3
    T3CONbits.ON = 0; //turn timer off
    T3CONbits.SIDL = 0; //continue operation even in idle mode
   
    T3CONbits.TGATE = 0;
    T3CONbits.TCS = 0; //setting oscillator
    
    PR3 = 1023; //initializes Timer3 to have 1023
    
    //sets pre-scaler to 256
    T3CONbits.TCKPS2 = 1;
    T3CONbits.TCKPS1 = 1;
    T3CONbits.TCKPS0 = 1;
    
    T3CONbits.ON = 1; //turn timer on
}