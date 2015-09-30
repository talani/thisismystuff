/*
* File:   leds.c
* Author: 
*
* Created on December 27, 2014, 1:31 PM
*/


//0 turns on led
//1 turns led off
//because of hardware
#include "leds.h"

#define OUTPUT 0
#define INPUT 1

void initLEDs(){
    TRISGbits.TRISG12 = OUTPUT; // set led1 as output RUN  TRD1
    TRISGbits.TRISG14 = OUTPUT; // set led2 as output STOP  TRD2
    
    //turn off all LEDs
    LATGbits.LATG12 = 1; //LED1 TRD1
    LATGbits.LATG14 = 1;  //LED2 TRD2
}

void turnOnLED(int led)
{
    switch (led)  {
            case 1: //turn on LED1
                LATGbits.LATG12 = 0; //0=ON
                LATGbits.LATG14 = 1;//1=OFF
                break;
            case 2: //turn on LED2
                LATGbits.LATG12 = 1;
                LATGbits.LATG14 = 0;
                break;
    }
}
