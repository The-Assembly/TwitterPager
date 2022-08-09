/*
 * This library allows you to communicate with I2C device 
 * Reaching to pins like the SDA (data line) and SCL (clock line) 
 */
#include <Wire.h>
#include <LiquidCrystal_I2C.h> //A library for I2C LCD displays.

/*
 * Initialize the I2C LCD object
 * Set the LCD address to 0x26 for a 16 chars and 2 line display
 */
LiquidCrystal_I2C lcd(0x26, 16, 2);

/*
 * Code in setup() only runs one time
 * Runs at the beginning of program
 */
void setup()        
{
  lcd.init();                // initialize the lcd
  lcd.backlight();           // enable/disable the backlight
  lcd.clear();               // clears LCD screen and positions cursor to upper-left corner
  lcd.setCursor(3,0);        // positions cursor
  lcd.print("Welcome to");   // prints text
  lcd.setCursor(2,1);        // positions cursor
  lcd.print("The Assembly"); // prints text
}

/*
 * Code in loop() will repeat over and over again
 */
void loop() {
}
