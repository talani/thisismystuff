#include <xc.h>
#include <sys/attribs.h>
#include "leds.h"
#include "lcd.h"
#include "interrupt.h"
#include "switch.h"
#include "timer.h"
#include "config.h"

typedef enum stateTypeEnum
{
    waitForPress, debouncePress, waitForRelease, debounceRelease
}stateType;

typedef enum operationEnum
{
    WAITING, RUNNING, STOPPED
}operation;

volatile stateType currState = waitForPress;
volatile int resetFlag = 0; //flag to tell me if reset button was pressed
volatile operation op = WAITING;
volatile int count = 0; //this increments to tell us the time on line 2 of the LCD

int main(void)
{
    SYSTEMConfigPerformance(40000000);
    
    initLEDs();
    initSW3(); //reset switch
    initSW2(); //start&stop switch
    initLCD();
    initTimer1();
    
    enableInterrupts(); 
    
    while(1)
    {
        switch(currState)
        {
            case waitForPress: //start state
                //wait for Change Notification to do its thang
                //wait for press here..
                break;
            case debouncePress:
                delayUs(50);
                currState = waitForRelease;
                break;
            case waitForRelease:
                //wait for change notification
                break;
            case debounceRelease: //will enter here if PORT reads 1
                delayUs(50);
              
                if(op == WAITING) //we check this after a press, so it it was waiting, we want it to run
                {
                    op = RUNNING;
                    turnOnLED(1);
                    moveCursorLCD(0, 1); //line 1
                    printStringLCD("RUNNING "); //LCD displays running
                }
                else if(op == RUNNING) //we check this after a press, so if it was running and we hit the button, we want to stop
                {
                    op = STOPPED;
                    turnOnLED(2);
                    moveCursorLCD(0,1); //line 1
                    printStringLCD("STOPPED "); //LCD displays stopped
                }
                else if(op == STOPPED)//we check this after a press, so if it was stopped and we hit the button, we want to run or reset
                {
                    if(resetFlag == 0) //we pushed a button and it wasn't the reset button
                        //we hit the start/stop button, 
                    {
                        op == RUNNING;
                        turnOnLED(1);
                        moveCursorLCD(0,1);
                        printStringLCD("RUNNING ");
                    }
                    if(resetFlag == 1) //we pushed the reset button while the first line read STOPPED
                    {
                        //reset button was pressed, so we want to display STOPPED, clear the counter
                        //when we are here, the timer is off, so the second line of the LCD isn't updating
                        op == STOPPED;
                        count = 0; //reset the count variable, this number is what displays on the second line of the LCD whenever the ISR for timer 1 vector is triggered.
                        turnOnLED(2);
                        clearLCD();
                        moveCursorLCD(0,1); //line 1
                        printStringLCD("STOPPED "); //LCD displays stopped
                        moveCursorLCD(0,2);
                        printStringLCD("00:00:00"); //hard code this because the timer is off and it only updates when the timer is on
                    }
                }
                currState = waitForPress;
                break;       
        }
    }
    return 0;
}

void __ISR(_CHANGE_NOTICE_VECTOR, IPL7SRS) _CNInterrupt(void)
{   
    //int dummy1 = PORTDbits.RD7; //start stop
    //int dummy2 = PORTDbits.RD6; //reset
    
    if(CNSTATDbits.CNSTATD7 == 1) //start stop
    {
        int dummy = PORTDbits.RD7;
    }

    if(CNSTATDbits.CNSTATD6 == 1) //reset
    {
        int dummy2 = PORTDbits.RD6;
    }

    if(PORTDbits.RD6 == 0) //pressed reset switch
    {
        resetFlag = 1; //the reset button was pressed
        currState = debouncePress;
        //IFS1bits.CNDIF = 0;
    }
    else if(PORTDbits.RD6 == 1) //released reset switch
    {
        resetFlag = 1; //the reset button was pressed and released
        currState = debounceRelease;
        //IFS1bits.CNDIF = 0;
    }
    
    //checks if the start/stop switch was pressed or released
    if(PORTDbits.RD7==0)//pressed
    {
        resetFlag = 0; //reset button is not pressed or released
        currState = debouncePress;
        //IFS1bits.CNAIF = 0;
    }
    else if(PORTDbits.RD7==1)//released
    {
        resetFlag = 0; //reset button is not pressed or released
        currState = debounceRelease;
        //IFS1bits.CNAIF = 0;
    }
   
    //IFS1bits.CNAIF = 0; //put flag down for start/stop button
    IFS1bits.CNDIF = 0; //put flag down for reset button
}

void __ISR(_TIMER_1_VECTOR, ipl7SRS) Timer1Handler()
{  
    moveCursorLCD(0,2);
//    printStringLCD(getTimeString(count));
    if(op == RUNNING)
    {
        count++;
    }
    IFS0bits.T1IF = 0; //put flag down
}

