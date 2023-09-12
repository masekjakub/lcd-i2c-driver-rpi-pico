# LCD I2C driver for Raspberry Pi Pico
This is a simple and fast driver for LCD displays with I2C interface for Raspberry Pi Pico.
## Runnig the example
1. Clone the repository
2. Create `build` directory and and enter it – `mkdir build && cd build`
3. Run `cmake ..` to generate the build files
4. Run `make` to build the example
5. Connect the LCD to the Raspberry Pi Pico to the I2C pins (SDA and SCL).
6. Connect the Raspberry Pi Pico to the computer while holding the BOOTSEL button
7. Copy the `example.uf2` file to the Raspberry Pi Pico

## Contributing
Feel free to open an issue or a pull request if you find a bug or have a suggestion.

## License
This project is licensed under the MIT License - feel free to do anything you want with the code.

## Acknowledgments
* [HD44780 Datasheet](https://www.sparkfun.com/datasheets/LCD/HD44780.pdf) – the LCD controller datasheet
* [LCD 1602 Datasheet](https://www.waveshare.com/datasheet/LCD_en_PDF/LCD1602.pdf) – the LCD datasheet
* [LCD Initialization](https://web.alfredstate.edu/faculty/weimandn/lcd/lcd_initialization/lcd_initialization_index.html) – the LCD initialization tutorial
* [LCD Addressing](https://web.alfredstate.edu/faculty/weimandn/lcd/lcd_addressing/lcd_addressing_index.html) – the LCD addressing tutorial
* [Raspberry Pi Pico SDK](https://www.raspberrypi.com/documentation/pico-sdk/) – the SDK used for this project