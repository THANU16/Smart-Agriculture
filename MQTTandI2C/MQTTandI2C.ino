#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 32
#define OLED_ADDR 0x3C  // I2C address of the SSD1306 display

#ifndef STASSID
#define STASSID "Galaxy A50s526A"
#define STAPSK "tzjr9405"
#endif

const char* ssid ="SLIoT";
const char* password = "sl10t@c$e";
char* clientID = "12345";

char tem_arr[8] = "aaaaa";
char con_arr[8] = "aaaaa";
char phv_arr[8] = "aaaaa";
char moi_arr[8] = "aaaaa";

WiFiClient wifiServer;
PubSubClient mqttClient(wifiServer);
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

void printTextOnDisplay(const String& text, int cursorX, int cursorY, bool clear = true) {
  if (clear) {
    display.clearDisplay();
  }
  display.setTextSize(2);
  display.setTextColor(SSD1306_WHITE);

  display.setCursor(cursorX, cursorY);
  display.println(text);
  display.display();
}

void setup(void) {
  Serial.begin(115200);
  setupWifi();
  setupMQTT();
  Wire.begin();

  // Initialize OLED display with the specified I2C address
  if (!display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;)
      ;
  }

  // Clear the display buffer
  display.clearDisplay();
  Wire.begin();
  delay(500);
}

void loop(void) {
  Wire.requestFrom(0x08, 4);  // Request 1 byte of data from the slave

  int i = 0;
  while (Wire.available() > 0) {
    *tem_arr = Wire.read();
    Serial.println(tem_arr);
    *con_arr = Wire.read();
    Serial.println(tem_arr);
    *phv_arr = Wire.read();
    Serial.println(phv_arr);
    *moi_arr = Wire.read();
    Serial.println(moi_arr);
  }

  delay(1000);

  if (!mqttClient.connected()) {
    connectToBroker();
  }
  mqttClient.loop();

  printTextOnDisplay("Temperature: ", 0, 0);
  printTextOnDisplay(tem_arr, 10, 20, false);
  mqttClient.publish("Soil-Tem", tem_arr);
  delay(2000);

  printTextOnDisplay("Moisture ", 0, 0);
  printTextOnDisplay(moi_arr, 10, 20, false);
  mqttClient.publish("Soil-Moi", moi_arr);
  delay(2000);

  printTextOnDisplay("Conductivity: ", 0, 0);
  printTextOnDisplay(con_arr, 10, 20, false);
  mqttClient.publish("Soil-Con", con_arr);
  delay(2000);

  printTextOnDisplay("PH value: ", 0, 0);
  printTextOnDisplay(phv_arr, 10, 20, false);
  mqttClient.publish("Soil-Phv", phv_arr);
  delay(2000);
}

void setupWifi() {
  WiFi.begin(ssid, password);
  Serial.println("Connecting to wifi");

  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println("Connected!");
}

void setupMQTT() {
  mqttClient.setServer("test.mosquitto.org", 1883);
}

void connectToBroker() {
  while (!mqttClient.connected()) {
    Serial.print("Attempting MQTT connection...");
    if (mqttClient.connect(clientID)) {
      Serial.println("Connected");
    } else {
      Serial.print("Failed ");
      Serial.println(mqttClient.state());
      delay(500);
    }
  }
}