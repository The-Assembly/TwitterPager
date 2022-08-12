#include <LiquidCrystal_I2C.h>
#include <Wire.h>

LiquidCrystal_I2C lcd(0x27,16,2);  

void setup()
{
  lcd.init();                      // initialize the lcd 
  lcd.backlight();
  Serial.begin(9600);
}

void loop()
{
  if (Serial.available()) {
    delay(100);        // wait a bit for the entire message to arrive
    lcd.clear();       // clear the screen
    String raw = Serial.readString();  // read all the available characters
    String input = raw.substring(0, raw.length() - 1); // removes the new line
    lcd.print(input);
  }
}
