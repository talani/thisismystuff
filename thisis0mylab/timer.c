/*
 * File:   timer.c
 * Author: gvanhoy
 *
 * Created on August 27, 2015, 3:26 PM
 */

#include "timer.h"

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
    
    PR1 = 9764; //initializes Timer1 to have a period = 1s. 
    
    //sets pre-scaler to 64
    T1CONbits.TCKPS1 = 1;
    T1CONbits.TCKPS0 = 0;
    
    T1CONbits.ON = 1; //turn timer on
}

initTimer2()
{
    TMR2=0; //resets timer
    T2CONbits.TCKPS=6; //set pre-scaler to 64 
    T2CONbits.TCS = 0; //configures oscillator
    IFS0bits.T2IF = 0; //interrupt flag
}

delayMs(unsigned int delay){
    //Must change PR to get the delay we want
    //figure out how many cycles the timer ticks before 1s passes
    //fcy = 8000000/2 --> *20 --> /16 --> /8 = 625000 ticks in one second
    //divide by 1000 = 625 ticks in 1 ms
    //divide by pre-scaler = 625
    //subtract one = 624 ticks
    
    TMR2 = 0; //reset the timer
    PR2 = delay*8; //delay in seconds
    IFS0bits.T2IF=0; //interrupt flag
    T2CONbits.ON=1; //turn timer on
    //while the flag is down, wait
    while(IFS0bits.T2IF == 0);
    T2CONbits.ON=0; //turn off Timer2
}

