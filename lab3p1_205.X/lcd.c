/*
 * File:   lcd.c
 */
#include <sys/attribs.h>
#include <xc.h>
#include "lcd.h"
#include "timer.h"

#define LCD_D7 LATDbits.LATD12
#define LCD_D6 LATDbits.LATD6
#define LCD_D5 LATDbits.LATD3
#define LCD_D4 LATDbits.LATD1

#define LCD_RS LATFbits.LATF1
#define LCD_E LATDbits.LATD9
#define LCD_RW LATGbits.LATG0


#define TRIS_D7 TRISDbits.TRISD12
#define TRIS_D6 TRISDbits.TRISD6
#define TRIS_D5 TRISDbits.TRISD3
#define TRIS_D4 TRISDbits.TRISD1

#define TRIS_RS TRISFbits.TRISF1
#define TRIS_E  TRISDbits.TRISD9
#define TRIS_RW TRISGbits.TRISG0

#define OUTPUT 0
#define INPUT 1

/* This function should take in a two-byte word and writes either the lower or upper
 * byte to the last four bits of LATB. Additionally, according to the LCD data sheet
 * It should set LCD_RS and LCD_E with the appropriate values and delays.
 * After these commands are issued, there should be a delay.
 * The command type is a simplification. From the data sheet, the RS is '1'
 * when you are simply writing a character. Otherwise, RS is '0'.
 */
void writeFourBits(unsigned char word, unsigned int commandType, unsigned int delayAfter, unsigned int lower){
    //TODO:
    if(lower==0) //upper
    {
        word = word>>4;
    }
    //command type=RS
    if(commandType==1)//we are writing
    {
        LCD_RS=1;
        delayUs(1);
    }
    else LCD_RS=0;
    delayUs(1);
    LCD_D4 = word & 0x01; //and word with 1 to get LSB
    LCD_D5 = (word>>1) & 0x01;
    LCD_D6 = (word>>2) & 0x01;
    LCD_D7 = (word>>3) & 0x01;
    
    
    LCD_E = 1; //set enable pin high
    delayUs(1);
    LCD_E = 0; //set enable pin low
    delayUs(1);
    delayUs(delayAfter);

}

/* Using writeFourBits, this function should write the two bytes of a character
 * to the LCD.
 */
void writeLCD(unsigned char word, unsigned int commandType, unsigned int delayAfter){
    //TODO:
    writeFourBits(word, commandType, delayAfter, 0); //upper
    writeFourBits(word, commandType, delayAfter, 1); //lower
}

/* Given a character, write it to the LCD. RS should be set to the appropriate value.
 */
void printCharLCD(char c) {
    //TODO:
    writeLCD(c, 1, 46);
}
/*Initialize the LCD
 */
void initLCD(void) {
    // Setup D, RS, and E to be outputs (0).
    TRIS_RS = OUTPUT;
    TRIS_E = OUTPUT;
    TRIS_D7 = OUTPUT;
    TRIS_D6 = OUTPUT;
    TRIS_D5 = OUTPUT;
    TRIS_D4 = OUTPUT;
    TRIS_RW = OUTPUT;
    LCD_RW=0; //Can ground this since we are not reading, or we can make it  in software

    // Initilization sequence utilizes specific LCD commands before the general configuration
    // commands can be utilized. The first few initilition commands cannot be done using the
    // WriteLCD function. Additionally, the specific sequence and timing is very important.

    
    //turn LCD off
    //writeFourBits(0,046,)
    
    // Enable 4-bit interface
    delayUs(1000); //delay 15ms after VDD reaches 4.5V
    delayUs(1000);
    delayUs(1000);
    delayUs(1000);
    delayUs(1000);
    delayUs(1000);
    delayUs(1000);
    delayUs(1000);
    delayUs(1000);
    delayUs(1000);
    delayUs(1000);
    delayUs(1000);
    delayUs(1000);
    delayUs(1000);
    delayUs(1000);
    
    //writeFourBits( word, unsigned commandType, delayAfter, lower)
    writeFourBits(0x03, 0, 46, 1); //000011
    delayUs(4100); //wait 4.1ms
    writeFourBits(0x03, 0, 46, 1); //000011
    delayUs(100); //wait 100us
    writeFourBits(0x03, 0, 46, 1); //000011
    writeFourBits(0x02, 0, 46, 1); //000010
    
    //if 0x28 doesn't work, try 0x2C
    writeLCD(0x2B, 0, 46);
    writeLCD(0x08, 0, 46);
    
    clearLCD();
   // writeLCD(0x01, 0, clearLCD());
    writeLCD(0x06, 0, 46);
    writeLCD(0x0C, 0, 46);
    

    // Function Set (specifies data width, lines, and font.

    // 4-bit mode initialization is complete. We can now configure the various LCD
    // options to control how the LCD will function.

    // TODO: Display On/Off Control
        // Turn Display (D) Off
    // TODO: Clear Display (The delay is not specified in the data sheet at this point. You really need to have the clear display delay here.
    // TODO: Entry Mode Set
        // Set Increment Display, No Shift (i.e. cursor move)
    // TODO: Display On/Off Control
        // Turn Display (D) On, Cursor (C) Off, and Blink(B) Off
}

/*
 * You can use printCharLCD here. Note that every time you write a character
 * the cursor increments its position automatically.
 * Since a string is just a character array, try to be clever with your use of pointers.
 */
void printStringLCD(const char* s) {
    //TODO:
    while(*s != '\0')
    {
        printCharLCD(*(s++));
        
    }
    
}

/*
 * Clear the display.
 */
void clearLCD(){
    writeLCD(0x01, 0, 1640);
}

/*
 Use the command for changing the DD RAM address to put the cursor somewhere.
 */
void moveCursorLCD(unsigned char x, unsigned char y){
    unsigned char location = 0x80;
    
    location = location +x;
    if (y==2)
    {
        location = location + 0x40;
    }
    
    writeLCD(location, 0, 40);
}

/*
 * This function is called in lab1p2.c for testing purposes.
 * If everything is working properly, you should get this to look like the video on D2L
 * However, it is suggested that you test more than just this one function.
 */
void testLCD(){
    initLCD();
    int i = 0;
    printCharLCD('c');
    for(i = 0; i < 1000; i++) delayUs(1000);
    clearLCD();
    printStringLCD("Hello!");
    moveCursorLCD(1, 2);
    for(i = 0; i < 1000; i++) delayUs(1000);
    printStringLCD("Hello!");
    for(i = 0; i < 1000; i++) delayUs(1000);
}
