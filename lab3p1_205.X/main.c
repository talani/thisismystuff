/* 
 * File:   main.c
 * Author: Tammy
 *
 * Created on October 18, 2015, 2:02 PM
 */
#include <sys/attribs.h>
#include <xc.h>
#include <stdio.h>
#include <stdlib.h>
#include "adc.h"
#include "interrupt.h"
#include "lcd.h"
#include "config.h"
#include "timer.h"
#include "pwm.h"

typedef enum stateTypeEnum{
    FWD, idle1, BCKWD, idle2, debouncePress, updateDirection, waitForRelease, debounceRelease
} stateType;

typedef enum directionEnum{
    Forward, Backward
} direction;

volatile direction dir = Forward;
volatile stateType currState = idle1;
volatile stateType nextState;

volatile unsigned int Dval = 0;
volatile float Aval = 0;
char str[16];
char str2[16];
int main(void)
{
    SYSTEMConfigPerformance(40000000);
    initLCD();
    enableInterrupts();
    initADC(); //
    initPWM();
    initTimer3();
    TRISDbits.TRISD0 = 0;
    
   while(1)
    {
        startRead();
        Dval = waitToFinish();
        
        //01 REVERSE
        //10 FORWARD
        
        Aval = (float)Dval * (3.3/1023.0); //get analog voltage value from digital value
        sprintf(str, "%1.5f", Aval);
        moveCursorLCD(0,1);
        printStringLCD(str);
        moveCursorLCD(0,2);
        sprintf(str, "%d", Dval);
        printStringLCD(str);
        
        
        
        switch(currState)
        {
            case FWD:
                LEFTMOTORDIRECTION1 = 1; 
                LEFTMOTORDIRECTION2 = 0; 
        
                RIGHTMOTORDIRECTION1 = 1;
                RIGHTMOTORDIRECTION2 = 0;
                
                currState = idle2;
                break;
            case idle1:
                if(SWITCH == 0) //pressed
                {
                    currState = debouncePress;   
                }
                break;
            case debouncePress:
                delayUs(50);
                currState = waitForRelease;
                break;
            case waitForRelease:
                if(SWITCH == 1)
                {
                    currState = debounceRelease;
                }
                break;
            case debounceRelease:
                delayUs(50);
                currState = updateDirection;  
                break;
            case updateDirection:
                updateDirectionState();
                break;
            case BCKWD:
                LEFTMOTORDIRECTION1 = 0; 
                LEFTMOTORDIRECTION2 = 1; 
        
                RIGHTMOTORDIRECTION1 = 0;
                RIGHTMOTORDIRECTION2 = 1;

                currState = idle2;
                break;
            case idle2:
                updateSpeed();
                if(SWITCH == 0) //pressed
                {
                    currState = debouncePress;
                }
                break;     
        }
        
    }
    return 0;
}

updateDirectionState()
{
    if(dir == Forward)
    {
        LEFTMOTORDIRECTION1 = 0; 
        LEFTMOTORDIRECTION2 = 0; 
        RIGHTMOTORDIRECTION1 = 0;
        RIGHTMOTORDIRECTION2 = 0;
        currState = FWD;
        dir = Backward;
        LATDbits.LATD0 = 1;
    }
    else if(dir == Backward)
    {
        LEFTMOTORDIRECTION1 = 0; 
        LEFTMOTORDIRECTION2 = 0; 
        RIGHTMOTORDIRECTION1 = 0;
        RIGHTMOTORDIRECTION2 = 0;
        currState = BCKWD;
        dir = Forward;
        LATDbits.LATD0 = 0;
    }
}

updateSpeed()
{
    OC4RS = Dval; //RIGHT
    OC3RS = Dval; //LEFT
}


//change notification
//{
//    if button is pressed, currState=nextState;
//}

//void __ISR(_ADC_VECTOR, IPL7AUTO) _ADCInterrupt(void)
//{
//    IEC0bits.AD1IE = 0;
////    IFS0bits.AD1IF = 0;
////    TRISDbits.TRISD0 = 0;
////    TRISDbits.TRISD1 = 0;
////    TRISDbits.TRISD2 = 0;
////    LATD += 1;
//    Dval = ADC1BUF0; //digital value
//}