/* 
 * File:   adc.c
 * Author: Tamara, Hannah, Alisha, Ashley
 *
 * Created on October 18, 2015, 2:02 PM
 */

#include <sys/attribs.h>
#include <xc.h>
#include "adc.h"

//PhotoTransistors
//Leftmost PIN7 J10 --> RE5 AN22
//Middle PIN33 J10 --> AN8
//Rightmost PIN49 J10 --> RB12 AN12

void initADC()
{
    ANSELBbits.ANSB0 = 0; 
    ANSELEbits.ANSE5 = 0;
    ANSELBbits.ANSB12 = 0;
    TRIPOT = INPUT; //input
    AD1CON1bits.FORM = 0; // 16 unsigned integer
    AD1CON1bits.SSRC = 7; // Auto-convert mode
    AD1CON1bits.ASAM = 0; //turn off auto-sampling
    AD1CON2bits.VCFG = 0; // Use board refernece voltages
    AD1CON2bits.CSCNA = 0; // Disable scanning
    AD1CON2bits.SMPI = 0; // 1 burrito
    AD1CON2bits.ALTS = 0; // Only Mux A
    AD1CON3bits.ADRC = 0; // Use PBCLK
    AD1CON3bits.SAMC = 2; // 2 Tad per sample
    AD1CON3bits.ADCS = 1; // 4 times the PBCLK
    AD1CHSbits.CH0NA = 0; // Use Vref- as negative reference
    
    IFS0bits.AD1IF = 0; // Put down ADC flag
    IPC5bits.AD1IP = 7;
    IEC0bits.AD1IE = 0;
    
    AD1CON1bits.SAMP = 0;
    AD1CON1bits.DONE = 1;
    
    AD1CON1bits.ADON = 1; // turn on the ADC
}

void startRead(int x)
{
    if (x == 0)
    {
        AD1CHSbits.CH0SA = 0; // Scan AN0
    }
    else if (x == 1)
    {
        AD1CHSbits.CH0SA = 22; // Scan AN22 Leftmost
    }
    else if (x == 2)
    {
        AD1CHSbits.CH0SA = 8; // Scan AN8 Middle
    }
    else if (x == 3)
    {
        AD1CHSbits.CH0SA = 12; // Scan AN12 Rightmost
    }
    AD1CON1bits.SAMP = 1;
    AD1CON1bits.DONE = 1;
}

int waitToFinish0() //pot
{
    int adcVal0=0;
    while(AD1CON1bits.DONE == 0);
    adcVal0 = ADC1BUF0;
    AD1CON1bits.SAMP = 0; //sample to convert
    return adcVal0;
}

int waitToFinish1() //leftmost phototransistor
{
    int adcVal1=0;
    while(AD1CON1bits.DONE == 0);
    adcVal1 = ADC1BUF0;
    AD1CON1bits.SAMP = 0; //sample to convert
    return adcVal1;
}

int waitToFinish2() //middle phototransistor
{
    int adcVal2=0;
    while(AD1CON1bits.DONE == 0);
    adcVal2 = ADC1BUF0;
    AD1CON1bits.SAMP = 0; //sample to convert
    return adcVal2;
}

int waitToFinish3() //rightmost phototransistor
{
    int adcVal3=0;
    while(AD1CON1bits.DONE == 0);
    adcVal3 = ADC1BUF0;
    AD1CON1bits.SAMP = 0; //sample to convert
    return adcVal3;
}