//
//// ******************************************************************************************* //
////
//// File:         lab1p1.c
//// Date:         
//// Authors:      
////
//// Description: Part 1 for lab 1
//// ******************************************************************************************* //
//#include <xc.h>
//#include <sys/attribs.h>
//#include "leds.h"
//#include "lcd.h"
//#include "interrupt.h"
//#include "switch.h"
//#include "timer.h"
//#include "config.h"
//
///* Please note that the configuration file has changed from lab 0.
// * the oscillator is now of a different frequency.
// */
//
//typedef enum stateTypeEnum
//{
//    wait, debouncePress, waitForRelease, debounceRelease
//}stateType;
//
//typedef enum LEDStateEnum
//{
//    RUN, STOP, WAIT2
//}LEDState;
//
////idle=nothing, risingEdge=button pressed, fallingEdge=button released
//typedef enum {Idle, risingEdge, fallingEdge} buttonPress;
//
//volatile buttonPress buttonState=Idle;
//volatile stateType currentState=wait;
//volatile LEDState currentLED=WAIT2;
//volatile int count=0;
//volatile int resetFlag=0;
//
//int main(void)
//{
//    SYSTEMConfigPerformance(40000000);
//    resetFlag=0;
//    
//    initLEDs();
//    initSW3(); //reset switch
//    initSW2(); //start&stop switch
//    initLCD();
//    initTimer1();
//    
//    enableInterrupts();
//    
//    while(1)
//    {
//        switch (currentState)
//        {
//            case wait:
//                if(buttonState==risingEdge)//button is pressed
//                {
//                    if(resetFlag==1)
//                    {
//                       // T1CONbits.ON=0;
//                       // TMR1=0;
//                        count=0;
//                    }
//                    currentState=debouncePress;
//                    buttonState=Idle;
//                }
//                break;
//            case debouncePress:
//                delayUs(100);
//                currentState=waitForRelease;
//                break;
//            case waitForRelease:
//                if(buttonState==fallingEdge)//button released
//                {
//                    currentState=debounceRelease;
//                    buttonState=Idle;
//                }
//                break;
//            case debounceRelease:
//                delayUs(100);
//                if(currentLED == WAIT2)
//                {
//                    turnOnLED(1);
//                    moveCursorLCD(0, 0); //line 1
//                    printStringLCD("RUNNING "); //LCD displays running
//                    T1CONbits.ON=1;
//                    currentLED=RUN;
//                }
//                else if(currentLED == RUN)
//                {
//                    turnOnLED(2);
//                    moveCursorLCD(0, 0); //line 1
//                    printStringLCD("STOPPED "); //LCD displays running
//                    currentLED=STOP;
//                    T1CONbits.ON=0;
//                }
//                else if(currentLED == STOP)
//                {
//                    if(resetFlag==1)
//                    {
//                       // count=0;
//                        resetFlag=0;
//                        T1CONbits.ON=1;
//                    }
//                    else
//                    {
//                        turnOnLED(1);
//                        moveCursorLCD(0, 0); //line 1
//                        printStringLCD("RUNNING "); //LCD displays running
//                        currentLED=RUN;
//                        T1CONbits.ON=1;
//                    }
//                }
//                currentState=wait;
//                break;
//        } 
//    } 
//    return 0;
//}
//
////if currentLED=STOP and someone presses the other switch, then count=0;
//
//void __ISR(_CHANGE_NOTICE_VECTOR, IPL7SRS) _CNInterrupt(void)
//{   
//    
//    if(CNSTATAbits.CNSTATA7 == 1)
//    {
//        int dummy = PORTAbits.RA7;
//    }
//
//    if(CNSTATDbits.CNSTATD6 == 1)
//    {
//        int dummy2 = PORTDbits.RD6;
//    }
//
//    //checks if the run&stop reset was pressed or released
//    //pressed =0
//    if(PORTDbits.RD6 == 0) //pressed reset switch
//    {
//        resetFlag=1;
//        LATDbits.LATD0=1;
//        buttonState=risingEdge;
//    }
//    else if(PORTDbits.RD6 == 1) //released reset switch
//    {
//        resetFlag=1;
//        buttonState=fallingEdge;  
//    }
//    
//    //checks if the start/stop switch was pressed or released
//    if(PORTAbits.RA7==0)//pressed
//    {
//        resetFlag=0;
//        buttonState=risingEdge;
//    }
//    else if(PORTAbits.RA7==1)//released
//    {
//        resetFlag=0;
//        buttonState=fallingEdge;
//    }
//    IFS1bits.CNAIF = 0;
//    IFS1bits.CNDIF = 0;
//}
//
//void __ISR(_TIMER_1_VECTOR, ipl7SRS) Timer1Handler()
//{  
//    moveCursorLCD(0,2);
//    printStringLCD(getTimeString(count));
//    if(currentLED==RUN)
//    {
//        count++;
//    }
//    IFS0bits.T1IF = 0; //put flag down
//}
