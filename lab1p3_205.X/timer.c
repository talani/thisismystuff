/*
 * File:   timer.c
 * Authors:
 *
 * Created on December 30, 2014, 8:07 PM
 */

#include <xc.h>
#include "timer.h"
//#include <stdio.h>
//#include <stdlib.h>

#define us 14 //microsecond
#define fcy 80000000
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
    IPC1bits.T1IP = 3;// Configure the Interrupt Priority
    
    //2^16=65,536
    //fcy =80MHz
    //delay=0.01s
    //8M*0.01=800,000
    
    PR1 = ((fcy*0.01)/256)-1;
    //PR1 = 3124; //initializes Timer1 to have a period = 1s. 
    T1CONbits.TCKPS=0b11;
    //sets pre-scaler to 256
    //T1CONbits.TCKPS1 = 1;
    //T1CONbits.TCKPS0 = 1;
    
    //T1CONbits.ON = 0; //turn timer on
}

//void initTimer2()
//{
//    TMR2=0; //resets timer
//    T2CONbits.TCKPS=6; //set pre-scaler to 64 
//    T2CONbits.TCS = 0; //configures oscillator
//    IFS0bits.T2IF = 0; //interrupt flag
//}
//void getTimedString(int watch, char* str){
//    int FF = watch % 100; //Get the lower 2 numbers
//    int SS = (watch / 100) % 60; //MOD 60 to turn 60 seconds in a minute
//    int MM = ((watch / 100) - SS) / 60; //Convert watch to minute
//    
//    
//    sprintf(str, "%02d:%02d:%02d", MM, SS, FF); //Prints the number to a string
//
//}
