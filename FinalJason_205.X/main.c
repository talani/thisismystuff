/* 
 * File:   main.c
 * Author: Tamara, Hannah, Alisha, Ashley
 *
 * Created on November 9, 2015, 2:02 PM
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

#define THRESHOLD 500
//if value < 500 = BLACK
//if value > 500 = WHITE
#define timeToTurn 10 //ms

//FWD                 
    //LEFTMOTORDIRECTION1 = 1; 
    //LEFTMOTORDIRECTION2 = 0;         
    //RIGHTMOTORDIRECTION1 = 1;
    //RIGHTMOTORDIRECTION2 = 0;
//BCKWRD                 
    //LEFTMOTORDIRECTION1 = 0; 
    //LEFTMOTORDIRECTION2 = 1;         
    //RIGHTMOTORDIRECTION1 = 0;
    //RIGHTMOTORDIRECTION2 = 1;

//PhotoTransistors
//Leftmost PIN7 J10 --> RE5 AN22
//Middle PIN33 J10 --> AN8
//Rightmost PIN49 J10 --> RB12 AN12

typedef enum stateTypeEnum{
    idle, findLine, moveRight, moveLeft, moveFwd
} stateType;

typedef enum endTypeEnum{
    state0, state1, state2, state3, state4, state5, state6
} endType;

typedef enum actionTypeEnum{
    followLine_detectEnd, turnAroundJason
}actionType;

volatile unsigned int adcVal0 = 0; //pot
volatile unsigned int adcVal1 = 0; //right
volatile unsigned int adcVal2 = 0; //middle
volatile unsigned int adcVal3 = 0; //left

//logic values for the three transistors
volatile int L1 = 0; //right
volatile int L2 = 0; //middle
volatile int L3 = 0; //left

volatile stateType currState = idle;
volatile endType end = state0;
volatile actionType jason = followLine_detectEnd;

char str[16];

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
       startRead(0); //starts reading from ADC (POT)
       adcVal0 = waitToFinish0(); //save digital value of pot
       
       startRead(1); //starts reading from ADC (Leftmost)
       adcVal1 = waitToFinish1(); //save digital value of leftmost phototransistor
       
       startRead(2); //starts reading from ADC (Middle)
       adcVal2 = waitToFinish2(); //save digital value of middle phototransistor
       
       startRead(3); //starts reading from ADC (Rightmost)
       adcVal3 = waitToFinish3(); //save digital value of rightmost phototransistor
       
       moveCursorLCD(0,1);
       sprintf(str, "%d", adcVal0); 
       printStringLCD(str);
       
       moveCursorLCD(0,2);
       sprintf(str, "%d,%d,%d", adcVal3/10, adcVal2/10, adcVal1/10);
       printStringLCD(str);
       
       scan(); //scan three transistors
       determineState();
       
       switch(jason)
       {
           case followLine_detectEnd:
               followLine();
               detectEnd();
               break;
           case turnAroundJason:
               turnAround();
               break;      
       }
       
   }
    return 0;
}

scan()
{
    if(adcVal3 < THRESHOLD) //left transistor detected black
    {
        L3 = 1;
    }
    else L3 = 0; //left transistor detected NOT black
    
    if(adcVal2 < THRESHOLD) //middle transistor detected black
    {
        L2 = 1;
    }
    else L2 = 0; //middle transistor detected NOT black
    
    if(adcVal1 < THRESHOLD) //right transistor detected black
    {
        L1 = 1;
    }
    else L1 = 0; //right transistor detected NOT black
}

determineState()
{
       if(L3==0 && L2==0 && L1==0) //000 find the line, no black was detected
       {
           currState = findLine;
       }
       else if(L3==0 && L2==0 && L1==1) //move right
       {
           currState = moveRight;
       }
       else if(L3==0 && L2==1 && L1==0) //move forward
       {
           currState = moveFwd;
       }
       else if(L3==0 && L2==1 && L1==1) //move right
       {
           currState = moveRight;
       }
       else if(L3==1 && L2==0 && L1==0) //move left
       {
           currState = moveLeft;
       }
       else if(L3==1 && L2==0 && L1==1) //shouldn't occur
       {
           currState = moveFwd;
       } 
       else if(L3==1 && L2==1 && L1==0) //move left
       {
           currState = moveLeft;
       }
       else if(L3==1 && L2==1 && L1==1) //shouldn't occur
       {
           currState = moveFwd;
       }
}

followLine()
{
    switch(currState)
       {
           case idle:
                //turn off the motors
                LEFTMOTORDIRECTION1 = 0; 
                LEFTMOTORDIRECTION2 = 0; 
                RIGHTMOTORDIRECTION1 = 0;
                RIGHTMOTORDIRECTION2 = 0;
               break;
           case findLine:
               //figure 8
               break;
           case moveRight:
               // Turn off right wheel
               // Turn on left wheel at full power
                LEFTMOTORDIRECTION1 = 1; 
                LEFTMOTORDIRECTION2 = 0;         
                RIGHTMOTORDIRECTION1 = 1;
                RIGHTMOTORDIRECTION2 = 0;
                OC4RS = 0; //RIGHT
                OC3RS = 1023; //LEFT
               break;
           case moveLeft:
               // Turn off left wheel
               // Turn on right wheel at full power
                LEFTMOTORDIRECTION1 = 1; 
                LEFTMOTORDIRECTION2 = 0;         
                RIGHTMOTORDIRECTION1 = 1;
                RIGHTMOTORDIRECTION2 = 0;
                OC4RS = 1023; //RIGHT
                OC3RS = 0; //LEFT
               break;
           case moveFwd:
               // Turn on both motors at full power
                LEFTMOTORDIRECTION1 = 1; 
                LEFTMOTORDIRECTION2 = 0;         
                RIGHTMOTORDIRECTION1 = 1;
                RIGHTMOTORDIRECTION2 = 0;
                OC4RS = 1023; //RIGHT
                OC3RS = 1023; //LEFT
               break;      
       }
}

detectEnd()
{
    switch(end)
    {
        case state0:
            //start here, wait for sequence 010
            if(L3==0 && L2==1 && L1==0)
            {
                end = state1;
            }
            break;
        case state1:
            //we have detected 010, wait to detect 111
            if(L3==1 && L2==1 && L1==1)
            {
                end = state2;
            }
            break;
        case state2:
            //we have detected 111, wait to detect 010 again
            if(L3==0 && L2==1 && L1==0)
            {
                end = state3;
            }
            break;
        case state3:
            //we have detected 010, wait to detect 111 again
            if(L3==1 && L2==1 && L1==1)
            {
                end = state4;
            }
            break;
        case state4:
            //we have detected 111, wait to detect 010 again
            if(L3==0 && L2==1 && L1==0)
            {
                end = state5;
            }
            break;
        case state5:
            //we have detected 010, wait to detect the last 111 sequence
            if(L3==1 && L2==1 && L1==1)
            {
                end = state6;
            }
            break;
        case state6:
            //we have detected the last 111
            //we are done with the track, must turn around and follow line
            end = state0; //reset
            jason = turnAroundJason;
            break;
    }
}

turnAround()
{
    //Make right wheel turn FWD
    RIGHTMOTORDIRECTION1 = 1;
    RIGHTMOTORDIRECTION2 = 0;
    //Make left wheel turn BCKWD
    LEFTMOTORDIRECTION1 = 0; 
    LEFTMOTORDIRECTION2 = 1; 
            
    OC4RS = 1023; //RIGHT full power
    OC3RS = 1023; //LEFT full power
    //delay for how long it takes for it to turn 180degrees
    delayMs(timeToTurn);
    jason = followLine_detectEnd;
}