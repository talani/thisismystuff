/*
* File:   leds.c
* Author: 
*
* Created on December 27, 2014, 1:31 PM
*/

#include "leds.h"

#define OUTPUT 0
#define INPUT 1

void initLEDs(){
    //TODO: Initialize the appropriate pins to work with the LEDs
    
    //use LED1 and LED2 on board 
    //TODO: change this to external LEDs TRD1, TRD2
    
    TRISGbits.TRISG12 = OUTPUT; // set led1 as output RUN
    TRISGbits.TRISG14 = OUTPUT; // set led2 as output STOP
    
 //   TRISDbits.TRISD0 = OUTPUT; // set led1 as output RUN
//    TRISDbits.TRISD1 = OUTPUT; // set led2 as output STOP
    
    //turn off all LEDs
    LATGbits.LATG12 = 0; 
    LATGbits.LATG14 = 0; 
//    LATDbits.LATD0 = 0; 
//    LATDbits.LATD1 = 0; 
}

void turnOnLED(int led)
{
    switch (led)  {
            case 1: //turn on LED1
                LATGbits.LATG12 = 1;
                LATGbits.LATG14 = 0;
//                LATDbits.LATD2 = 0;
                break;
            case 2: //turn on LED2
                LATGbits.LATG12 = 0;
                LATGbits.LATG14 = 1;
//                LATDbits.LATD2 = 0;
                break;
//            LATDbits.LATD2 = 1;
    }
}