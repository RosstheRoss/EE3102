/* 
 * File:   spies046_lab6_lcdLib_v001.h
 * Author: micha
 *
 * Created on April 14, 2020, 8:43 PM
 */

#ifndef SPIES046_LAB6_LCDLIB_V001_H
#define	SPIES046_LAB6_LCDLIB_V001_H

#ifdef	__cplusplus
extern "C" {
#endif

    void delay(int msecs);
    void lcd_cmd(char package);
    void lcd_init(void);
    void lcd_setCursor(char col, char row);
    void lcd_printChar(char myChar);
    void lcd_printStr(const char s[]);
    void lcd_ShiftL(const char s[]);
    void findLength(const char s[]);
    void contrastMask(char conTot);

#ifdef	__cplusplus
}
#endif

#endif	/* SPIES046_LAB6_LCDLIB_V001_H */

