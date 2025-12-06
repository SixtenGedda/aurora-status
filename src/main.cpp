#include <Arduino.h>
#include <HTTPClient.h>
#include <WiFi.h>
#include <config.h>

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

void callApi() {
  WiFiClientSecure *client = new WiFiClientSecure;
  client->setInsecure();

  HTTPClient https;

  Serial.println("Sending Aurora API request");
  https.begin(*client, "https://services.swpc.noaa.gov/products/"
                       "noaa-planetary-k-index-forecast.json");

  int httpCode = https.GET();

  if (httpCode == HTTP_CODE_OK) {
    Serial.println("Aurora API request successful");
    String payload = https.getString();
    Serial.println("JSON data:");
    Serial.println(payload);

  } else {
    Serial.print("Aurora API request failed: ");
    Serial.println(https.errorToString(httpCode));
  }

  https.end();
  delete client;
}

void setup() {
  Serial.begin(115200);
  wifiSetup();
  callApi();
}

void loop() {
  Serial.println("Loop körs...");
  delay(1000);
}
