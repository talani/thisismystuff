/* 
 * File:   adc.h
 * Author: Tamara, Hannah, Alisha, Ashley
 *
 * Created on October 18, 2015, 2:02 PM
 */
#ifndef ADC_H
#define	ADC_H

#define TRIPOT TRISBbits.TRISB0
#define INPUT 1

void initADC();
//void checkInterrupt();
void startRead();
int waitToFinish();

#endif	/* AC_H */