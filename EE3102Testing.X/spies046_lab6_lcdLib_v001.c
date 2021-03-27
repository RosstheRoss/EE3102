/*
 * File:   spies046_lab6_lcdLib_v001.c
 * Author: micha
 *
 * Created on April 14, 2020, 6:19 PM
 */


#include "xc.h"
#include "spies046_lab6_asm_v001.h"

volatile char con1 = 0x70;
volatile char con2 = 0x54;
volatile int strLength = 0;
#define CONTRAST 0b00011111

void delay(int msecs) {           //Delay for 1ms times the input msecs
    int i = 0;           
    while(i < msecs) {
        mbs_wait_1ms();
        i++;
    }
    return;
}

void contrastMask(char conTot) {        //Masking the contrast values into the LCD initalization commands
    con1 |= (conTot & 0x0F);
    con2 |= (conTot >> 4) & 0x03;
}

void lcd_cmd(char cmd) {
    I2C1CONbits.SEN = 1;       //Initialize start condition
    while (I2C1CONbits.SEN == 1); //Wait until start bit is low again

    _MI2C1IF = 0;
    I2C1TRN = 0b01111100;           //Write slave address of LCD
    while (IFS1bits.MI2C1IF == 0);    //Wait until I2C1 interrupt flag is set
    
    _MI2C1IF = 0;                //Reset I2C1 Flag
    I2C1TRN = 0b00000000;           //Write control byte to LCD
    while (IFS1bits.MI2C1IF == 0);     //Wait until I2C1 interrupt flag is set
    
    _MI2C1IF = 0;                //Reset I2C1 Flag
    I2C1TRN = cmd;              //Write package value to LCD
    while (IFS1bits.MI2C1IF == 0);     //Wait until I2C1 interrupt flag is set
    
    _MI2C1IF = 0;                //Reset I2C1 Flag
    I2C1CONbits.PEN = 1;           //Initialize stop condition
    while (I2C1CONbits.PEN == 1);    //Wait until stop bit is low again
}

void lcd_init(void) {
    contrastMask(CONTRAST);
    
    delay(50);
    lcd_cmd(0b00111000); // function set, normal instruction mode
    lcd_cmd(0b00111001); // function set, extended instruction mode
    lcd_cmd(0b00010100); // interval osc
    lcd_cmd(con1);       // contrast C3-C0
    lcd_cmd(con2);       // Ion, Bon, C5-C4
    lcd_cmd(0b01101100); // follower control
    
    delay(200);
    lcd_cmd(0b00111000); // function set, normal instruction mode
    lcd_cmd(0b00001100); // Display On
    lcd_cmd(0b00000001); // Clear Display
    
    delay(2);
}

void lcd_setCursor(char x, char y) {
    lcd_cmd(((0x40 * y) + x) | 0x80);    //Shift and mask row and column values to set cursor
}                                            // col can be 0-39, row can be 0-1

void lcd_printChar(char myChar) {
    I2C1CONbits.SEN = 1;        //Initialize start condition
    while (I2C1CONbits.SEN == 1);

    _MI2C1IF = 0;           
    I2C1TRN = 0b01111100;           //Write slave address to LCD  
    while (IFS1bits.MI2C1IF == 0);   
    
    _MI2C1IF = 0;           
    I2C1TRN = 0b01000000;            //Write control value to LCD w/ Co=0 (only one being written)
    while (IFS1bits.MI2C1IF == 0);   
    
    _MI2C1IF = 0;             
    I2C1TRN = myChar;              //Write character value to LCD
    while (IFS1bits.MI2C1IF == 0);    
    
    _MI2C1IF = 0;           
    I2C1CONbits.PEN = 1;           //Initialize stop condition
    while (I2C1CONbits.PEN == 1);   
}

void lcd_printStr(const char s[]) {
    int index = 0;
    
    I2C1CONbits.SEN = 1;         //Initialize start condition
    while (I2C1CONbits.SEN == 1); 

    _MI2C1IF = 0;               
    I2C1TRN = 0b01111100;           //Write slave address to LCD
    while (IFS1bits.MI2C1IF == 0);    
    
    while (index < 8) {     //Only writing string length, not whole buffer length
        
        _MI2C1IF = 0;
        if (index < 7) {     //writing control byte to LCD
            I2C1TRN = 0b11000000;       //Co=1 if the byte being written is not the last one
        } else {
            I2C1TRN = 0b01000000;       //Co=0 if byte being written is the last one
        }
        while (IFS1bits.MI2C1IF == 0);

        
        _MI2C1IF = 0;                
        I2C1TRN = s[index];              //Write character value to LCD
        while (IFS1bits.MI2C1IF == 0);   
        
        index++;
    }

    _MI2C1IF = 0;              
    I2C1CONbits.PEN = 1;          //Initialize stop condition
    while (I2C1CONbits.PEN == 1);  
}

void lcd_ShiftL(const char s[]) {
    lcd_cmd(0b00011000);    //Shift display and cursor to the left once
    lcd_cmd(0b00000001);    //Clear the display
    lcd_printStr(s);        //Reprint string on LCD in new position
}

void findLength(const char s[]) {
    int index = 0;
    for (index = 0; s[index] != '\n'; index++) {  //Increment strLength if character in string is not \n
        strLength++;                              //to find the total length of the string
    }
}
