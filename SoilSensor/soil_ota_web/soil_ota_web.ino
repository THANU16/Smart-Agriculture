//#include <ESP8266WiFi.h>
#include "offset_memory.h"
#include "sensor.h"
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>

// BLE Server variables
BLEServer *pServer;


BLECharacteristic *pPHvalueCharacteristic;
BLECharacteristic *pConductivityCharacteristic;
BLECharacteristic *pMoistureCharacteristic;
BLECharacteristic *pTemperatureCharacteristic;

bool deviceConnected = false;

// BLE Service UUID
#define SERVICE_UUID "4524248a-2ba4-11ee-be56-0242ac120002"

// BLE Service Characteristics for different soil properties

#define PHVALUE_CHARACTERISTIC_UUID "45242b06-2ba4-11ee-be56-0242ac120002"
#define CONDUCTIVITY_CHARACTERISTIC_UUID "45242e3a-2ba4-11ee-be56-0242ac120002"
#define MOISTURE_CHARACTERISTIC_UUID "4524301a-2ba4-11ee-be56-0242ac120002"
#define TEMPERATURE_CHARACTERISTIC_UUID "452431c8-2ba4-11ee-be56-0242ac120002"


// Callback class for BLE server events
class MyServerCallbacks : public BLEServerCallbacks
{
  void onConnect(BLEServer *pServer)
  {
    deviceConnected = true;
  }

  void onDisconnect(BLEServer *pServer)
  {
    deviceConnected = false;
  }
};


void setup(void) {

  Serial.begin(115200);
   // Initialize BLE Server
  BLEServerInit();

  setup_sensor();
  read_offset();
}

void loop(void) {

  Serial.println(deviceConnected);
  if (deviceConnected)
  {
    dataRead_print();
    characteristicNotify(pPHvalueCharacteristic, phv_arr);
    characteristicNotify(pConductivityCharacteristic, con_arr);
    characteristicNotify(pMoistureCharacteristic, moi_arr);
    characteristicNotify(pTemperatureCharacteristic, tem_arr);
    delay(1000);
  }
  else{
    pServer->getAdvertising()->start();
    Serial.println("Waiting for connections...");
    delay(1000);
  } 
}


void characteristicNotify(BLECharacteristic *pCharacteristic, char *value)
{
  // Update the characteristic value with data from sensors or other sources
  pCharacteristic->setValue(value);
  // Notify/Indicate the value to the connected client
  pCharacteristic->notify();
  Serial.print("Sending value: ");
  Serial.println(value);
}

void BLEServerInit()
{
  // Create the BLE Server
  BLEDevice::init("MyESP32");
  pServer = BLEDevice::createServer();
  pServer->setCallbacks(new MyServerCallbacks());

  // Create the BLE Service
  BLEService *pService = pServer->createService(SERVICE_UUID);

  // Create the BLE Characteristics
  pPHvalueCharacteristic = pService->createCharacteristic(
      PHVALUE_CHARACTERISTIC_UUID,
      BLECharacteristic::PROPERTY_WRITE |
          BLECharacteristic::PROPERTY_NOTIFY);

  pConductivityCharacteristic = pService->createCharacteristic(
      CONDUCTIVITY_CHARACTERISTIC_UUID,
      BLECharacteristic::PROPERTY_WRITE |
          BLECharacteristic::PROPERTY_NOTIFY);

  pMoistureCharacteristic = pService->createCharacteristic(
      MOISTURE_CHARACTERISTIC_UUID,
      BLECharacteristic::PROPERTY_WRITE |
          BLECharacteristic::PROPERTY_NOTIFY);

  pTemperatureCharacteristic = pService->createCharacteristic(
      TEMPERATURE_CHARACTERISTIC_UUID,
      BLECharacteristic::PROPERTY_WRITE |
          BLECharacteristic::PROPERTY_NOTIFY);

  // Add a descriptor for the Characteristic
  pPHvalueCharacteristic->addDescriptor(new BLE2902());
  pConductivityCharacteristic->addDescriptor(new BLE2902());
  pMoistureCharacteristic->addDescriptor(new BLE2902());
  pTemperatureCharacteristic->addDescriptor(new BLE2902());

  // Start the service
  pService->start();

  // Start advertising
  pServer->getAdvertising()->start();
  Serial.println("Waiting for connections...");
}