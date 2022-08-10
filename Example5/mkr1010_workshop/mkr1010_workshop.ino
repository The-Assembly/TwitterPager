#include <ArduinoHttpClient.h> // used for HTTP (GET, POST, PUT, DELETE) requests to a web server
#include <LiquidCrystal_I2C.h>
#include <SPI.h>
#include <WiFiNINA.h>
#include <Wire.h>

char ssid[] = "ASSEMBLY";  //  your network SSID (name)
char pass[] = "WelcomeToASSEMBLY";   // your network password

LiquidCrystal_I2C lcd(0x26, 16, 2);
String stringVariable;

IPAddress apiWrapper(37, 16, 4, 227);
int status = WL_IDLE_STATUS; // is a constant, initialized in WiFi library

WiFiClient wifi;
HttpClient client = HttpClient(wifi, laptop, 80);

void setup() {
  Serial.begin(9600);

  while (!Serial) // ensure serial monitor is responding
    ;
  status = WiFi.begin(ssid, pass); // connect to the network initially

  // attempt to connect to Wifi network:
  while (status != WL_CONNECTED) { // is a constant, initialized in WiFi library
    Serial.print("Attempting to connect to network: ");
    Serial.println(ssid);
    // Connect to WPA/WPA2 network:
    status = WiFi.begin(ssid, pass);

    // wait 10 seconds for connection:
    delay(10000);
  }
  Serial.print("Dns configured. \n");
  Serial.print("Enter a twitter username: \n");
}

void loop() {
  Serial.println("\nmaking GET request");
  
  client.get("/makesmartthings");

  // read the status code and body of the response
  int statusCode = client.responseStatusCode();
  String response = client.responseBody();

  Serial.print("Status code: ");
  Serial.println(statusCode);
  Serial.print("Response: ");
  Serial.println(response);

  lcd.init();
  lcd.backlight();
  lcd.clear();
  lcd.setCursor(0, 0);

  lcd.autoscroll();
  for (int i = 0; i < response.length(); i++) {
    lcd.print(response.charAt(i));
    if (i % 16 == 0) {
      delay(1000);
    }
  }

  Serial.println("Wait 1 hour");
  delay(1000 * 60 * 60);
}
