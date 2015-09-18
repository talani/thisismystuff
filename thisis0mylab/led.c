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

void initLEDs()
{
    TRISDbits.TRISD0 = OUTPUT; // set led1 as output
    TRISDbits.TRISD1 = OUTPUT; // set led2 as output
    TRISDbits.TRISD2 = OUTPUT; // set led3 as output
    
    //turn off all LEDs
    LATDbits.LATD0 = 0; 
    LATDbits.LATD1 = 0; 
    LATDbits.LATD2 = 0; 
}

void turnOnLED(int led)
{
    switch (led)  {
            case 1: //turn on LED1
                LATDbits.LATD0 = 1;
                LATDbits.LATD1 = 0;
                LATDbits.LATD2 = 0;
                break;
            case 2: //turn on LED2
                LATDbits.LATD0 = 0;
                LATDbits.LATD1 = 1;
                LATDbits.LATD2 = 0;
                break;
            case 3: //turn on LED3
                LATDbits.LATD0 = 0;
                LATDbits.LATD1 = 0;
                LATDbits.LATD2 = 1;
                break;
    }
}