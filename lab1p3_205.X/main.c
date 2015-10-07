#include <xc.h>
#include <sys/attribs.h>
#include "leds.h"
#include "lcd.h"
#include "interrupt.h"
#include "switch.h"
#include "timer.h"
#include "config.h"

#define RESET PORTDbits.RD13 //reset button
#define BUTTON PORTAbits.RA7 //run&stop button
#define PRESSED 0
#define RELEASED 1

//specifies the states for my FSM
typedef enum stateTypeEnum
{
    waitForPress, debouncePress, waitForRelease, debounceRelease, debouncePressReset
}stateType;

//specifies whether I'm running or stopping
typedef enum operationEnum
{
    WAITING, RUNNING, STOPPED
}operation;

volatile stateType currState;
volatile int resetFlag; //flag to tell me if reset button was pressed
volatile operation op;
volatile int count; //this increments to tell us the time on line 2 of the LCD

int main(void)
{
    SYSTEMConfigPerformance(80000000);
    
    initLEDs();
    initSW3(); //reset switch
    initSW2(); //start&stop switch
    initLCD();
    initTimer1();
    op = WAITING;
    resetFlag = 0;
    count = 0;
    currState = waitForPress;
    
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
            case debouncePressReset:
                delayUs(50);
                resetFlag = 1; //checked later to reset the time
                currState=waitForRelease;
                break;
                
            case waitForRelease:
                if(RESET==RELEASED || BUTTON==RELEASED)
                {
                    currState=debounceRelease;
                }
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
                        op = RUNNING;
                        turnOnLED(1);
                        moveCursorLCD(0,1);
                        printStringLCD("RUNNING! ");
                    }
                    else if(resetFlag == 1) //we pushed the reset button while the first line read STOPPED
                    {
                        //reset button was pressed, so we want to display STOPPED, clear the counter
                        count = 0;
                        op = STOPPED;
                        turnOnLED(2);
                        resetFlag = 0;
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
    int dummy1 = BUTTON; //start stop
    int dummy2 = RESET; //start stop
    if(RESET == PRESSED)
    {
        currState=debouncePressReset;
    }
    if(BUTTON == PRESSED)
    {
        currState=debouncePress;
        resetFlag = 0;
    }

    IFS1bits.CNDIF = 0; //put flag down for D pins 
    IFS1bits.CNAIF = 0; //put flag down for A pins 
}

void __ISR(_TIMER_1_VECTOR, ipl7SRS) Timer1Handler()
{  
    moveCursorLCD(0, 2);
    printStringLCD(getTimeString(count));
    if(op == RUNNING)
    {
        count++; //increment count to display on LCD whenever we are running
    }
    IFS0bits.T1IF = 0; //put flag down
}
