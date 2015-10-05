/*
 * File:   switch.c
 * Author: Garrett
 *
 * Created on September 19, 2015, 10:46 AM
 */
#include <xc.h>
#include <sys/attribs.h>
#include "switch.h"

#define INPUT 1
#define OUTPUT 0

#define ENABLED 1
#define DISABLED 0

//void initSW2(){
//    TRISAbits.TRISA7 = INPUT; //TRD3/RA7
//    
//    //change notification interrupt
//    CNCONAbits.ON = 1;                  // Enable overall interrupt
//    CNENAbits.CNIEA7 = ENABLED;         // Enable pin CN
//    CNPUAbits.CNPUA7 = ENABLED;         // Enable pull-up resistor
//    IFS1bits.CNAIF = 0;                 // Put down the flag
//    IPC8bits.CNIP = 7;                  // Configure interrupt priority
//  //  IPC8bits.CNIS = 3;                  // Configure the interrupt sub-priority
//    IEC1bits.CNAIE = ENABLED;           // Enable interrupt for D pin
//}



//void initSW3(){ //reset switch
//    TRISEbits.TRISE7 = INPUT; //TRD3/RA7
//    
//    //change notification interrupt
//    CNCONEbits.ON = 1;                  // Enable overall interrupt
//    CNENEbits.CNIEE7 = ENABLED;         // Enable pin CN
//    CNPUEbits.CNPUE7 = ENABLED;         // Enable pull-up resistor
//    IFS1bits.CNEIF = 0;                 // Put down the flag
//    IPC8bits.CNIP = 7;                  // Configure interrupt priority
//    //IPC8bits.CNIS = 4;                  // Configure the interrupt sub-priority
//    IEC1bits.CNEIE = ENABLED;           // Enable interrupt for D pin
//}
//
void initSW3()
{
    TRISDbits.TRISD6 = INPUT;           // Configure switch as input
    CNCONDbits.ON = 1;                  // Enable overall interrupt
    CNENDbits.CNIED6 = ENABLED;         // Enable pin CN
    CNPUDbits.CNPUD6 = ENABLED;         // Enable pull-up resistor
    IFS1bits.CNDIF = 0;                 // Put down the flag
    IPC8bits.CNIP = 7;                  // Configure interrupt priority
    //IPC8bits.CNIS = 4;                  // Configure the interrupt sub-priority
    IEC1bits.CNDIE = ENABLED;           // Enable interrupt for D pins
}

void initSW2() //using onboard switch for start/stop
{
    TRISDbits.TRISD7 = INPUT;           // Configure switch as input
    CNCONDbits.ON = 1;                  // Enable overall interrupt
    CNENDbits.CNIED7 = ENABLED;         // Enable pin CN
    CNPUDbits.CNPUD7 = ENABLED;         // Enable pull-up resistor
    IFS1bits.CNDIF = 0;                 // Put down the flag
    IPC8bits.CNIP = 7;                  // Configure interrupt priority
    //IPC8bits.CNIS = 4;                  // Configure the interrupt sub-priority
    IEC1bits.CNDIE = ENABLED;           // Enable interrupt for D pins
}

