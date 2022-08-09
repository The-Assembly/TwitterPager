#include <Wire.h>

#include <LiquidCrystal_I2C.h>


LiquidCrystal_I2C lcd(0x26, 16, 2); 

void setup()
{
  lcd.init(); 
  lcd.backlight(); 
  lcd.clear(); 
  lcd.setCursor(3,0); 
  lcd.print("Welcome to"); 
  lcd.setCursor(2,1); 
  lcd.print("The Assembly");
}
void loop() {
}
