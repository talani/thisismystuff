// ******************************************************************************************* //
// File:         main.c
// Date:         
// Authors:      
//
// Description: 
// ******************************************************************************************* //

#include <xc.h>
#include <sys/attribs.h>
#include "lcd.h"
#include "timer.h"
#include "keypad.h"
#include "config.h"
#include "interrupt.h"

#define PRESSED 0
#define RELEASED 1

typedef enum stateTypeEnum
{
    waitForPress, debouncePress, waitForRelease, debounceRelease
}stateType;

volatile stateType currState = waitForPress;
volatile char keyToPrint;
volatile int LCDrow = 1;
volatile int LCDcol = 0;
//volatile int LCDcolLINE2 = 0;

int main(void)
{
    SYSTEMConfigPerformance(80000000);
    initKeypad();
    enableInterrupts();
    initLCD();
    initTimer1();
//    
    TRISDbits.TRISD0 = 0;
    
    TRISDbits.TRISD13 = 1;           // Configure switch as input
//    CNCONDbits.ON = 1;                  // Enable overall interrupt
//    CNENDbits.CNIED13 = 1;         // Enable pin CN
    CNPUDbits.CNPUD13 = 1;         // Enable pull-up resistor
//    IFS1bits.CNDIF = 0;                 // Put down the flag
//    IPC8bits.CNIP = 7;                  // Configure interrupt priority
//    IEC1bits.CNDIE = 1;           // Enable interrupt for D pins
    
    while(1) {
        LATDbits.LATD0 = COL1;
        //LATDbits.LATD0 = PORTDbits.RD13;//
    }

    while(1);
    {
        switch(currState)
        {
            case waitForPress:
                //wait for change notification to do its thing
                break;
            case debouncePress:
                delayMs(10);
                keyToPrint = scanKeypad();
                if(keyToPrint != -1)
                {
                    //2 rows & 8 columns on LCD
                    if(LCDrow == 1 && LCDcol != 8)
                    {
                        moveCursorLCD(LCDcol++, LCDrow);
                        printStringLCD(keyToPrint);
                    }

                    //check cursor and print
                }
                currState = waitForRelease;
                break;
            case waitForRelease:
                if(COL1 == RELEASED || COL2 == RELEASED || COL3 == RELEASED)
                {
                    currState = debounceRelease;
                }
                break;
            case debounceRelease:
                delayMs(10);
                break;       
        }
        
    }
    
    return 0;
}


void __ISR(_CHANGE_NOTICE_VECTOR, IPL7SRS) _CNInterrupt(void)
{
    int dummy1 = COL1;
    int dummy2 = COL2;
    int dummy3 = COL3;
    
    int dummy5 = PORTDbits.RD13;
    
   // LATDbits.LATD0 =1;

    if(COL1 == PRESSED || COL2 == PRESSED || COL3 == PRESSED)
    {
        currState = debouncePress;
    }
    
    IFS1bits.CNCIF = 0; //put flag down for E pins 
    IFS1bits.CNDIF = 0; //put flag down for D pins 
}

void __ISR(_TIMER_1_VECTOR, ipl7SRS) Timer1Handler()
{  
    //printStringLCD(keyToPrint); //Print char on LCD
    IFS0bits.T1IF = 0; //put flag down
}