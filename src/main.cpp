#include <Arduino.h>
#include <ArduinoJson.h>
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
    Serial.println("JSON data:");
    Serial.println(jsondata);
    return (jsondata);
    https.end();

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
  Serial.print("First Kp value: ");
  Serial.println(kp);
  return kp;
}

void setup() {
  Serial.begin(115200);
  delay(1000);
  wifiSetup();
  parseJson(callApi());
}

void loop() {
  Serial.println("Loop körs...");
  delay(10000);
}
