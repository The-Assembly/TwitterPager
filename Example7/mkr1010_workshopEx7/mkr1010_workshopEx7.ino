#include <Wire.h>
#include <WiFiNINA.h>
#include <SPI.h>
#include <LiquidCrystal_I2C.h>
#include <ArduinoHttpClient.h>

char ssid[] = "ASSEMBLY";           //  Your network SSID (name)
char pass[] = "WelcomeToASSEMBLY";  // Your network password

String username;
unsigned long startTime = 0;  // Tracks when a new username was set

LiquidCrystal_I2C lcd(0x27, 20, 4);

IPAddress apiWrapper(37, 16, 4, 227);  // Twitter API wrapper server IP
int status = WL_IDLE_STATUS;

WiFiClient wifi;  // initializes WiFi client
HttpClient client = HttpClient(
    wifi, apiWrapper, 80);  // use WiFi client to initialize HTTP client

void setup() {
  Serial.begin(9600);

  while (!Serial)
    ;
  status = WiFi.begin(ssid, pass);  // connect WiFi client to the network

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

void nextLineOrClear(LiquidCrystal_I2C* lcd, int* currentCol, int* currentRow,
                     int clearDelayAmount) {
  *currentCol = 0;
  if (*currentRow == 1) {
    lcd->clear();
    lcd->setCursor(0, 0);
    *currentRow = 0;
    delay(clearDelayAmount);
  } else {
    lcd->setCursor(0, 1);
    *currentRow = 1;
  }
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

    char* ptr = strtok((char*)response.c_str(), " ");

    lcd.init();
    lcd.backlight();
    lcd.clear();
    lcd.setCursor(0, 0);

    int currentRow = 0;
    int currentCol = 0;
    while (ptr != NULL) {
      String word = String(ptr);

      if (word.length() > 16) {
        // go to the next line if we're not at the beginning of a line
        nextLineOrClear(&lcd, &currentCol, &currentRow, 0);

        for (int i = 0; i < word.length(); i++) {
          if (i != 0 && i % 16 == 0) {
            nextLineOrClear(&lcd, &currentCol, &currentRow, 500);
          }

          lcd.print(word.charAt(i));
        }
      } else {
        if (currentCol != 0) {
          lcd.print(' ');
          currentCol += 1;
        }

        if (currentCol + word.length() >= 16) {
          nextLineOrClear(&lcd, &currentCol, &currentRow, 500);
          continue;
        } else {
          currentCol += word.length();
          lcd.print(word);
        }
      }

      // create next part
      ptr = strtok(NULL, " ");
      delay(500);
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
