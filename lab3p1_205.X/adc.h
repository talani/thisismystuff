//tamara alani

#ifndef ADC_H
#define	ADC_H

#define TRIPOT TRISBbits.TRISB0
#define INPUT 1

void initADC();
//void checkInterrupt();
void startRead();
int waitToFinish();

#endif	/* AC_H */