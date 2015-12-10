/* 
 * File:   adc.h
 * Author: Tamara, Hannah, Alisha, Ashley
 *
 * Created on October 18, 2015, 2:02 PM
 */
#ifndef ADC_H
#define	ADC_H

#define TRIPOT TRISBbits.TRISB0 //potentiometer define
#define INPUT 1 //input defined as 1

void initADC(); //initialize the ADC
void startRead(); //read from buffer
int waitToFinish(); //wait for read to finish

#endif	/* AC_H */