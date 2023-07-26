#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEClient.h>
#include <BLE2902.h>

// BLE Client variables
BLEClient* pClient;
bool deviceConnected = false;

// Server MAC Address
#define SERVER_MAC_ADDR  "CC:DB:A7:68:B1:26"

// BLE Service UUID
#define SERVICE_UUID        "4fafc201-1fb5-459e-8fcc-c5c9c331914b"

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

void BLEClientInit(){
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
    Serial.print("Received PHvalue data: ");
    Serial.write(pData, length);
    Serial.println();
  });

  pConductivityCharacteristic->registerForNotify([](BLERemoteCharacteristic* pBLERemoteCharacteristic, uint8_t* pData, size_t length, bool isNotify) {
    Serial.print("Received Conductivity data: ");
    Serial.write(pData, length);
    Serial.println();
  });

  pMoistureCharacteristic->registerForNotify([](BLERemoteCharacteristic* pBLERemoteCharacteristic, uint8_t* pData, size_t length, bool isNotify) {
    Serial.print("Received Moisture data: ");
    Serial.write(pData, length);
    Serial.println();
  });

  pTemperatureCharacteristic->registerForNotify([](BLERemoteCharacteristic* pBLERemoteCharacteristic, uint8_t* pData, size_t length, bool isNotify) {
    Serial.print("Received Temperature data: ");
    Serial.write(pData, length);
    Serial.println();
  });


  delay(1000);
}