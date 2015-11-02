/* 
 * File:   main.c
 * Author: Tamara, Hannah, Alisha, Ashley
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
    FWD, BCKWD, debouncePress, waitForRelease, debounceRelease, StartState, waitForPress, STOP
} stateType;

typedef enum directionEnum{
    Forward1, Forward2, Backward2, Start, Backward1
} direction;

volatile direction dir = Start; 
volatile stateType currState = StartState;

volatile unsigned int Dval = 0; //digital value
volatile float temp =0; //temporary variable for updating the speed of the motor
volatile float Aval = 0; //analog value
char str[16]; //string to print the analog value to line one of the LCD
int main(void)
{
    SYSTEMConfigPerformance(40000000);
    initLCD(); //initialize the LCD
    enableInterrupts();
    initADC(); //initialize the ADC
    initPWM(); //initialize the PWM
    initTimer3(); //initialize the timer
    
   while(1)
    {
        startRead(); //starts reading from ADC
        Dval = waitToFinish(); //save digital value
        
        Aval = (float)Dval * (3.3/1023.0); //get analog voltage value from digital value
        sprintf(str, "%1.5f", Aval); //save analog value to string
        moveCursorLCD(0,1); //move to the first line for printing on the LCD
        printStringLCD(str); //print the analog value to lcd
        switch(currState)
        {
            case StartState:
                //turn off the motors
                LEFTMOTORDIRECTION1 = 0; 
                LEFTMOTORDIRECTION2 = 0; 
                RIGHTMOTORDIRECTION1 = 0;
                RIGHTMOTORDIRECTION2 = 0;
                currState = waitForPress;
                break;
            case waitForPress: //waits for switch press
                //we don't want to move the motors in between states
                //when dir is start or forward2 or backward2, the motors are stopped
                if(dir != Start && dir != Forward2 && dir != Backward2)
                {
                    updateSpeed();
                }
                if(SWITCH == 0) //pressed
                {
                    currState = debouncePress;
                }
                break;
            case debouncePress:
                delayUs(50); //delay for debounce
                currState = waitForRelease;
                break;
            case waitForRelease:
                if(SWITCH == 1) //released
                {
                    currState = debounceRelease;
                }
                break;
            case debounceRelease:
                delayUs(50); //delay for debounce
                updateDirectionState(); //function to update the direction of the motor
                break;
            case STOP: //stop all motors
                LEFTMOTORDIRECTION1 = 0; 
                LEFTMOTORDIRECTION2 = 0; 
                RIGHTMOTORDIRECTION1 = 0;
                RIGHTMOTORDIRECTION2 = 0;
                currState = waitForPress;
                break;
            case FWD: //move motors forward
                LEFTMOTORDIRECTION1 = 1; 
                LEFTMOTORDIRECTION2 = 0;         
                RIGHTMOTORDIRECTION1 = 1;
                RIGHTMOTORDIRECTION2 = 0;
                currState = waitForPress;
                break;
            case BCKWD: //move motors backward
                LEFTMOTORDIRECTION1 = 0; 
                LEFTMOTORDIRECTION2 = 1;        
                RIGHTMOTORDIRECTION1 = 0;
                RIGHTMOTORDIRECTION2 = 1;
                currState = waitForPress;
                break;
        }    
    }
    return 0;
}
//uses the dir variable to control the direction of the motors.
//if we want to reverse the direction of the motor, we want to stop first and then reverse.
///if we want to move forward from the reverse direction, we want to stop first and then move forward
updateDirectionState()
{
    if(dir == Start) //this is the first press of the program, we want to go forward
    {
        currState = FWD;
        dir = Forward1;
    }
    else if(dir == Forward1) //we were going forward, we pressed the button, so now we want to stop
    {
        currState = STOP;
        dir = Forward2;
    }
    else if(dir == Forward2) //we were stopped, now we want to go backward
    {
        currState = BCKWD;
        dir = Backward1;
    }
    else if(dir == Backward1) //we were going backward, we pressed the button, so now we want to stop
    {
        currState = STOP;
        dir = Backward2;
    }
    else if(dir == Backward2) //we were stopped, now we want to go forward
    {
        currState = FWD;
        dir = Forward1;
    }
}
//Use the Dval variable to control the speed of the motors. 
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
        //between 1.613V and 1.6903V
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
