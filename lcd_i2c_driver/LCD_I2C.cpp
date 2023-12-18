/**
 * @file LCD_I2C.cpp
 * @author Jakub Mašek (xmasek19@fit.vutbr.cz)
 * @brief Driver for LCD display
 * @version 0.1
 * @date 2023-09-12
 */

#include "LCD_I2C.h"

/*-------------------------[Public functions]-------------------------*/

/**
 * @brief Initialize LCD display
 *
 * @param i2c_number i2c bus number
 * @param addr Address of display on i2c bus
 * @param baudrate i2c bus baudrate (default 100kHz)
 */
void LCD_I2C::init(i2c_inst_t *i2c_number, int addr, const int baudrate)
{
    this->i2c_addr = addr;
    this->i2cNumber = i2c_number;
    if (!i2c_init(i2c_number, baudrate)){
        fprintf(stderr, "Failed to initialize i2c bus\n");
        return;
    }

    busy_wait_ms(300);

    // init sequence from datasheet
    LCD_I2C::sendByte(0x03, TYPE_COMMAND);
    sleep_ms(5);
    LCD_I2C::sendByte(0x03, TYPE_COMMAND);
    LCD_I2C::sendByte(0x03, TYPE_COMMAND);
    LCD_I2C::sendByte(0x02, TYPE_COMMAND); // 4-bit mode

    LCD_I2C::sendByte(CMD_FUNCTIONSET | CMD_FS_2LINE | CMD_FS_5x8DOTS, TYPE_COMMAND); // 16x2 display
    LCD_I2C::sendByte(CMD_DISPLAYCONTROL | CMD_DC_DISPLAYON | CMD_DC_CURSOROFF | CMD_DC_BLINKOFF , TYPE_COMMAND); // display control, display on/off, cursor on/off, blink on/off
    LCD_I2C::sendByte(CMD_CLEARDISPLAY, TYPE_COMMAND); // clear display
    sleep_ms(5);
    LCD_I2C::sendByte(CMD_ENTRYMODESET | this->entry_mode, TYPE_COMMAND); // entry mode set, entry left/right, entry shift increment/decrement
}

/**
 * @brief Set cursor to given position
 *
 * @param line Line number
 * @param row Row number
 */
void LCD_I2C::setCursor(uint8_t line, uint8_t row)
{
    uint8_t linesOffsets[4] = {0x00,0x40,0x14,0x54};
    LCD_I2C::sendByte(DDRAM_LINE1 | (linesOffsets[line] + row), TYPE_COMMAND);
}

/**
 * @brief Clear LCD display
 *
 */
void LCD_I2C::clear(void)
{
    LCD_I2C::sendByte(CMD_CLEARDISPLAY, TYPE_COMMAND);
    busy_wait_ms(3);
}

/**
 * @brief Write string to LCD display
 *
 * @param data String to write
 */
void LCD_I2C::write(const char* s)
{
    string str(s);
    LCD_I2C::writeString(str);
}

/**
 * @brief Write string to LCD display
 * 
 * @param s String to write
 */
void LCD_I2C::write(string s)
{
    LCD_I2C::writeString(s);
}

/**
 * @brief Write character from char table
 * 
 * @param location Address of character in character table
 */
void LCD_I2C::write(int location)
{
    LCD_I2C::writeChar(location);
}

/**
 * @brief Turn on/off backlight
 *
 * @param state True = on, False = off
 */
void LCD_I2C::backlight(bool state)
{
    this->backlight_state = state ? CMD_BACKLIGHT : CMD_NOBACKLIGHT;
    LCD_I2C::sendByte(CMD_NOOP, TYPE_COMMAND);
}

/**
 * @brief scroll display left by 1
 * 
 */
void LCD_I2C::scrollLeft(void) {
	LCD_I2C::sendByte(CMD_CURSORSHIFT | CMD_CS_DISPLAYMOVE | CMD_CS_MOVELEFT, TYPE_COMMAND);
}

/**
 * @brief scroll display right by 1
 * 
 */
