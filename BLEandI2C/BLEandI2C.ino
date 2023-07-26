#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEClient.h>
#include <BLE2902.h>
#include <Wire.h>

uint8_t buffer[4] = {};
bool bufferReady = false; // Flag to indicate when buffer is ready to be sent

// BLE Client variables
BLEClient* pClient;
bool deviceConnected = false;

// Server MAC Address
#define SERVER_MAC_ADDR  "CC:DB:A7:68:B1:26"

// BLE Service UUID
#define SERVICE_UUID "4524248a-2ba4-11ee-be56-0242ac120002"

// BLE Service Characteristics for different soil properties
#define PHVALUE_CHARACTERISTIC_UUID "45242b06-2ba4-11ee-be56-0242ac120002"
#define CONDUCTIVITY_CHARACTERISTIC_UUID "45242e3a-2ba4-11ee-be56-0242ac120002"
#define MOISTURE_CHARACTERISTIC_UUID "4524301a-2ba4-11ee-be56-0242ac120002"
#define TEMPERATURE_CHARACTERISTIC_UUID "452431c8-2ba4-11ee-be56-0242ac120002"

// Callback class for BLE client events
class MyClientCallbacks : public BLEClientCallbacks {
  void onConnect(BLEClient* pClient) {
    deviceConnected = true;
  }

  void onDisconnect(BLEClient* pClient) {
    deviceConnected = false;
  }
};

void setup() {
  // Initialize Serial Communication
  Serial.begin(115200);

  // Initialize BLE Client
  BLEClientInit();
}

void loop() {
  if (deviceConnected) {
    // Perform any desired actions with the connected server
    delay(1000);
  }
}

void BLEClientInit() {
  // Create the BLE Client
  BLEDevice::init("MyESP32");
  pClient = BLEDevice::createClient();
  pClient->setClientCallbacks(new MyClientCallbacks());

  // Connect to the server
  pClient->connect(BLEAddress(SERVER_MAC_ADDR));

  // Get the service and characteristic
  BLERemoteService* pRemoteService = pClient->getService(SERVICE_UUID);
  BLERemoteCharacteristic* pPHvalueCharacteristic = pRemoteService->getCharacteristic(PHVALUE_CHARACTERISTIC_UUID);
  BLERemoteCharacteristic* pConductivityCharacteristic = pRemoteService->getCharacteristic(CONDUCTIVITY_CHARACTERISTIC_UUID);
  BLERemoteCharacteristic* pMoistureCharacteristic = pRemoteService->getCharacteristic(MOISTURE_CHARACTERISTIC_UUID);
  BLERemoteCharacteristic* pTemperatureCharacteristic = pRemoteService->getCharacteristic(TEMPERATURE_CHARACTERISTIC_UUID);

  // Enable notifications/indications
  pPHvalueCharacteristic->registerForNotify([](BLERemoteCharacteristic* pBLERemoteCharacteristic, uint8_t* pData, size_t length, bool isNotify) {
    Serial.write(pData, length);
    Serial.println();
    for (size_t i = 0; i < length; i++) {
      buffer[0] = pData[i]; // Correctly assign each byte of pData to the buffer
    }
    Serial.print(":");
    Serial.println(buffer[0]);
    checkBufferReady();
});

pConductivityCharacteristic->registerForNotify([](BLERemoteCharacteristic* pBLERemoteCharacteristic, uint8_t* pData, size_t length, bool isNotify) {
    Serial.write(pData, length);
    for (size_t i = 0; i < length; i++) {
      buffer[1] = pData[i]; // Correctly assign each byte of pData to the buffer
    }
    Serial.print(":");
    Serial.println(buffer[1]);
    checkBufferReady();
});

pMoistureCharacteristic->registerForNotify([](BLERemoteCharacteristic* pBLERemoteCharacteristic, uint8_t* pData, size_t length, bool isNotify) {
    Serial.write(pData, length);
    Serial.println();
    for (size_t i = 0; i < length; i++) {
      buffer[2] = pData[i]; // Correctly assign each byte of pData to the buffer
    }
    Serial.print(":");
    Serial.println(buffer[2]);
    checkBufferReady();
});

pTemperatureCharacteristic->registerForNotify([](BLERemoteCharacteristic* pBLERemoteCharacteristic, uint8_t* pData, size_t length, bool isNotify) {
    Serial.write(pData, length);
    Serial.println();
    for (size_t i = 0; i < length; i++) {
      buffer[3] = pData[i]; // Correctly assign each byte of pData to the buffer
    }
    Serial.print(":");
    Serial.println(buffer[3]);
    checkBufferReady();
});


  // I2C protocol connect
  Wire.begin(0x08);
  Wire.onRequest(requestEvent);

  delay(1000);
}

void checkBufferReady() {
  // Check if all four readings are received and stored in the buffer
  if (buffer[0] != 0 && buffer[1] != 0 && buffer[2] != 0 && buffer[3] != 0) {
    bufferReady = true; // Set the flag when all four readings are received
    // Serial.print("Buffer Data: ");
    // for (int i = 0; i < 4; i++) {
    //   Serial.print(buffer[i]);
    //   Serial.print(" ");
    // }
    // Serial.println();
  }
}

void requestEvent() {
  if (bufferReady){
     Wire.write(buffer, 4); // Send the buffer data to the master
    //  Serial.println(buffer,4);
  Serial.println("Buffer sent to master");

  // Reset the buffer and bufferReady flag for next set of readings
  memset(buffer, 0, sizeof(buffer));
  bufferReady = false;
  }
 
  Serial.println("Buffer nit fill ");
}