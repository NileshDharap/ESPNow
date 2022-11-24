

/*
  ESP32 MAC Address printout
  esp32-mac-address.ino
  Prints MAC Address to Serial Monitor
*/

// Include WiFi Library
#include <ESP8266WiFi.h>

void setup() {

  // Setup Serial Monitor
  Serial.begin(115200);

  // Put ESP32 into Station mode
//  WiFi.mode(WIFI_MODE_STA);

  // Print MAC Address to Serial monitor
  Serial.print("MAC Address: ");
  Serial.println(WiFi.macAddress());
}

void loop() {

}
