/*
Designed by Raghav Malik

This program reads temperature and pressure from an MS5611 sensor and advertises the data over BLE using an ESP32-S3-WROOM-1. The data is updated every 2 seconds.

Libraries
Wire.h: For I2C communication with the MS5611 sensor.
MS5611.h: For interfacing with the MS5611 sensor.
NimBLEDevice.h: For BLE functionality.

BLE Service and Characteristics

Service UUID: 0000181A-0000-1000-8000-00805F9B34FB (Environmental Sensing)
Temperature Characteristic UUID: 00002A6E-0000-1000-8000-00805F9B34FB
Pressure Characteristic UUID: 00002A6D-0000-1000-8000-00805F9B34FB

*/

#include <Wire.h>
#include <MS5611.h>
#include <NimBLEDevice.h>

// Define the BLE Service and Characteristic UUIDs
#define SERVICE_UUID        "0000181A-0000-1000-8000-00805F9B34FB"  // Environmental Sensing Service
#define TEMP_CHAR_UUID      "00002A6E-0000-1000-8000-00805F9B34FB"  // Temperature Characteristic
#define PRESSURE_CHAR_UUID  "00002A6D-0000-1000-8000-00805F9B34FB"  // Pressure Characteristic

// Global declarations for BLE characteristics
NimBLECharacteristic* tempCharacteristic;
NimBLECharacteristic* pressureCharacteristic;

// Initialize the MS5611 sensor
MS5611 ms5611;

// Function prototypes
void setupBLE();
void updateSensorData();

void setup() {
  // Initialize serial communication
  Serial.begin(115200);

  // Initialize I2C communication and MS5611 sensor
  Wire.begin(20, 21);  // SDA, SCL
  if (!ms5611.begin()) {
    Serial.println("Could not find a valid MS5611 sensor, check wiring!");
    while (1);
  }
  Serial.println("MS5611 sensor initialized.");

  // Setup BLE
  setupBLE();
}

void loop() {
  // Update and transmit sensor data over BLE
  updateSensorData();
  
  // Delay before the next reading
  delay(2000);
}

/**
 * @brief Set up the BLE server, service, and characteristics
 */
void setupBLE() {
  // Initialize BLE device
  NimBLEDevice::init("IoT Node 1 - Raghav Malik");

  // Create BLE server
  NimBLEServer* pServer = NimBLEDevice::createServer();

  // Create BLE service
  NimBLEService* pService = pServer->createService(SERVICE_UUID);

  // Create BLE characteristics for temperature and pressure
  tempCharacteristic = pService->createCharacteristic(
                        TEMP_CHAR_UUID,
                        NIMBLE_PROPERTY::READ | NIMBLE_PROPERTY::NOTIFY
                      );
  pressureCharacteristic = pService->createCharacteristic(
                            PRESSURE_CHAR_UUID,
                            NIMBLE_PROPERTY::READ | NIMBLE_PROPERTY::NOTIFY
                          );

  // Start the service
  pService->start();

  // Start advertising
  NimBLEAdvertising* pAdvertising = NimBLEDevice::getAdvertising();
  pAdvertising->addServiceUUID(SERVICE_UUID);
  pAdvertising->setScanResponse(true);
  pAdvertising->start();

  Serial.println("BLE Advertising Started");
}

/**
 * @brief Read sensor data, convert to bytes, and update BLE characteristics
 */
void updateSensorData() {
  // Read the sensor data
  ms5611.read();
  float temperature = ms5611.getTemperature();
  float pressure = ms5611.getPressure();

  // Convert temperature to hundredths of degrees Celsius
  int16_t intTemperature = static_cast<int16_t>(temperature * 100);
  // Convert pressure to tenths of hPa
  int32_t intPressure = static_cast<int32_t>(pressure * 10);

  // Print the sensor data to the Serial Monitor
  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.println(" *C");

  Serial.print("Pressure: ");
  Serial.print(pressure);
  Serial.println(" hPa");

  // Convert integer values to byte arrays
  uint8_t tempBytes[2];
  tempBytes[0] = intTemperature & 0xFF;
  tempBytes[1] = (intTemperature >> 8) & 0xFF;

  uint8_t pressureBytes[4];
  pressureBytes[0] = intPressure & 0xFF;
  pressureBytes[1] = (intPressure >> 8) & 0xFF;
  pressureBytes[2] = (intPressure >> 16) & 0xFF;
  pressureBytes[3] = (intPressure >> 24) & 0xFF;

  // Debug: Print the byte arrays
  Serial.print("Temp Bytes: ");
  for (int i = 0; i < 2; i++) {
    Serial.print(tempBytes[i], HEX);
    Serial.print(" ");
  }
  Serial.println();

  Serial.print("Pressure Bytes: ");
  for (int i = 0; i < 4; i++) {
    Serial.print(pressureBytes[i], HEX);
    Serial.print(" ");
  }
  Serial.println();

  // Update BLE characteristics value
  tempCharacteristic->setValue(tempBytes, sizeof(tempBytes));
  tempCharacteristic->notify();

  pressureCharacteristic->setValue(pressureBytes, sizeof(pressureBytes));
  pressureCharacteristic->notify();
}
