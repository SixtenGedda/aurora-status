#include <Arduino.h>
#include <WiFi.h>

void setup() {
  Serial.begin(115200);
  Serial.println("Hej från C++!");
}

void loop() {
  Serial.println("Loop körs...");
  delay(1000);
}
