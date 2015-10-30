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
volatile float temp =0;
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
    
   while(1)
    {
        startRead();
        Dval = waitToFinish();
        
        Aval = (float)Dval * (3.3/1023.0); //get analog voltage value from digital value
        sprintf(str, "%1.5f", Aval);
        moveCursorLCD(0,1);
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
    }
    else if(dir == Backward)
    {
        LEFTMOTORDIRECTION1 = 0; 
        LEFTMOTORDIRECTION2 = 0; 
        RIGHTMOTORDIRECTION1 = 0;
        RIGHTMOTORDIRECTION2 = 0;
        currState = BCKWD;
        dir = Forward;
    }
}

updateSpeed()
{
    if(Dval == 0)
    {
        //turn on left motor at full power
        OC4RS = 0; //RIGHT
        OC3RS = 1023; //LEFT
        
    }
    else if(Dval == 1023)
    {
        //turn on right motor at full power
        OC4RS = 1023; //RIGHT
        OC3RS = 0; //LEFT
    }
    else if(Dval >=500 && Dval <=524)
    {
        //turn on both motors at full power
        OC4RS = 1023; //RIGHT
        OC3RS = 1023; //LEFT
    }
    else if(Dval>0 && Dval<500)
    {
        //right wheel is full power
        OC4RS = 1023; //RIGHT
        //left wheel is fraction
        temp = Dval/499;
        OC3RS = Dval*temp; //LEFT
    }
    else if(Dval>524 && Dval<1023)
    {
        //left wheel is full power
        OC3RS = 1023; //LEFT
        //right wheel is fraction
        temp = Dval-524;
        temp = temp/499;
        OC4RS = Dval*temp; //RIGHT
    }
}

