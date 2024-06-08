IoT System Project
Overview
This project aims to create an IoT system using an ESP32-S3 microcontroller and a GY-63 sensor to collect environmental data (temperature and pressure). The data is transmitted over BLE to a Raspberry Pi running Raspbian Lite 64-bit. The Raspberry Pi then forwards this data to a VPS running InfluxDB and Grafana using MQTT for data storage and visualization.

Components
ESP32-S3-WROOM-1: A powerful microcontroller with built-in WiFi and BLE capabilities.
GY-63: A sensor module to measure temperature and pressure.
Raspberry Pi: Acts as a gateway to receive BLE data and push it to the VPS.
VPS: Runs InfluxDB and Grafana for data storage and visualization.
System Architecture
Node (ESP32-S3 and GY-63):

Collects temperature and pressure data.
Transmits the data over BLE.
Gateway (Raspberry Pi):

Runs Raspbian Lite 64-bit.
Receives data from the ESP32-S3 over BLE.
Uses MQTT to forward the data to the VPS.
VPS:

Runs InfluxDB for data storage.
Runs Grafana for data visualization.
Setup Instructions
1. ESP32-S3 and GY-63
Code
The ESP32-S3 reads data from the GY-63 sensor and transmits it over BLE.
The code for the ESP32-S3 is provided in the repository.
Hardware Connections
Connect the GY-63 sensor to the ESP32-S3.
SDA: GPIO 20
SCL: GPIO 21
2. Raspberry Pi
Software Setup
Install Raspbian Lite 64-bit on the Raspberry Pi.
Set up BLE and MQTT services on the Raspberry Pi.
Use Python or a suitable script to read BLE data and publish it to MQTT.
Python Script (Example)
python
Copy code
import paho.mqtt.client as mqtt
import time
import Adafruit_BluefruitLE

# Set up BLE and MQTT client
ble = Adafruit_BluefruitLE.get_provider()
client = mqtt.Client()
client.connect("your-vps-ip", 1883, 60)

def read_ble_and_publish():
    # Implement BLE reading and MQTT publishing here
    pass

if __name__ == "__main__":
    ble.initialize()
    read_ble_and_publish()
3. VPS Setup
InfluxDB
Install InfluxDB on the VPS.
Configure InfluxDB to accept data from the Raspberry Pi.
Grafana
Install Grafana on the VPS.
Set up Grafana to visualize data from InfluxDB.
Usage
Start the ESP32-S3: Ensure it is powered and running the provided code.
Start the Raspberry Pi: Ensure it is running the BLE and MQTT services.
Monitor Data: Use Grafana on the VPS to visualize the temperature and pressure data.
Future Work
Add more sensors and nodes.
Implement more complex data processing.
Expand visualization capabilities.
Contributions
Contributions are welcome! Please fork the repository and submit a pull request with your changes.

License
This project is licensed under the MIT License.

Author
Raghav Malik
