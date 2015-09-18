/* 
 * File:   switch.c
 * Author: gvanhoy
 *
 * Created on August 27, 2015, 3:12 PM
 */

#include "switch.h"
#define OUTPUT 0
#define INPUT 1

void initSwitch1()
{
    TRISDbits.TRISD6 = INPUT; //set switch1 as input
    CNPUDbits.CNPUD6 = 1; //enable internal pull-up resistor
}