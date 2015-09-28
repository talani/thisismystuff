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

volatile stateType currentState=wait;
volatile LEDState currentLED=RUN;

updateLEDState()
{
    
    int j;
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
//idle=nothing, risingEdge=button pressed, fallingEdge=button released
typedef enum {Idle, risingEdge, fallingEdge} buttonPress;
volatile buttonPress buttonState=Idle;

int main(void)
{
    enableInterrupts();
    initLEDs();
    initSW2();
    initTimer2();
    updateLEDState();
                LATGbits.LATG12 = 1;
                LATGbits.LATG14 = 1;
    
    
    while(1);
    {
        switch (currentState)
        {
            case wait:
                if(buttonState==risingEdge)//button is pressed
                {
                    
                    currentState=debouncePress;
                    buttonState=Idle;
                }
//                if(IFS1bits.CNDIF==1)
//                //if(PORTDbits.RD6==0) //!!! use change notifications, not polling
//                {
//                    currentState=debouncePress;
//                }
                break;
            case debouncePress:
                delayUs(50);
                currentState=waitForRelease;
                break;
            case waitForRelease:
                //!!! use change notifications, not polling
                if(buttonState==fallingEdge)//button released
                {
                    currentState=debounceRelease;
                    buttonState=Idle;
                }
//                if(IFS1bits.CNDIF==1)
//                //if(PORTDbits.RD6==1)//released
//                {
//                    currentState=debounceRelease;
//                }
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
//void __attribute__((interrupt(), vector(_CHANGE_NOTICE_VECTOR))) CNInterrupt() 
{
    //buttonPress = PORTDbits.RD6;
    //TODO: Implement the interrupt to capture the press of the button
    
    if(PORTGbits.RG13==0)//pressed
    {
        //LATDbits.LATD2 = 1;
        buttonState=risingEdge;
        //turnOnLED(1);
//        turnOnLED(3);
        
    }
    else if(PORTGbits.RG13==1)//released
    {
       // LATDbits.LATD2 = 0;
        buttonState=fallingEdge;
//        turnOnLED(2);
    }
    IFS1bits.CNDIF = 0; 

}
