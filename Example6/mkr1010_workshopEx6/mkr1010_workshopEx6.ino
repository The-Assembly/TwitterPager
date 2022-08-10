#include <ArduinoHttpClient.h>
#include <LiquidCrystal_I2C.h>
#include <SPI.h>
#include <WiFiNINA.h>
#include <Wire.h>

char ssid[] = "ASSEMBLY";           //  Your network SSID (name)
char pass[] = "WelcomeToASSEMBLY";  // Your network password

String username;
unsigned long startTime = 0;  // Tracks when a new username was set

LiquidCrystal_I2C lcd(0x26, 16, 2);

IPAddress apiWrapper(37, 16, 4, 227);  // Twitter API wrapper server IP
int status = WL_IDLE_STATUS;

WiFiClient wifi;
HttpClient client = HttpClient(wifi, apiWrapper, 80);

void setup() {
  Serial.begin(9600);

  while (!Serial)
    ;
  status = WiFi.begin(ssid, pass);

  // Attempt to connect to Wifi network:
  while (status != WL_CONNECTED) {
    Serial.print("Attempting to connect to network: ");
    Serial.println(ssid);
    // Connect to WPA/WPA2 network:
    status = WiFi.begin(ssid, pass);
    // Wait 10 seconds for connection:
    delay(10000);
  }

  Serial.print("Dns configured.\n");
  Serial.print("Enter a twitter username: \n");

  while (Serial.available() == 0)
    ;

  String raw = Serial.readString();
  username = raw.substring(0, raw.length() - 1);
  Serial.println("Initializing pager with user: " + username);
}

void loop() {
  if (startTime == 0 || millis() - startTime >= (1000 * 60 * 60)) {
    // An hour has passed
    Serial.println("\nMaking GET request");

    client.get(username);

    // Read the status code and body of the response
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

    startTime = millis();
    Serial.println("Waiting...");
  } else {
    if (Serial.available()) {
      String raw = Serial.readString();
      username = raw.substring(0, raw.length() - 1);
      startTime = 0;  // Reset the start time
    }
  }
}
