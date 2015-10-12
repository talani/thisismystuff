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

    TRISDbits.TRISD13 = INPUT;           // Configure switch as input
    CNCONDbits.ON = 1;                  // Enable overall interrupt
    CNENDbits.CNIED13 = 1;         // Enable pin CN
    CNPUDbits.CNPUD13 = 1;         // Enable pull-up resistor
    IFS1bits.CNDIF = 0;                 // Put down the flag
    //IPC8bits.CNIP = 7;                  // Configure interrupt priority
    //IPC8bits.CNIS = 4;                  // Configure the interrupt sub-priority
    IEC1bits.CNDIE = 1;           // Enable interrupt for D pins
    
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

    //Enable pull up on COLs
//    CNPUCbits.CNPUC14 = 1;
//    CNPUCbits.CNPUC2 = 1;
//    CNPUCbits.CNPUC4 = 1;
    
    
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
    
    //Disable all ROWs 
    ROW1 = DISABLE;
    ROW2 = DISABLE;
    ROW3 = DISABLE;
    ROW4 = DISABLE;
    
    //Check ROW1 only
    ROW1 = ENABLE;
    delayMs(1); //Is this too long?
    //Check the COLs
    if(COL1 == PRESSED)
    {
        numberOfPresses += 1;
        key = '1';
    }
    else if (COL2 == PRESSED)
    {
        numberOfPresses += 1;
        key = '2';
    }
    else if(COL3 == PRESSED)
    {
        numberOfPresses += 1;
        key = '3';
    }
    
    ROW1 = DISABLE;
    ROW2 = ENABLE;
    delayMs(1);
    //Check COLs
    if(COL1 == PRESSED)
    {
        numberOfPresses += 1;
        key = '4';
    }
    else if (COL2 == PRESSED)
    {
        numberOfPresses += 1;
        key = '5';
    }
    else if(COL3 == PRESSED)
    {
        numberOfPresses += 1;
        key = '6';
    }
    
    ROW2 = DISABLE;
    ROW3 = ENABLE;
    delayMs(1);
    //Check COLs
    if(COL1 == PRESSED)
    {
        numberOfPresses += 1;
        key = '7';
    }
    else if (COL2 == PRESSED)
    {
        numberOfPresses += 1;
        key = '8';
    }
    else if(COL3 == PRESSED)
    {
        numberOfPresses += 1;
        key = '9';
    }
    
    ROW3 = DISABLE;
    ROW4 = ENABLE;
    delayMs(1);
    //Check COLs
    if(COL1 == PRESSED)
    {
        numberOfPresses += 1;
        key = '*';
    }
    else if (COL2 == PRESSED)
    {
        numberOfPresses += 1;
        key = '0';
    }
    else if(COL3 == PRESSED)
    {
        numberOfPresses += 1;
        key = '#';
    }   

    if(numberOfPresses == 1)
    {
        return key;
    }
    else
    {
        return -1;
    }
}

