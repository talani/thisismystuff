/*
 * File:   switch.c
 * Author: Garrett
 *
 * Created on September 19, 2015, 10:46 AM
 */

#include "switch.h"

#define INPUT 1
#define OUTPUT 0

#define ENABLED 1
#define DISABLED 0

void initSW2(){
    //TODO: Initialize the appropriate pins to work with the external switch using a
    //change notification interrupt.
    
    //use SW1 RD6 for start/stop
    //TODO: change this to external switch TRD0/RG13
    TRISGbits.TRISG13 = INPUT;
    //TRISDbits.TRISD6=INPUT;
    
    //change notification interrupt
    CNCONGbits.ON = 1;                  // Enable overall interrupt
    CNENGbits.CNIEG13 = ENABLED;         // Enable pin CN
    CNPUGbits.CNPUG13 = ENABLED;         // Enable pull-up resistor
    IFS1bits.CNGIF = 0;                 // Put down the flag
    IPC8bits.CNIP = 2;                  // Configure interrupt priority
    IPC8bits.CNIS = 3;                  // Configure the interrupt sub-priority
    IEC1bits.CNGIE = ENABLED;           // Enable interrupt for D pins
    
}
