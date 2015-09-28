/* 
 * File:   main.c
 * Author: gvanhoy
 * Description: lab 0.
 * Created on August 27, 2015, 10:14 AM
 */

#include <xc.h>
#include <sys/attribs.h>
#include "switch.h"
#include "timer.h"
#include "led.h"
#include "interrupt.h"
#include "config.h"

//states for FSM
typedef enum stateTypeEnum{
    wait, wait2, debouncePress, debounceRelease, debounceRelease2
} stateType;

//states for LED
typedef enum stateLEDEnum{
    led1, led2, led3, off
} stateLED;

volatile stateType state=wait;
volatile stateLED LEDstate=off;

volatile int seconds; //to count how long switch is pressed

//Switch statement to keep track of state of LED during operation
//Updates the LED forward for switch presses < 2 seconds
void updateLEDStateFWD()
{
    switch (LEDstate)
    {
        case off:
            turnOnLED(1); //turn on first led
            LEDstate = led1;
            break;
        case led1:
            turnOnLED(2); //turn on second led
            LEDstate = led2;
            break;
        case led2:
            turnOnLED(3); //turn on third led
            LEDstate = led3;
            break;
        case led3:
            turnOnLED(1);
            LEDstate = led1;
            break;    
    }
}

//Switch statement to keep track of state of LED during operation
//Updates the LED backward for switch presses >= 2 seconds
void updateLEDStateBCKWRD()
{
        switch (LEDstate)
    {
        case off:
            turnOnLED(1); //turn on first led
            LEDstate = led1;
            break;
        case led1:
            turnOnLED(3); //turn on second led
            LEDstate = led3;
            break;
        case led2:
            turnOnLED(1); //turn on third led
            LEDstate = led1;
            break;
        case led3:
            turnOnLED(2);
            LEDstate = led2;
            break;    
    }
}

//Interrupt Service Routine for Timer1
//Timer1 - period = 1sec
void __ISR(_TIMER_1_VECTOR, ipl3SRS) Timer1Handler()
{ 
    IFS0bits.T1IF = 0; //put flag down
    seconds++; //increment seconds
}

//Interrupt Service Routine for Timer2
//Timer2 used for delay
void __ISR(_TIMER_2_VECTOR, IPL4SRS) _T2Interrupt()
{
    IFS0bits.T2IF = 0; //put flag down
}

int main() 
{ 
    enableInterrupts();
    initSwitch1(); 
    initLEDs();
    initTimer2();
    initTimer1();
    
    while(1)
    {
        //switch statement for FSM
        switch(state) 
        {
            case wait:
                if(PORTDbits.RD6 == 0)//switch 1 is pressed
                {
                    state = debouncePress;
                    T1CONbits.ON=0; //turn off timer
                    TMR1=0; //reset the timer
                }
                break;
            case debouncePress:
                delayMs(100); //delay the program for 100ms 
                seconds=0; //reset seconds counter
                T1CONbits.ON=1; //turn on timer
                state=wait2;
                break;
            case wait2:
                while(PORTDbits.RD6==0) //switch1 is pressed
                {
                    state=wait2; //stay in current state
                }
                if(PORTDbits.RD6 == 1)//switch1 is releaseds
                {
                    T1CONbits.ON=0; //turn off timer
                    if(seconds<2)
                    {   
                        state = debounceRelease;
                    }
                    else  //seconds >=2
                    {
                        state = debounceRelease2;
                    }
                }
                break;
            case debounceRelease:
                delayMs(100); //delay program for 100ms
                state=wait;
                updateLEDStateFWD(); //change the state of the LED to move forward
                break;
            case debounceRelease2:
                delayMs(100); //delay program for 100ms
                state=wait;
                updateLEDStateBCKWRD(); //change the state of the LED to move backward
                break;
        }
    }
    return 0;
}

