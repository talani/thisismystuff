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
    waitForPress, debouncePress, waitForRelease, debounceRelease, debounceRelease2, scan, printLCD, checkCursor
}stateType;

volatile stateType currState = waitForPress;
volatile char keyToPrint = NULL;

int main(void)
{
    SYSTEMConfigPerformance(80000000);
    initKeypad();
    enableInterrupts();
    initLCD();
    
    int cursor = 0; //cursor count to make sure I print on the right line
    moveCursorLCD(0,1); //move cursor to beginning of line 1
    
    while(1)
    {
        switch(currState)
        {
            case waitForPress:
                //wait for change notification to do its thing
                break;
            case debouncePress:
                delayMs(5);
                currState = scan;
                break;
            case scan: //scans the press to get the correct key to print
                keyToPrint = scanKeypad();
                currState = waitForRelease;
                break;
            case waitForRelease: //wait for change notification to do its thing
                break;
            case debounceRelease:
                delayMs(5);
                if(keyToPrint != '\0') currState = printLCD;
                else currState = waitForPress;
                break;
            case debounceRelease2:
                delayMs(5);
                keyToPrint = '\0';
                currState = waitForPress;
                break;
            case printLCD:
                printCharLCD(keyToPrint);
                currState = checkCursor;
                break;
            case checkCursor:
                if(cursor == 7) //go to next line
                {
                    moveCursorLCD(0,2); 
                    cursor++; //increments the count laterally on the LCD
                }
                else if(cursor == 15) //go back to first line to overwrite
                {
                    moveCursorLCD(0,1); //start at position 0 of line 1
                    cursor = 0; //reset the count
                }
                else cursor++; //increments the count laterally on the LCD
                currState = waitForPress;
                break;  
        }
    }
    return 0;
}


void __ISR(_CHANGE_NOTICE_VECTOR, IPL7SRS) _CNInterrupt(void)
{
    //must read the columns before I can do anything because Microchip is stupid
    int dummy1 = COL1;
    int dummy2 = COL2;
    int dummy3 = COL3;

    if (keyToPrint == -1) //this means more than one key was pressed
    {
        currState = debounceRelease2;
    }
    else if(COL1 == PRESSED || COL2 == PRESSED || COL3 == PRESSED) 
    {
        currState = debouncePress;
    }
    else currState = debounceRelease;
    
    IFS1bits.CNCIF = 0; //put flag down for E pins 
}
