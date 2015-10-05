/*
 * File:   timer.c
 * Authors:
 *
 * Created on December 30, 2014, 8:07 PM
 */

#include <xc.h>
#include <sys/attribs.h>
#include "timer.h"
//#include <stdio.h>
//#include <stdlib.h>

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

void initTimer1()
{
    TMR1 = 0; //clear TMR1

    IEC0bits.T1IE = 1; //enable the interrupt
    IFS0bits.T1IF = 0;// Put the flag down
    IPC1bits.T1IP = 7;// Configure the Interrupt Priority
    
    
    //PR1 = ((fcy*0.01)/256)-1;
    PR1=3124;
    T1CONbits.TCKPS=0b11;
    
        T1CONbits.ON = 1; //turn timer on
}
