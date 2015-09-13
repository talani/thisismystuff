/* 
 * File:   main.c
 * Author: gvanhoy
 * Description: lab 0.
 * Created on August 27, 2015, 10:14 AM
 */

#include <xc.h>
#include <sys/attribs.h>


#define OUTPUT 0
#define INPUT 1

//TODO: Define states of the state machine
typedef enum stateTypeEnum{
    led1, led2, led3, wait, wait2, debouncePress, debounceRelease, debounceRelease2
} stateType;

//TODO: Use volatile variables that change within interrupts

int main() {
    TRISDbits.TRISD6 = 1;
    TRISDbits.TRISD0 = 0; //led1
    TRISDbits.TRISD1 = 0; //led2
    TRISDbits.TRISD2 = 0; //led3
    
    LATDbits.LATD0 = 0; //off
    LATDbits.LATD1 = 0; //off
    LATDbits.LATD2 = 0; //off
    
    while(1){
        if(PORTDbits.RD6 == 0) //pressed
        {
            LATDbits.LATD0 = 1; //off
            LATDbits.LATD1 = 0; //off
            LATDbits.LATD2 = 0; //off
        }
        if(PORTDbits.RD6 == 1)
        {
            LATDbits.LATD0 = 0; //off
            LATDbits.LATD1 = 0; //off
            LATDbits.LATD2 = 1; //off
        }
        //TODO: Implement a state machine to create the desired functionality
        
    }
    
    return 0;
}

