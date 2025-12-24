#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Arduino.h>
#include <ArduinoJson.h>
#include <HTTPClient.h>
#include <SPI.h>
#include <WiFi.h>
#include <Wire.h>
#include <config.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

#define OLED_RESET -1
#define SCREEN_ADDRESS 0x3C

#define SDA_PIN 21
#define SCL_PIN 22

TwoWire myWire = TwoWire(0); // Eget I2C-objekt
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &myWire, OLED_RESET);

void wifiSetup() {
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(WIFI_SSID);

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("WiFi connected");
  Serial.println();
}

void oledSetup() {
  Serial.println("Starting OLED setup");

  // Starta I2C med egna pins
  myWire.begin(SDA_PIN, SCL_PIN);

  // Initiera OLED
  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;)
      ; // loop forever
  }

  display.clearDisplay();
}

void updateOled(String kp) {
  display.clearDisplay();

  display.setTextSize(2);
  display.setTextColor(SSD1306_WHITE);

  display.setCursor((SCREEN_WIDTH - 3 * 6 * kp.length()) / 2,
                    (SCREEN_HEIGHT - 7 * 4) / 2);
  display.print("kp: " + kp);

  display.display();
}

String callApi() {
  WiFiClientSecure client;
  client.setInsecure();

  HTTPClient https;

  Serial.println("Sending Aurora API request");
  https.begin(client, "https://services.swpc.noaa.gov/products/"
                      "noaa-planetary-k-index-forecast.json");

  int httpCode = https.GET();

  if (httpCode == HTTP_CODE_OK) {
    Serial.println("Aurora API request successful");
    String jsondata = https.getString();
    https.end();
    return (jsondata);

  } else {
    Serial.print("Aurora API request failed: ");
    Serial.println(https.errorToString(httpCode));
    return (https.errorToString(httpCode));
    https.end();
  }
}

String parseJson(String jsonData) {
  JsonDocument doc;
  DeserializationError error = deserializeJson(doc, jsonData);

  String kp = doc[1][1];
  Serial.print("Kp value: ");
  Serial.println(kp);
  return kp;
}

void setup() {
  Serial.begin(115200);
  delay(1000);
  wifiSetup();
  oledSetup();
  updateOled(parseJson(callApi()));
}

void loop() {
  Serial.println("Loop running");
  delay(5000);
  Serial.println("updating KP value");
  display.ssd1306_command(SSD1306_DISPLAYON);
  updateOled(parseJson(callApi()));
  delay(10000);
  Serial.println("turning off display");
  display.clearDisplay();
  display.ssd1306_command(SSD1306_DISPLAYOFF);
}
