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
#include "uart.h"

#define THRESHOLD 500
//if value < 450 = BLACK
//if value > 450 = WHITE
#define timeToTurn 10 //ms
#define LW OC4RS //LEFT wheel
#define RW OC3RS //RIGHT Wheel

//PhotoTransistors
//Leftmost PIN7 J10 --> RE5 AN22
//Middle PIN33 J10 --> AN8
//Rightmost PIN49 J10 --> RB12 AN12

typedef enum stateTypeEnum{
    idle, findLine, moveRight, moveLeft, moveFwd, debouncePress, waitForRelease, debounceRelease
} stateType;

typedef enum endTypeEnum{
    state0, state1, state2, state3, state4, state5, state6
} endType;

typedef enum actionTypeEnum{
    followLine_detectEnd, turnAroundJason
}actionType;

volatile unsigned int adcVal0 = 0; //pot
volatile unsigned int adcVal1 = 0; //left
volatile unsigned int adcVal2 = 0; //middle
volatile unsigned int adcVal3 = 0; //right

volatile int flag = 0; //0=go, 1=stop

//logic values for the three transistors
volatile int L1 = 0; //left
volatile int L2 = 0; //middle
volatile int L3 = 0; //right

volatile stateType currState = idle;
volatile endType end = state0;
volatile actionType jason = followLine_detectEnd;

char str[16];

volatile char input = '\0';

