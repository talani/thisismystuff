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

void initTimer1()
{
    TMR1 = 0; //clear TMR1
    T1CONbits.ON = 0; //turn timer off
    T1CONbits.SIDL = 0; //continue operation even in idle mode
    T1CONbits.TWDIS = 0; //back to back writes are enabled
    T1CONbits.TWIP = 0; //asynchronous write to TMR1 register complete
    T1CONbits.TGATE = 0;
    T1CONbits.TCS = 0; //setting oscillator
    IEC0bits.T1IE = 1; //enable the interrupt
    IFS0bits.T1IF = 0;// Put the flag down
    IPC1bits.T1IP = 3;// Configure the Interrupt Priority
    
    PR1 = 625; //initializes Timer1 to have a period = 1s. 
    
    //sets pre-scaler to 64
    T1CONbits.TCKPS1 = 0;
    T1CONbits.TCKPS0 = 0;
    
    T1CONbits.ON = 1; //turn timer on
}

//void initTimer2()
//{
//    TMR2=0; //resets timer
//    T2CONbits.TCKPS=6; //set pre-scaler to 64 
//    T2CONbits.TCS = 0; //configures oscillator
//    IFS0bits.T2IF = 0; //interrupt flag
//}

