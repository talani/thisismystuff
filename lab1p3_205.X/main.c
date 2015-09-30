// ******************************************************************************************* //
//
// File:         lab1p1.c
// Date:         
// Authors:      
//
// Description: Part 1 for lab 1
// ******************************************************************************************* //

#include "leds.h"
#include "interrupt.h"
#include "switch.h"
#include "timer.h"
#include "config.h"

/* Please note that the configuration file has changed from lab 0.
 * the oscillator is now of a different frequency.
 */

typedef enum stateTypeEnum
{
    wait, debouncePress, waitForRelease, debounceRelease
}stateType;

typedef enum LEDStateEnum
{
    RUN, STOP, WAIT
}LEDState;

//idle=nothing, risingEdge=button pressed, fallingEdge=button released
typedef enum {Idle, risingEdge, fallingEdge} buttonPress;

volatile buttonPress buttonState=Idle;
volatile stateType currentState=wait;
volatile LEDState currentLED=WAIT;
volatile int count=0;

updateLEDState()
{   
    switch (currentLED)
    {
            case RUN: //turn on led1, turn off led2
                turnOnLED(1);
                moveCursorLCD(0, 0); //line 1
                printStringLCD("RUNNING "); //LCD displays running
                count++;
                moveCursorLCD(0,2);
                //printStringLCD("what");
                printStringLCD(getTimeString(count));
                //currentLED=STOP;
            break;
            case STOP: //turn off led1, turn on led2
                moveCursorLCD(0, 0); //line 1
                printStringLCD("STOPPED "); //LCD displays stopped
                turnOnLED(2);
                //currentLED=RUN;
            break;
            case WAIT:
                initLCD();
                break;
    }
}


int main(void)
{
    //SYSTEMConfigPerformance(40000000);

    initLEDs();
    initSW2();
    //initTimer1();
    updateLEDState();
    enableInterrupts();
    
  
    
    while(1)
    {
        switch (currentState)
        {
            case wait:
                if(buttonState==risingEdge)//button is pressed
                {
                    currentState=debouncePress;
                    //T1CONbits.ON=0;
                    //TMR1=0;
                    buttonState=Idle;
                }
                break;
            case debouncePress:
                delayUs(50);
                T1CONbits.ON=1;
                currentState=waitForRelease;
                break;
            case waitForRelease:
                if(buttonState==fallingEdge)//button released
                {
                    currentState=debounceRelease;
                    buttonState=Idle;
                }
                break;
            case debounceRelease:
                delayUs(50);
                if(currentLED==WAIT)
                {
                    currentLED=RUN;
                }
                else if(currentLED==RUN)
                {
                    currentLED=STOP;
                }
                else if(currentLED==STOP)
                {
                    currentLED=RUN;
                }
                updateLEDState();
                currentState=wait;
                break;
        } 
    } 
    return 0;
}

void __ISR(_CHANGE_NOTICE_VECTOR, IPL2SRS) _CNInterrupt(void)
{   
    int dummy = PORTAbits.RA7;
    
    if(PORTAbits.RA7==0)//pressed
    {
        buttonState=risingEdge;
    }
    else if(PORTAbits.RA7==1)//released
    {
        buttonState=fallingEdge;
    }
    IFS1bits.CNAIF = 0;
}

void __ISR(_TIMER_1_VECTOR, ipl3SRS) Timer1Handler()
{ 
    IFS0bits.T1IF = 0; //put flag down
    if(currentLED==RUN)
    {
        count++;
    }
}