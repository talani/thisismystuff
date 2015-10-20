/* 
 * File:   main.c
 * Author: Tammy
 *
 * Created on October 18, 2015, 2:02 PM
 */
#include <sys/attribs.h>
#include <xc.h>
#include <stdio.h>
#include <stdlib.h>
#include "adc.h"
#include "interrupt.h"
#include "lcd.h"
#include "config.h"
#include "timer.h"
#include "pwm.h"

//volatile unsigned int val = 0;
int main(void)
{
    SYSTEMConfigPerformance(40000000);
    initLCD();
    
    enableInterrupts();
    //initADC(); // :(
    moveCursorLCD(0,1);
    printStringLCD("HELLO");
    
    while(1)
    {
        
    }
    return 0;
}


void __ISR(_ADC_VECTOR, IPL7AUTO) _ADCInterrupt(void)
{
    IFS0bits.AD1IF = 0;
    //val = ADC1BUF0;
}