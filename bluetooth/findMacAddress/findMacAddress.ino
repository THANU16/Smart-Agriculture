#include <WiFi.h>

void setup() {
  Serial.begin(115200);
  delay(1000); // Give some time to initialize serial communication

  // Get MAC address
  uint8_t mac[6];
  WiFi.macAddress(mac);

  // Print MAC address
  Serial.print("MAC Address: ");
  for (int i = 0; i < 6; i++) {
    Serial.print(mac[i], HEX);
    if (i < 5) Serial.print(":");
  }
  Serial.println();
}

void loop() {
  // Your main code goes here
}
