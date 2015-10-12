/* 
 * File:   keypad.h
 * Author: user
 *
 * Created on February 5, 2015, 11:42 AM
 */

#ifndef KEYPAD_H
#define	KEYPAD_H

#define TRISCOL1 TRISCbits.TRISC14
#define TRISCOL2 TRISCbits.TRISC2
#define TRISCOL3 TRISCbits.TRISC4

#define TRISROW1 TRISEbits.TRISE7
#define TRISROW2 TRISEbits.TRISE5
#define TRISROW3 TRISEbits.TRISE3
#define TRISROW4 TRISEbits.TRISE1

#define COL1 PORTCbits.RC14
#define COL2 PORTCbits.RC2
#define COL3 PORTCbits.RC4

#define ROW1 LATEbits.LATE7
#define ROW2 LATEbits.LATE5
#define ROW3 LATEbits.LATE3
#define ROW4 LATEbits.LATE1

#define ROW1PORT PORTEbits.RE7
#define ROW2PORT PORTEbits.RE5
#define ROW3PORT PORTEbits.RE3
#define ROW4PORT PORTEbits.RE1

#define ODCROW1 ODCEbits.ODCE7
#define ODCROW2 ODCEbits.ODCE5
#define ODCROW3 ODCEbits.ODCE3
#define ODCROW4 ODCEbits.ODCE1

#define OUTPUT 0
#define INPUT 1

#define ENABLE 0
#define DISABLE 1

#define PRESSED 0
#define RELEASED 1


void initKeypad(void);
char scanKeypad(void);

#endif	/* KEYPAD_H */

