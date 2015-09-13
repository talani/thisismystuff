/* 
 * File:   switch.c
 * Author: gvanhoy
 *
 * Created on August 27, 2015, 3:12 PM
 */

//#include <xc.h>
//#include <sys/attribs.h>
#include "switch.h"
#define OUTPUT 0
#define INPUT 1

void initSwitch1(){
    //TODO: Initialize switch 1
    TRISDbits.TRISD6 = INPUT;
   // CNCONDbits.ON = 1;          // Turn on CN device
    
//    CNENDbits.CNIED6 = 1;       // Enable CN interrupt for pin
//    IEC1bits.CNDIE = 1;         // Enable overall CN Interrupt
//    IFS1bits.CNDIF = 0;         // Put Interrupt flag down
    
    CNPUDbits.CNPUD6 = 1;       // Enable Internal Pull-up resistor
//    ANSELD=0;
}