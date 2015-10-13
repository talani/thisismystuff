/* 
 * File:   keypad.h
 * Author: user
 *
 * Created on February 5, 2015, 11:42 AM
 */

#ifndef KEYPAD_H
#define	KEYPAD_H

//TRISTATE for columns
#define TRISCOL2 TRISCbits.TRISC14
#define TRISCOL1 TRISCbits.TRISC2
#define TRISCOL3 TRISCbits.TRISC4

//TRISTATE for rows
#define TRISROW1 TRISGbits.TRISG15
#define TRISROW4 TRISGbits.TRISG12
#define TRISROW3 TRISDbits.TRISD5
#define TRISROW2 TRISDbits.TRISD11

//PORT to read columns
#define COL2 PORTCbits.RC14
#define COL1 PORTCbits.RC2
#define COL3 PORTCbits.RC4

//LAT to write to rows
#define ROW1 LATGbits.LATG15
#define ROW4 LATGbits.LATG12
#define ROW3 LATDbits.LATD5
#define ROW2 LATDbits.LATD11

//PORT to read rows
#define ROW1PORT PORTGbits.RG15
#define ROW4PORT PORTGbits.RG12
#define ROW3PORT PORTDbits.RD5
#define ROW2PORT PORTDbits.RD11

//ODC bits for rows
#define ODCROW1 ODCGbits.ODCG15
#define ODCROW4 ODCGbits.ODCG12
#define ODCROW3 ODCDbits.ODCD5
#define ODCROW2 ODCDbits.ODCD11

#define OUTPUT 0
#define INPUT 1

#define ENABLE 0 //power
#define DISABLE 1 //NO POWER

#define PRESSED 0
#define RELEASED 1

void initKeypad(void);
char scanKeypad(void);

#endif	/* KEYPAD_H */

