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

volatile unsigned int Dval = 0;
volatile float Aval = 0;
char str[16];
char str2[16];
int main(void)
{
    SYSTEMConfigPerformance(40000000);
    initLCD();
    enableInterrupts();
    initADC(); //
    initPWM();
    initTimer3();
    
   while(1)
    {
        startRead();
        Dval = waitToFinish();
        OC2RS = Dval;
        Aval = (float)Dval * (3.3/1023.0); //get analog voltage value from digital value
        sprintf(str, "%1.5f", Aval);
        moveCursorLCD(0,1);
        printStringLCD(str);
    }
    return 0;
}


//void __ISR(_ADC_VECTOR, IPL7AUTO) _ADCInterrupt(void)
//{
//    IEC0bits.AD1IE = 0;
////    IFS0bits.AD1IF = 0;
////    TRISDbits.TRISD0 = 0;
////    TRISDbits.TRISD1 = 0;
////    TRISDbits.TRISD2 = 0;
////    LATD += 1;
//    Dval = ADC1BUF0; //digital value
//}