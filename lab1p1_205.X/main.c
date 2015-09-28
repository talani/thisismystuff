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
    RUN, STOP
}LEDState;

//idle=nothing, risingEdge=button pressed, fallingEdge=button released
typedef enum {Idle, risingEdge, fallingEdge} buttonPress;

volatile buttonPress buttonState=Idle;
volatile stateType currentState=wait;
volatile LEDState currentLED=RUN;

updateLEDState()
{   
    switch (currentLED)
    {
            case RUN: //turn on led1, turn off led2
                turnOnLED(1);
                currentLED=STOP;
            break;
            case STOP: //turn off led1, turn on led2
                turnOnLED(2);
                currentLED=RUN;
            break;
    }
}


int main(void)
{
    
    initLEDs();
    initSW2();
    initTimer2();
    updateLEDState();
    enableInterrupts();
    int x;
   
    
    while(1)
    {
        switch (currentState)
        {
            case wait:
                if(buttonState==risingEdge)//button is pressed
                {
                    currentState=debouncePress;
                    buttonState=Idle;
                }
                break;
            case debouncePress:
                delayUs(50);
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
