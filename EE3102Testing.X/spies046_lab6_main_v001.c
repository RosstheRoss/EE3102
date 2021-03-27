/*
 * File:   spies046_lab6_main_v001.c
 * Author: micha
 *
 * Created on April 14, 2020, 1:51 PM
 */

#include "xc.h"
#include <stdio.h>
#include "spies046_lab6_buffer_v001.h"
#include "spies046_lab6_asm_v001.h"
#include "spies046_lab6_lcdLib_v001.h"


// CW1: FLASH CONFIGURATION WORD 1 (see PIC24 Family Reference Manual 24.1)
#pragma config ICS = PGx1          // Comm Channel Select (Emulator EMUC1/EMUD1 pins are shared with PGC1/PGD1)
#pragma config FWDTEN = OFF        // Watchdog Timer Enable (Watchdog Timer is disabled)
#pragma config GWRP = OFF          // General Code Segment Write Protect (Writes to program memory are allowed)
#pragma config GCP = OFF           // General Code Segment Code Protect (Code protection is disabled)
#pragma config JTAGEN = OFF        // JTAG Port Enable (JTAG port is disabled)


// CW2: FLASH CONFIGURATION WORD 2 (see PIC24 Family Reference Manual 24.1)
#pragma config I2C1SEL = PRI       // I2C1 Pin Location Select (Use default SCL1/SDA1 pins)
#pragma config IOL1WAY = OFF       // IOLOCK Protection (IOLOCK may be changed via unlocking seq)
#pragma config OSCIOFNC = ON       // Primary Oscillator I/O Function (CLKO/RC15 functions as I/O pin)
#pragma config FCKSM = CSECME      // Clock Switching and Monitor (Clock switching is enabled, 
                                       // Fail-Safe Clock Monitor is enabled)
#pragma config FNOSC = FRCPLL      // Oscillator Select (Fast RC Oscillator with PLL module (FRCPLL))

volatile int adValue;

void __attribute__((__interrupt__,__auto_psv__)) _ADC1Interrupt(void) {
    _AD1IF = 0;
    
    putVal(ADC1BUF0);      //Grab latest sampled value ater conversion and place it in buffer
}

void __attribute__((__interrupt__,__auto_psv__)) _T2Interrupt(void) {
    _T2IF = 0;
    
    adValue = getAvg();      //Grab average buffer value every 100ms
}

void setup (void) {
    AD1PCFG = 0x9FFC;       //Set pin RA0, RA1 as analog
    TRISA = 0x0003;        //Set pin RA0, RA1 as an input
    
    T2CON = 0;                  //Set up timer 2 to have a delay of 100ms
    PR2 = 24999;                //and enable the interrupt for timer 2
    T2CONbits.TCKPS = 2;
    T2CONbits.TON = 1;
    _T2IE = 1;
    _T2IF = 0;
    
    I2C1CONbits.I2CEN = 0;        //Disable I2C2 to safely change Baud Rate
    I2C1BRG = 0x9D;               //Set Baud Rate to 100kHz
    I2C1CONbits.I2CEN = 1;        //Enable I2C2 for project use
    _MI2C1IF = 0;                 //clear I2C2 flag
    
    T3CON = 0;                 //Set up timer 3 with a delay of 62.5ms for  
    PR3 = 15625;               //use with ADC conversions on pin RA0
    T3CONbits.TCKPS = 2;
    T3CONbits.TON = 1;
    
    AD1CON1 = 0, AD1CON2 = 0, AD1CON3 = 0;
    _AD1IE = 1;
    _AD1IF = 0;
    AD1CON1bits.ASAM = 1;       //Turn on automatic sampling
    AD1CON1bits.SSRC = 2;     //Use timer 3 to end sampling time
    
    AD1CON2bits.VCFG = 0;       //Configure refernce voltages to be Vdd and GND
    AD1CON2bits.SMPI = 0;     //Interrupt after completion of every conversion 
    
    AD1CON3bits.SAMC = 1;       //Set Tsmp = 1*Tad
    AD1CON3bits.ADCS = 1;     //Set Tad = 2*Tcy
    
    AD1CON1bits.ADON = 1;       //Turn on ADC1 Peripheral
}

int main(void) {
    setup();             //Initializing pins and setting up peripherals
    initBuffer();        //Clearing the circular buffer
    lcd_init();          //Running through LCD initialization sequence
    
    char adStr[20];               //The string printed to the LCD
    
    while(1) {
        lcd_setCursor(0,0);                    //Setting the cursor to the top left corner of the display
        sprintf(adStr,"%6.4f V",(3.3/1024)*adValue);      //Formatting the string that will be written
        
        lcd_printStr(adStr);         //Writing the entire string to the display    
    }
    
    
    return 0;
}
