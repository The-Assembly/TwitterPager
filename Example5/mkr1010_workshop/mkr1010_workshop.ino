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
// ^ temp status assigned when WiFi.begin() is called and remains active until the number of attempts expires

WiFiClient wifi; //initializes WiFi client
HttpClient client = HttpClient(wifi, apiWrapper, 80); // use WiFi client to initialize HTTP client

void setup() {
  Serial.begin(9600);

  while (!Serial) // ensure serial monitor is responding
    ;
  status = WiFi.begin(ssid, pass); // connect WiFi client to the network initially

  // attempt to connect to Wifi network:
  while (status != WL_CONNECTED) { // is a constant, initialized in WiFi library
    Serial.print("Attempting to connect to network: ");
    Serial.println(ssid);
    status = WiFi.begin(ssid, pass); // // Connect to WPA/WPA2 network:
    delay(10000); // wait 10 seconds for connection
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
  Serial.println(statusCode); // ex 404 okay
  Serial.print("Response: ");
  Serial.println(response); // the body of the tweet

  lcd.init();
  lcd.backlight();
  lcd.clear();
  lcd.setCursor(0, 0);

  lcd.autoscroll(); // moves all the text one space to the left each time a letter is added
  for (int i = 0; i < response.length(); i++) {
    lcd.print(response.charAt(i));
    if (i % 16 == 0) { // wait until the 16th character
      delay(1000);
    }
  }

  Serial.println("Wait 1 hour");
  delay(1000 * 60 * 60);
}
