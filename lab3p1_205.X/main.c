/* 
 * File:   main.c
 * Author: Tammy
 *
 * Created on October 18, 2015, 2:02 PM
 */

#include <stdio.h>
#include <stdlib.h>

/*
 * 
 */
int main(void)
{
    SYSTEMConfigPerformance(80000000);
    initADC();
    initLCD();
    enableInterrupts();
    
    while(1)
    {
        
    }
    return 0;
}

