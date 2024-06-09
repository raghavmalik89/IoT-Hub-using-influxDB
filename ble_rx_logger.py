import os
import csv
import asyncio
from datetime import datetime
from bleak import BleakScanner

log_dir = os.path.expanduser("~/iot-project/log")

if not os.path.exists(log_dir):
    os.makedirs(log_dir)

def log_data(temperature, pressure):
    current_time = datetime.now()
    log_filename = current_time.strftime("%d%m%y_%H%M.csv")
    log_filepath = os.path.join(log_dir, log_filename)

    if os.path.exists(log_filepath) and os.path.getsize(log_filepath) >= 1024 * 1024:
        log_filename = current_time.strftime("%d%m%y_%H%M%S.csv")
        log_filepath = os.path.join(log_dir, log_filename)

    file_exists = os.path.isfile(log_filepath)
    with open(log_filepath, 'a', newline='') as csvfile:
        fieldnames = ['date', 'time', 'temperature (C)', 'pressure (hPa)']
        writer = csv.DictWriter(csvfile, fieldnames=fieldnames)
        if not file_exists:
            writer.writeheader()
        writer.writerow({
            'date': current_time.strftime("%d/%m/%Y"),
            'time': current_time.strftime("%H:%M:%S"),
            'temperature (C)': temperature,
            'pressure (hPa)': pressure
        })

async def scan_ble():
    def detection_callback(device, advertisement_data):
        if advertisement_data.local_name == "IoT Node 1":
            print(f"Device {device.address} ({advertisement_data.local_name}), RSSI={device.rssi} dB")
            temperature = 25.5  # Example value, replace with actual data extraction logic
            pressure = 1013.5    # Example value, replace with actual data extraction logic
            log_data(temperature, pressure)

    scanner = BleakScanner()
    scanner.register_detection_callback(detection_callback)

    await scanner.start()
    await asyncio.sleep(10.0)
    await scanner.stop()

if __name__ == "__main__":
    asyncio.run(scan_ble())