void LCD_I2C::scrollRight(void) {
	LCD_I2C::sendByte(CMD_CURSORSHIFT | CMD_CS_DISPLAYMOVE | CMD_CS_MOVERIGHT, TYPE_COMMAND);
}

/**
 * @brief Write from left to right
 * 
 */
void LCD_I2C::entryLeft(void) {
    this->entry_mode |= CMD_EM_ENTRYLEFT;
    LCD_I2C::sendByte(CMD_ENTRYMODESET | this->entry_mode, TYPE_COMMAND);
}

/**
 * @brief Write from right to left (from cursor position to left)
 * 
 */
void LCD_I2C::entryRight(void) {
    this->entry_mode &= ~CMD_EM_ENTRYLEFT;
    LCD_I2C::sendByte(CMD_ENTRYMODESET | this->entry_mode, TYPE_COMMAND);
}

/**
 * @brief Move whole display to the left by 1 when new character is written
 * 
 */
void LCD_I2C::autoScroll(void) {
    this->entry_mode |= CMD_EM_ENTRYSHIFTINCREMENT;
    LCD_I2C::sendByte(CMD_ENTRYMODESET | this->entry_mode, TYPE_COMMAND);
}

/**
 * @brief Disable auto scroll
 * 
 */
void LCD_I2C::noAutoScroll(void) {
    this->entry_mode &= ~CMD_EM_ENTRYSHIFTINCREMENT;
    LCD_I2C::sendByte(CMD_ENTRYMODESET | this->entry_mode, TYPE_COMMAND);
}

/**
 * @brief Load custom character to character table
 * 
 * @param location Address of character in character table (0-7)
 * @param charMap Array of 8 bytes representing character (5x8)
 */
void LCD_I2C::createChar(uint8_t location, uint8_t charMap[]) {
    if (location > 7) {
        fprintf(stderr, "Location must be between 0 and 7\n");
        return;
    }

    uint8_t charAddr[8] = {0x40, 0x48, 0x50, 0x58, 0x60, 0x68, 0x70, 0x78};
    LCD_I2C::sendByte(charAddr[location], TYPE_COMMAND);
    for (int i = 0; i < 8; i++) {
        LCD_I2C::sendByte(charMap[i], TYPE_CHARACTER);
    }
}

/*-----------------------[END Public functions]-----------------------*/


/*------------------------[Private functions]-------------------------*/
/**
 * @brief Write byte to i2c bus
 *
 * @param value Byte to write
 */
void LCD_I2C::i2cWriteByte(uint8_t value)
{
    i2c_write_blocking(i2cNumber, i2c_addr, &value, 1, false);
}

/**
 * @brief Toggle enable pin
 *
 * @param val Value to toggle
 */
void LCD_I2C::toggleEnable(uint8_t val)
{
    LCD_I2C::i2cWriteByte(val | CMD_ENABLE);
    busy_wait_us(5);
    LCD_I2C::i2cWriteByte(val & ~CMD_ENABLE);
    busy_wait_us(80);
}

/**
 * @brief Send byte to LCD display
 *
 * @param val Value to send
 * @param mode Character or command
 */
void LCD_I2C::sendByte(uint8_t val, uint8_t mode)
{
    uint8_t high_nibble = mode | (val & 0xF0) | this->backlight_state;
    uint8_t low_nibble = mode | ((val << 4) & 0xF0) | this->backlight_state;

    LCD_I2C::i2cWriteByte(high_nibble);
    LCD_I2C::toggleEnable(high_nibble);
    LCD_I2C::i2cWriteByte(low_nibble);
    LCD_I2C::toggleEnable(low_nibble);
}

/**
 * @brief Write character to LCD display
 *
 * @param ch Character to write
 */
void LCD_I2C::writeChar(const char ch)
{
    LCD_I2C::sendByte(ch, TYPE_CHARACTER);
}

/**
 * @brief Write string to LCD display
 *
 * @param s String to write
 */
void LCD_I2C::writeString(string s)
{
    for (int i = 0; i < s.length(); i++)
    {
        LCD_I2C::writeChar(s[i]);
    }
}
/*----------------------[END Private functions]-----------------------*/