#include <Wire.h>

byte buffer[4] = {0b11110000, 0b10101010};

void setup(){
  Serial.begin(112500);
  Wire.begin(7);
  Wire.onRequest(requestEvent);}

void loop(){}

void requestEvent(){
  Wire.write(buffer, 2);
  Serial.print("Sent : ");
  Serial.print(buffer[0], BIN);
  Serial.print(" , ");
  Serial.println(buffer[1], BIN);
}
