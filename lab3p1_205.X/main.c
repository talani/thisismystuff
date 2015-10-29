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

typedef enum stateTypeEnum{
    FWD, idle1, BCKWD, idle2
} stateType;

volatile stateType currState = FWD;
volatile stateType nextState;

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
        
//        if(PORTDbits.RD6 == 0) //pressed FWD
//        {
//            PIN3 = 0;
//            PIN4 = 0;
//            OC4RS = Dval;
//            OC3RS = Dval;  
//        }
//        else if (PORTDbits.RD6 == 1) //released BCKWARD
//        {
//            PIN1 = 0;
//            PIN2 = 0;
//            OC4RS = Dval;
//            OC3RS = Dval;  
//        }
        //01 REVERSE
        //10 FORWARD
        LEFTMOTORDIRECTION1 = 1; 
        LEFTMOTORDIRECTION2 = 0; 
        
        RIGHTMOTORDIRECTION1 = 1;
        RIGHTMOTORDIRECTION2 = 0;
        
        OC4RS = Dval; //RIGHT
        OC3RS = Dval; //LEFT
        
        
        
        Aval = (float)Dval * (3.3/1023.0); //get analog voltage value from digital value
        sprintf(str, "%1.5f", Aval);
        moveCursorLCD(0,1);
        printStringLCD(str);
        moveCursorLCD(0,2);
        sprintf(str, "%d,%d", PR3, OC3RS);
        printStringLCD(str);
        
        
//        switch(currState)
//        {
//            case FWD:
////                OC3RS = 10;
////                OC4RS = 10;
//                break;
//            case idle1:
//                break;
//            case BCKWD:
//                break;
//            case idle2:
//                break;     
//        }
        
    }
    return 0;
}

//change notification
//{
//    if button is pressed, currState=nextState;
//}

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