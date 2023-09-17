/**
 * @file LCD_I2C.h
 * @author Jakub Mašek (xmasek19@fit.vutbr.cz)
 * @brief Header file for LCD display driver
 * @version 0.1
 * @date 2023-09-12
 */

#ifndef LCD_I2C_H
#define LCD_I2C_H

#include <string>
#include "hardware/i2c.h"

/*---------------------------[lcd byte types]---------------------------*/
#define TYPE_COMMAND 0
#define TYPE_CHARACTER 1
/*-------------------------[END lcd byte types]-------------------------*/

/*----------------------------[lcd commands]----------------------------*/
#define CMD_NOOP 0x00
#define CMD_CLEARDISPLAY 0x01
#define CMD_RETURNHOME 0x02
#define CMD_ENABLE 0x04
#define CMD_BACKLIGHT 0x08
#define CMD_NOBACKLIGHT 0x00
/*--------------------------[END lcd commands]--------------------------*/

/*--------------------------[display control]---------------------------*/
#define CMD_DISPLAYCONTROL 0x08
#define CMD_DC_DISPLAYON 0x04
#define CMD_DC_DISPLAYOFF 0x00
#define CMD_DC_CURSORON 0x02
#define CMD_DC_CURSOROFF 0x00
#define CMD_DC_BLINKON 0x01
#define CMD_DC_BLINKOFF 0x00
/*------------------------[END display control]-------------------------*/

/*-------------------------[lcd function set]---------------------------*/
#define CMD_FUNCTIONSET 0x20
#define CMD_FS_1LINE 0x00
#define CMD_FS_2LINE 0x08
#define CMD_FS_5x8DOTS 0x00
#define CMD_FS_5x10DOTS 0x04
/*--------------------------[END function set]--------------------------*/

/*---------------------------[lcd entry mode]---------------------------*/
#define CMD_ENTRYMODESET 0x04
#define CMD_EM_ENTRYLEFT 0x02
#define CMD_EM_ENTRYRIGHT 0x00
#define CMD_EM_ENTRYSHIFTINCREMENT 0x01
#define CMD_EM_ENTRYSHIFTDECREMENT 0x00
/*---------------------------[END entry mode]---------------------------*/

/*---------------------------[lcd cursor shift]-------------------------*/
#define CMD_CURSORSHIFT 0x10
#define CMD_CS_DISPLAYMOVE 0x08
#define CMD_CS_CURSORMOVE 0x00
#define CMD_CS_MOVERIGHT 0x04
#define CMD_CS_MOVELEFT 0x00
/*-------------------------[END cursor shift]---------------------------*/

/*---------------------------[lcd adrresses]----------------------------*/
#define DDRAM_LINE1 0x80
/*--------------------------[END lcd adrresses]-------------------------*/

/*--------------------------[lcd other defines]-------------------------*/
#define DEFAULT_BAUDRATE 100000
/*------------------------[END lcd other defines]-----------------------*/

using namespace std;

class LCD_I2C
{
public:
    void init(i2c_inst_t*, int = 0x27, const int = DEFAULT_BAUDRATE);
    void clear();
    void write(const char*);
    void write(string);
    void write(int);
    void setCursor(uint8_t, uint8_t);
    void backlight(bool);
    void scrollLeft();
    void scrollRight();
    void entryLeft();
    void entryRight();
    void autoScroll();
    void noAutoScroll();
    void createChar(uint8_t, uint8_t[]);

private:
    int i2c_addr;
    i2c_inst_t *i2cNumber;
    int backlight_state = CMD_BACKLIGHT;
    int entry_mode = CMD_EM_ENTRYLEFT | CMD_EM_ENTRYSHIFTDECREMENT;
    void i2cWriteByte(uint8_t);
    void toggleEnable(uint8_t);
    void sendByte(uint8_t, uint8_t);
    void writeChar(const char);
    void writeString(string);
};

#endif