/* 
 * File:   pwm.h
 * Author: Tamara, Hannah, Alisha, Ashley
 *
 * Created on October 18, 2015, 2:02 PM
 */
#ifndef PWM_H
#define	PWM_H

void initPWM();

#define SWITCH PORTDbits.RD7

//FORWARD
#define PIN1 RPD11Rbits.RPD11R //OC4 RIGHT
#define PIN2 RPB5Rbits.RPB5R //OC3 LEFT

//when 01, IT GOES REVERSE
//when 10,, IT GOES FORWARD
#define LEFTMOTORDIRECTION1 LATDbits.LATD2 //left
#define LEFTMOTORDIRECTION2 LATCbits.LATC4 //left

#define RIGHTMOTORDIRECTION1 LATGbits.LATG15 //right todo find 2 free pins
#define RIGHTMOTORDIRECTION2  LATGbits.LATG12//right 

#define LEFTWHEEL OC3RS //mapped to... TODO
#define RIGHTWHEEL OC4RS //mapped to RD1

#define INPUT 1

#endif	/* PWM_H */