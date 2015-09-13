/* 
 * File:   timer.c
 * Author: gvanhoy
 *
 * Created on August 27, 2015, 3:26 PM
 */

//#include <xc.h>
#include "timer.h"
//#define FOSC (8000000.0)
//#define fcy (625000)
//#define PRESCALE 64.0
//#define PR1DESIRED 1.0
//#define PERIOD1 (((PR1DESIRED*fcy)/PRESCALE) - 1)

void initTimer1(){
    //TODO: Initialize Timer 1 to have a period of
    // 1 second. Enable its interrupt
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
    
    PR1 = 9764;
   // PR1 = PERIOD1;
    
    //sets pre-scaler to 64
    T1CONbits.TCKPS1 = 1;
    T1CONbits.TCKPS0 = 0;
    
    T1CONbits.ON = 1; //turn timer on
}

initTimer2(){
    //TODO: Initialize Timer 2.
    TMR2=0; //resets timer
    T2CONbits.TCKPS=6;    //set pre-scaler to 1
    T2CONbits.TCS = 0; //configures oscillator
    IFS0bits.T2IF = 0; //interrupt flag
//    IPC2bits.T2IP = 4;// Configure the Interrupt Priority
}

delayMs(unsigned int delay){
    //TODO: Using timer 2, create a delay
    // that is delay amount of ms.
    //Must change PR to get the delay we want
    //figure out how many cycles the timer ticks before 1s passes
    //fcy = 8000000/2 --> *20 --> /16 --> /8 = 625000 ticks in one second
    //divide by 1000 = 625 ticks in 1 ms
    //divide by pre-scaler = 625
    //subtract one = 624 ticks
    
    TMR2 = 0;
    PR2 = delay*8; //delay in seconds
    IFS0bits.T2IF=0;
    T2CONbits.ON=1; //turn timer on
    //while the flag is down, wait
    while(IFS0bits.T2IF == 0);
    T2CONbits.ON=0; 
}

