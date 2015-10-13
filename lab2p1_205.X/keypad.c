#include <xc.h>
#include "keypad.h"
#include "timer.h"

/* Initialize the rows as ODC outputs and the columns as inputs with pull-up
 * resistors. Don't forget about other considerations...
 */


void initKeypad(void){
    
    CNCONCbits.ON = 1; //Enable overall Interrupt
    IFS1bits.CNCIF = 0; //Put the flag down
    IPC8bits.CNIP = 7; //Priority
    IEC1bits.CNCIE = 1; //Enable the interrupt
    
    //No Analog pins..
    
    //Set Rows to outputs
    TRISROW1 = OUTPUT; 
    TRISROW2 = OUTPUT;
    TRISROW3 = OUTPUT;
    TRISROW4 = OUTPUT;
    
    //Set Columns to inputs
    TRISCOL1 = INPUT;
    TRISCOL2 = INPUT;
    TRISCOL3 = INPUT;

    //Enable Open Drain Collectors for Rows 
    ODCROW1 = 1;
    ODCROW2 = 1;
    ODCROW3 = 1;
    ODCROW4 = 1;
    
    //Initialize ROWs to catch interrupt 
    ROW1 = ENABLE;
    ROW2 = ENABLE;
    ROW3 = ENABLE;
    ROW4 = ENABLE;
    
    //Enable the interrupt for COLs
    CNENCbits.CNIEC14 = 1;
    CNENCbits.CNIEC2 = 1;
    CNENCbits.CNIEC4 = 1;
}

/* This function will be called AFTER you have determined that someone pressed
 * SOME key. This function is to figure out WHICH key has been pressed.
 * This function should return -1 if more than one key is pressed or if
 * no key is pressed at all. Otherwise, it should return the ASCII character of
 * the key that is pressed.
 */
char scanKeypad(void){
     //initializes key to -1
    char key = -1;
    //only return the char if 1 button is pressed. Else return -1
    int numberOfPresses=0; //counts how many buttons on the keypad were pressed
    
    IEC1bits.CNCIE = 0; //Disable the interrupt
    
    //Disable all ROWs 
    ROW1 = DISABLE;
    ROW2 = DISABLE;
    ROW3 = DISABLE;
    ROW4 = DISABLE;
    
    ROW1 = ENABLE;
    delayMs(5);
    //Check the COLs
    if(COL1 == PRESSED)
    {
        numberOfPresses++;
        key = '1';
    }
    if (COL2 == PRESSED)
    {
        numberOfPresses++;
        key = '2';
    }
    if(COL3 == PRESSED)
    {
        numberOfPresses++;
        key = '3';
    }
    
    ROW1 = DISABLE; //I don't want to check ROW1 anymore
    
    ROW2 = ENABLE; //check ROW2 now
    delayMs(5);
    //Check COLs
    if(COL1 == PRESSED)
    {
        numberOfPresses++;
        key = '4';
    }
    if (COL2 == PRESSED)
    {
        numberOfPresses++;
        key = '5';
    }
    if(COL3 == PRESSED)
    {
        numberOfPresses++;
        key = '6';
    }
    
    ROW2 = DISABLE; //I don't want to check ROW2 anymore
    
    ROW3 = ENABLE; //check ROW3 now
    delayMs(5);
    //Check COLs
    if(COL1 == PRESSED)
    {
        numberOfPresses++;
        key = '7';
    }
    if (COL2 == PRESSED)
    {
        numberOfPresses++;
        key = '8';
    }
    if(COL3 == PRESSED)
    {
        numberOfPresses++;
        key = '9';
    }
    
    ROW3 = DISABLE; //I don't want to check ROW3 anymore
    
    ROW4 = ENABLE; //check ROW4 now
    delayMs(5);
    //Check COLs
    if(COL1 == PRESSED)
    {
        numberOfPresses++;
        key = '*';
    }
    if (COL2 == PRESSED)
    {
        numberOfPresses++;
        key = '0';
    }
    if(COL3 == PRESSED)
    {
        numberOfPresses++;
        key = '#';
    }   
    ROW4 = DISABLE; //I don't want to check ROW4 anymore
    
    //Enable all rows to catch interrupt
    ROW1 = ENABLE;
    ROW2 = ENABLE;
    ROW3 = ENABLE;
    ROW4 = ENABLE;
    
    IEC1bits.CNCIE = 1; //Enable the interrupt
    
    if(numberOfPresses == 1) //checks if only 1 key was pressed by user
    {
        return key; //return that key to print in FSM
    }
    else
    {
        return -1; //more than one key was pressed, return -1 to not print in FSM
    }
}

