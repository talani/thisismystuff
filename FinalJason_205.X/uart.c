/* 
 * File:   uart.c
 * Author: Tamara, Hannah, Alisha, Ashley
 *
 * Created on October 18, 2015, 2:02 PM
 */
#include <sys/attribs.h>
#include <xc.h>
#include "uart.h"

void initUART()
{
    
    RPF5Rbits.RPF5R = 0b0001; //remap TX
    TRISFbits.TRISF5 = 0; //set TX as output
    
    
    U2RXRbits.U2RXR = 0b0010; //remap RX
    TRISFbits.TRISF4 = 1; //set RX as input
    
    //set all TX modes
    U2MODEbits.ON = 0;
    U2MODEbits.SIDL = 0;
    U2MODEbits.IREN = 0;
    U2MODEbits.RTSMD = 1;
    U2MODEbits.UEN = 0;
    U2MODEbits.WAKE = 0;
    U2MODEbits.LPBACK = 0;
    U2MODEbits.ABAUD = 0;
    U2MODEbits.RXINV = 0;
    U2MODEbits.BRGH = 0;
    U2MODEbits.PDSEL = 0;
    U2MODEbits.STSEL = 0;
    
    U2STAbits.ADM_EN = 0;
    U2STAbits.UTXINV = 0;
    U2STAbits.UTXBRK = 0;
    
    U2STAbits.URXISEL = 0;
    
    U2BRG = 52;//(8000000.0/(16.0*9600.0)) - 1.0;
   
    IEC1bits.U2TXIE = 0;
    IEC1bits.U2RXIE = 0; //disable the interrupt
    IFS1bits.U2RXIF = 0; // Put the flag down
    IFS1bits.U2TXIF = 0; // Put the flag down
    IPC9bits.U2IP = 3;  // Configure the Interrupt Priority
    
    U2STAbits.URXEN = 1; 
    U2STAbits.UTXEN = 1;
    U2MODEbits.ON = 1;
}

void sendByte(char c)
{
    U2TXREG = c;
    U2STAbits.UTXEN = 1;
    U2MODEbits.ON = 1;
    while(U2STAbits.TRMT == 0);
}