/* 
 * File:   led.c
 * Author: gvanhoy
 *
 * Created on August 27, 2015, 3:15 PM
 */

//#include <xc.h>
//#include <sys/attribs.h>
#include "led.h"
#define OUTPUT 0
#define INPUT 1

void initLEDs(){
    
    //TODO: Initialize LEDs
    TRISDbits.TRISD0 = OUTPUT; //led1
    TRISDbits.TRISD1 = OUTPUT; //led2
    TRISDbits.TRISD2 = OUTPUT; //led3
    
    LATDbits.LATD0 = 0; //off
    LATDbits.LATD1 = 0; //off
    LATDbits.LATD2 = 0; //off
}

void turnOnLED(int led){
    //LATD=0;//turn off all leds
    //TODO: You may choose to write this function
    // as a matter of convenience
    switch (led)  {
            case 1:
                LATDbits.LATD0 = 1;
                LATDbits.LATD1 = 0;
                LATDbits.LATD2 = 0;
                break;
            case 2:
                LATDbits.LATD0 = 0;
                LATDbits.LATD1 = 1;
                LATDbits.LATD2 = 0;
                break;
            case 3:
                LATDbits.LATD0 = 0;
                LATDbits.LATD1 = 0;
                LATDbits.LATD2 = 1;
                break;
    }
}