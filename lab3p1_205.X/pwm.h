//Tamara Alani
#ifndef PWM_H
#define	PWM_H

void initPWM();

//FORWARD
#define PIN1 RPD11Rbits.RPD11R //OC4 RIGHT
#define PIN2 RPB5Rbits.RPB5R //OC3 LEFT

//#define LATPIN1 LATDbits.LATD11 //right forward
//#define LATPIN2 LATBbits.LATB5 //left forward

//BACKWARD
//#define PIN3 RPD2Rbits.RPD2R //OC3 LEFT
//#define PIN4 RPC4Rbits.RPC4R //OC4 RIGHT

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