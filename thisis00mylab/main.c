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

//TODO: Define states of the state machine
typedef enum stateTypeEnum{
    wait, wait2, debouncePress, debounceRelease, debounceRelease2
} stateType;

typedef enum stateLEDEnum{
    led1, led2, led3, off
} stateLED;

volatile stateType state=wait;
volatile stateLED LEDstate=off;

volatile int seconds;

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

//TODO: Use volatile variables that change within interrupts
void __ISR(_TIMER_1_VECTOR, ipl3SRS) Timer1Handler()
//void __ISR(_TIMER_1_VECTOR, IPL3SRS) _T1Interrupt()
{ 
    IFS0bits.T1IF = 0; //put flag down
    //IFS0CLR = 0x00000010; // Be sure to clear the Timer1 interrupt status 
    seconds++;
}

void __ISR(_TIMER_2_VECTOR, IPL4SRS) _T2Interrupt()
{
    IFS0bits.T2IF = 0; //put flag down
    //IFS0CLR = 0x00000200; // Be sure to clear the Timer1 interrupt status 
}

int main() {
    //This function is necessary to use interrupts. 
    enableInterrupts();
    initSwitch1(); //done
    initLEDs(); //done
    initTimer2();
    initTimer1();
    
    while(1){
        //TMR1=0;
        //TODO: Implement a state machine to create the desired functionality
        switch(state) {
            case wait:
                if(PORTDbits.RD6 == 0)//pressed
                {
                    state = debouncePress;
                    T1CONbits.ON=0;
                    TMR1=0;
                    //T1CONbits.ON=0; //turn off timer 
                }
                break;
            case debouncePress:
                delayMs(100);
                seconds=0; 
                //TMR1=0; //reset the timer
                T1CONbits.ON=1; //turn on timer
                state=wait2;
                break;
            case wait2:
                while(PORTDbits.RD6==0)
                {
                    state=wait2;
                }
                if(PORTDbits.RD6 == 1)//released
                {
                    //state=debounceRelease;
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
                delayMs(100);
                state=wait;
                updateLEDStateFWD();
                break;
            case debounceRelease2:
                delayMs(100);
                state=wait;
                updateLEDStateBCKWRD();
                break;
        }
    }
    return 0;
}

