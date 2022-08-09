#include <Wire.h>

void setup(){
  Wire.begin();           // initializes Wire library and join the I2C bus as a controller/peripheral
  Serial.begin(9600);     // starts serial communication (Arduino can send out commands through the USB connection)
  while (!Serial);        // wait for serial monitor      
  Serial.println("\nI2C Scanner");
}
 
void loop(){
  byte error, address;    // initialize variables for error and I2C address
  int nDevices;
 
  Serial.println("Scanning...");
 
  nDevices = 0;
  for(address = 1; address < 127; address++ ){
    Wire.beginTransmission(address); //begins a transmission to the I2C peripheral device with the given address
    error = Wire.endTransmission();
 
    if (error == 0){
      Serial.print("I2C device found at address 0x");
      if (address<16)
        Serial.print("0");
      Serial.print(address,HEX);
      Serial.println("  !");
 
      nDevices++;
    } else if (error==4){
      Serial.print("Unknown error at address 0x");
      if (address<16)
        Serial.print("0");
      Serial.println(address,HEX);
    }    
  }
  if (nDevices == 0)
    Serial.println("No I2C devices found\n");
  else
    Serial.println("done\n");
 
  delay(5000);       
}
