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
    //use SW1 RD6 for start/stop
    //TODO: change this to external switch TRD0/RG13
    TRISAbits.TRISA7 = INPUT; //TRD3/RA7
    
    //change notification interrupt
    CNCONAbits.ON = 1;                  // Enable overall interrupt
    CNENAbits.CNIEA7 = ENABLED;         // Enable pin CN
    CNPUAbits.CNPUA7 = ENABLED;         // Enable pull-up resistor
    IFS1bits.CNAIF = 0;                 // Put down the flag
    IPC8bits.CNIP = 2;                  // Configure interrupt priority
    IPC8bits.CNIS = 3;                  // Configure the interrupt sub-priority
    IEC1bits.CNAIE = ENABLED;           // Enable interrupt for D pin
}
