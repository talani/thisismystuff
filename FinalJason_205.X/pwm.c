/* 
 * File:   pwm.c
 * Author: Tamara, Hannah, Alisha, Ashley
 *
 * Created on October 18, 2015, 2:02 PM
 */

#include <sys/attribs.h>
#include <xc.h>
#include "pwm.h"


void initPWM(){
    
    //init switch RD6
    TRISDbits.TRISD7 = INPUT;
    CNPUDbits.CNPUD7 = 1; //enable internal pull-up resistor
    
    TRISDbits.TRISD11 = 0; //output Forward right ENABLE FOR LEFT WHEEL
    TRISBbits.TRISB5 = 0; //output Forward left ENABLE FOR RIGHT WHEEL
    
    TRISDbits.TRISD2 = 0; //output Backward left CONTROL LEFT WHEEL
    TRISCbits.TRISC4 = 0; //output Backward right CONTROL LEFT WHEEL
    TRISGbits.TRISG15 = 0; //CONTROL RIGHT WHEEL
    TRISGbits.TRISG12 = 0; //CONTROL RIGHT WHEEL
    
    
    RPD11Rbits.RPD11R = 0b1011; // maps OC4 RD11 FWD J10 pin 15 RIGHT ENABLE FOR LEFT WHEEL
    //RPC4Rbits.RPC4R = 0b1011; //another mapping for OC4 to RC4 for BCKWD J10 pin 21 RIGHT
    
    RPB5Rbits.RPB5R = 0b1011; // maps OC3 to RB5 FWD J10 pin 29 LEFT ENABLE FOR RIGHT WHEEL
    //RPD2Rbits.RPD2R = 0b1011; //Another mapping for OC3 to RD2 for BCKWD J11 pin 17 LEFT
    
    OC4CON = 0x0000; // Turn off OC4 while doing setup.
    OC4CONbits.OCTSEL = 1; //Use Timer 3
    OC4R = 0x0000; // Initialize primary Compare Register
    
    OC3CON = 0x0000; // Turn off OC3 while doing setup.
    OC3CONbits.OCTSEL = 1; //Use Timer 3
    OC3R = 0x0000; // Initialize primary Compare Register
    
    OC4RS = 0x0000; // Initialize secondary Compare Register RIGHT WHEEL
    OC3RS = 0x0000; //left wheel
    
    OC4CON = 0x0006; // Configure for PWM mode
    OC4CONSET = 0x8000; // Enable OC4
    
    OC3CON = 0x0006; // Configure for PWM mode
    OC3CONSET = 0x8000; // Enable OC3
}

