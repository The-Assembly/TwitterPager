/*
 * This library allows you to communicate with I2C device 
 * Reaching to pins like the SDA (data line) and SCL (clock line) 
 */
#include <Wire.h>

/*
 * Code in setup() only runs one time
 * Runs at the beginning of program
 */
void setup(){
  Wire.begin();           // initializes Wire library and join the I2C bus as a controller/peripheral
  /*
   * starts serial communication 
   * (Arduino can send out commands through the USB connection
   * serial port is capable of transferring a maximum of 9600 bits per second
   * 9600 = 'baud rate' of the connection. aka how fast the data is to be sent.
   */
  Serial.begin(9600);     
  while (!Serial);        // wait for serial monitor      
  Serial.println("\nI2C Scanner");
}

/*
 * Code in loop() will repeat over and over again
 */
void loop(){
  byte error, address;    // initialize variables for error and I2C address
  int nDevices;
 
  Serial.println("Scanning...");
  nDevices = 0;

  // loop through possible i2c addresses (0-128)
  for(address = 1; address < 127; address++ ){
    Wire.beginTransmission(address); // begins a transmission to the I2C peripheral device with the given address
    error = Wire.endTransmission();  // ends a transmission to a peripheral device 
 
    if (error == 0){                  // 0 = success
      Serial.print("I2C device found at address 0x");
      if (address<16)                 // get it in the format 0x0ff12fe 
        Serial.print("0");
      Serial.print(address,HEX);      // an arduino builtin thing
      Serial.println("  !");          // HEX is a constant and it tells arduino to print it in a certain way
 
      nDevices++;                     // move to the next device
    } else if (error==4){             // 4 = other error
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
