/**
 * @file example.cpp
 * @author Jakub Mašek (xmasek19@fit.vutbr.cz)
 * @brief Usage example of LCD_I2C library
 * @version 0.1
 * @date 2023-09-12
 */

#include "LCD_I2C.h"
#include "pico/stdlib.h"
#include "pico/binary_info.h"
#include <unistd.h>
#include <iostream>

using namespace std;

void testWrite(LCD_I2C lcd)
{
    lcd.clear();
    lcd.setcursor(0, 5);
    lcd.write("Hello");
    lcd.setcursor(1, 2);
    lcd.write("Second line");

    sleep_ms(4000);

    lcd.clear();
    lcd.setcursor(0, 15);
    lcd.entryRight();
    lcd.write("backwards");
    lcd.write(" lamron");

    lcd.entryLeft();
    sleep_ms(4000);
}

void testBlinking(LCD_I2C lcd)
{
    lcd.clear();
    lcd.write("Blinking");
    for (int i = 0; i < 4; i++)
    {
        lcd.backlight(false);
        sleep_ms(500);
        lcd.setcursor(0, 10);
        lcd.write(to_string(i));
        lcd.backlight(true);
        sleep_ms(500);
    }
    sleep_ms(1000);
}

void testScroll(LCD_I2C lcd)
{
    lcd.clear();
    lcd.write("Scrolling");
    for (int i = 0; i < 4; i++)
    {
        lcd.scrollRight();
        sleep_ms(500);
    }
    for (int i = 0; i < 4; i++)
    {
        lcd.scrollLeft();
        sleep_ms(500);
    }
    sleep_ms(1000);
}

void testCustomChars(LCD_I2C lcd)
{
    lcd.clear();
    lcd.setcursor(0, 2);
    lcd.write("Custom chars");
    uint8_t cc1[] = {0x00, 0x00, 0x0A, 0x00, 0x11, 0x0E, 0x00, 0x00}; // smiley
    uint8_t cc2[] = {0x1F, 0x10, 0x1F, 0x01, 0x1F, 0x10, 0x1F, 0x01}; // snake
    uint8_t cc3[] = {0x00, 0x00, 0x04, 0x02, 0x1F, 0x02, 0x04, 0x00}; // arrow
    lcd.createChar(0, cc1);
    lcd.createChar(1, cc2);
    lcd.createChar(2, cc3);

    lcd.setcursor(1, 6);
    lcd.write(0);
    lcd.write(1);
    lcd.write(2);

    sleep_ms(3000);
}

void writeEndSpeech(LCD_I2C lcd)
{
    lcd.autoScroll();
    string s = "Feel free to use this code in your project :)  Please star the repo if you like it! Created by @masekjakub";

    while (1)
    {
        lcd.clear();
        lcd.setcursor(0, 16);
        for (int i = 0; i < s.length(); i++)
        {
            // ensure that the text will not flow out of the proper line
            if (i % 24 == 0 && i > 0)
            {
                lcd.noAutoScroll();
                lcd.clear();
                lcd.setcursor(0, 0);
                lcd.write(s.substr(i - 16, 16));
                lcd.autoScroll();
                lcd.setcursor(0, 16);
            }
            lcd.write(s[i]);
            sleep_ms(300);
        }
        sleep_ms(1000);
    }
}

int main()
{
    stdio_init_all(); // usb

    gpio_set_function(PICO_DEFAULT_I2C_SDA_PIN, GPIO_FUNC_I2C);
    gpio_set_function(PICO_DEFAULT_I2C_SCL_PIN, GPIO_FUNC_I2C);
    gpio_pull_up(PICO_DEFAULT_I2C_SDA_PIN);
    gpio_pull_up(PICO_DEFAULT_I2C_SCL_PIN);
    // Make the I2C pins available to picotool
    bi_decl(bi_2pins_with_func(PICO_DEFAULT_I2C_SDA_PIN, PICO_DEFAULT_I2C_SCL_PIN, GPIO_FUNC_I2C));

    sleep_ms(1000);

    LCD_I2C lcd;
    lcd.init(i2c_default);
    cout << "LCD initialized" << endl;

    testWrite(lcd);
    testBlinking(lcd);
    testScroll(lcd);
    testCustomChars(lcd);
    cout << "Done" << endl;

    writeEndSpeech(lcd);
    return 0;
}