int main(void)
{
    SYSTEMConfigPerformance(40000000);
    initLCD(); //initialize the LCD
    enableInterrupts();
    initADC(); //initialize the ADC
    initPWM(); //initialize the PWM
    initTimer3(); //initialize the timer
    TRISDbits.TRISD0 = 0;
    LATDbits.LATD0 = 0;
    initUART(); //initialize UART
    
    sendByte('M');
    
   while(1)
   {
        if(U2STAbits.URXDA)
        {
            input = U2RXREG; //saves RX reg buffer
            sendByte(input); //echos pressed key back to prompt
            sendByte(':'); //sends : to terminal prompt
        }
       
        switch(input) {
            case 'w': //forward
                LEFTMOTORDIRECTION1 = 0; 
                LEFTMOTORDIRECTION2 = 1;         
                RIGHTMOTORDIRECTION1 = 0;
                RIGHTMOTORDIRECTION2 = 1;
                RW = 700;
                LW = 700; 
                break;
            case 'a': //left
                LEFTMOTORDIRECTION1 = 0; 
                LEFTMOTORDIRECTION2 = 1;         
                RIGHTMOTORDIRECTION1 = 0;
                RIGHTMOTORDIRECTION2 = 1;
                RW = 700; 
                LW = 100;
                break;
            case 's': //backward
                LEFTMOTORDIRECTION1 = 1; 
                LEFTMOTORDIRECTION2 = 0;         
                RIGHTMOTORDIRECTION1 = 1;
                RIGHTMOTORDIRECTION2 = 0;
                RW = 700; 
                LW = 700;               
                break;
            case 'd': //right
                LEFTMOTORDIRECTION1 = 0; 
                LEFTMOTORDIRECTION2 = 1;         
                RIGHTMOTORDIRECTION1 = 0;
                RIGHTMOTORDIRECTION2 = 1;
                RW = 100; 
                LW = 700;                
                break;
            case 'p':
                LEFTMOTORDIRECTION1 = 0; 
                LEFTMOTORDIRECTION2 = 1;         
                RIGHTMOTORDIRECTION1 = 0;
                RIGHTMOTORDIRECTION2 = 1;
                RW = 0;
                LW = 0; 
                break;
        }
        
        
       startRead(0); //starts reading from ADC (POT)
       adcVal0 = waitToFinish0(); //save digital value of pot
       
       startRead(1); //starts reading from ADC (Leftmost)
       adcVal1 = waitToFinish1(); //save digital value of leftmost phototransistor
       
       startRead(2); //starts reading from ADC (Middle)
       adcVal2 = waitToFinish2(); //save digital value of middle phototransistor
       
       startRead(3); //starts reading from ADC (Rightmost)
       adcVal3 = waitToFinish3(); //save digital value of rightmost phototransistor
       
       moveCursorLCD(0,2);
       sprintf(str, "%d,%d,%d", adcVal1/10, adcVal2/10, adcVal3/10);
       printStringLCD(str);
       
       scan(); //scan three transistors
       
       moveCursorLCD(0,1);
       sprintf(str, "%d, %d, %d", L1, L2, L3);
       printStringLCD(str);
       
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
    if(currState==findLine || currState==moveFwd || currState==moveRight || currState==moveLeft)
    {
        
        if(adcVal1 < THRESHOLD) L1 = 1;//right transistor detected black
        else L1 = 0; //right transistor detected NOT black
        
        if(adcVal2 < THRESHOLD) L2 = 1; //middle transistor detected black
        else L2 = 0; //middle transistor detected NOT black
    
        if(adcVal3 < THRESHOLD) L3 = 1; //left transistor detected black
        else L3 = 0; //left transistor detected NOT black
    }
}

determineState()
{
    if(currState==findLine || currState==moveFwd || currState==moveRight || currState==moveLeft)
    {
       if(L1==0 && L2==0 && L3==0) //000 find the line, no black was detected
       {
           currState = findLine;
       }
       else if(L1==0 && L2==0 && L3==1) //  move right
       {
           currState = moveRight;
       }
       else if(L1==0 && L2==1 && L3==0) //move forward
       {
           currState = moveFwd;
       }
       else if(L1==0 && L2==1 && L3==1) // 011 move right
       {
           currState = moveRight;
       }
       else if(L1==1 && L2==0 && L3==0) // 100 move left
       {
           currState = moveLeft;
       }
       else if(L1==1 && L2==0 && L3==1) //shouldn't occur
       {
           currState = moveFwd;
       } 
       else if(L1==1 && L2==1 && L3==0) // 011 move left
       {
           currState = moveLeft;
       }
       else if(L1==1 && L2==1 && L3==1) //shouldn't occur
       {
           currState = moveFwd;
       }
    }
}

followLine()
{
    switch(currState)
       {
           case idle:
               flag = 0;
                //turn off the motors
                LEFTMOTORDIRECTION1 = 0; 
                LEFTMOTORDIRECTION2 = 0; 
                RIGHTMOTORDIRECTION1 = 0;
                RIGHTMOTORDIRECTION2 = 0;
                //wait for button press
                if(SWITCH == 0) //pressed
                {
                    currState = debouncePress;
                }
               break;
        case debouncePress:
            delayMs(10);
            currState = waitForRelease;
            break;
        case waitForRelease:
            if(SWITCH == 1) //released
            {
                currState = debounceRelease;
            }
            break;
        case debounceRelease:
            delayMs(10);
            if(flag == 1) currState = idle;
            else currState = findLine;
            break;
           case findLine:
               checkStop();
               //DONUT
               LEFTMOTORDIRECTION1 = 0; 
               LEFTMOTORDIRECTION2 = 1;         
               RIGHTMOTORDIRECTION1 = 0;
               RIGHTMOTORDIRECTION2 = 1;
               RW = 500;
               LW = 250; 
               break;
           case moveRight:
               checkStop();
               // Turn off right wheel
               // Turn on left wheel at full power
                LEFTMOTORDIRECTION1 = 0; 
                LEFTMOTORDIRECTION2 = 1;         
                RIGHTMOTORDIRECTION1 = 0;
                RIGHTMOTORDIRECTION2 = 1;
                RW = 100; 
                LW = 700; 
               break;
           case moveLeft:
                checkStop();
                //Turn off left wheel
                //Turn on right wheel at full power
                LEFTMOTORDIRECTION1 = 0; 
                LEFTMOTORDIRECTION2 = 1;         
                RIGHTMOTORDIRECTION1 = 0;
                RIGHTMOTORDIRECTION2 = 1;
                RW = 700; 
                LW = 100; 
               break;
           case moveFwd: 
                checkStop();
                //Turn on both motors at full power
                LEFTMOTORDIRECTION1 = 0; 
                LEFTMOTORDIRECTION2 = 1;         
                RIGHTMOTORDIRECTION1 = 0;
                RIGHTMOTORDIRECTION2 = 1;
                RW = 700;
                LW = 700; 
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
            LATDbits.LATD0 = 1;
            end = state0; //reset
            jason = turnAroundJason;
            break;
    }
}

turnAround()
{
    //Make right wheel turn FWD
    RIGHTMOTORDIRECTION1 = 0;
    RIGHTMOTORDIRECTION2 = 1;
    //Make left wheel turn BCKWD
    LEFTMOTORDIRECTION1 = 1; 
    LEFTMOTORDIRECTION2 = 0; 
            
    OC4RS = 1023; //RIGHT full power
    OC3RS = 1023; //LEFT full power
    
    //delay for how long it takes for it to turn 180degrees
    delayMs(timeToTurn);
    jason = followLine_detectEnd;
}

checkStop()
{
    if (SWITCH == 0) 
    {
        flag = 1;
        currState = debouncePress;
    }
}

//void __ISR(_TIMER_1_VECTOR, ipl3SRS) Timer1Handler()
//void __attribute__((vector(_UART_2_VECTOR), interrupt(IPL3SRS), nomips16)) UART2_ISR(void)
//void __ISR(_UART_2_VECTOR, ipl3) IntUart2Handler(void)
//void __ISR(_UART_2_VECTOR, ipl3SRS) _UART2Handler()
////void __ISR(_UART2_VECTOR, ipl3) _UART2Handler(void)
//{
//    
//    //sendByte(U2RXREG); 
//    //sendByte(test);
//    test = U2RXREG + 1;
//    //test++;
//       if (test > 'z')
//           test = 'A';
//    IFS1bits.U2RXIF = 0; // Put the flag down
//}
