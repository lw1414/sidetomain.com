#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEScan.h>
#include <BLEAdvertisedDevice.h>
#include <vector>

int scanTime = 5;  // In seconds
BLEScan* pBLEScan;

const std::vector<std::string> desiredAddresses = {
  "10:06:1c:07:d7:52",  // Desired device address 1
  "58:d2:9a:4c:9f:01",
  "4e:e7:64:60:33:5e",
  "ed:37:72:11:7f:3b"  // Desired device address 2, replace with your address
  // Add more addresses as needed
};

class MyAdvertisedDeviceCallbacks : public BLEAdvertisedDeviceCallbacks {
    void onResult(BLEAdvertisedDevice advertisedDevice) {
      std::string deviceAddress = advertisedDevice.getAddress().toString();

      for (const auto& desiredAddress : desiredAddresses) {
        if (deviceAddress == desiredAddress) {
          Serial.println("Advertised Device:");
          Serial.printf("  Name: %s\n", advertisedDevice.getName().c_str());
          Serial.printf("  Address: %s\n", deviceAddress.c_str());
          Serial.printf("  TxPower: %d\n", advertisedDevice.getTXPower());
          Serial.printf("  RSSI: %d\n", advertisedDevice.getRSSI());

          if (advertisedDevice.haveServiceData()) {
            Serial.print("  Service Data (Raw): ");

            const uint8_t* serviceData = reinterpret_cast<const uint8_t*>(advertisedDevice.getServiceData().data());
            size_t serviceDataLength = advertisedDevice.getServiceData().length();

            for (int i = 0; i < serviceDataLength; i++) {
              Serial.printf("%02X ", serviceData[i]);
            }
            Serial.println();
            Serial.printf("  Service Data Length: %d\n", serviceDataLength);
          }
          Serial.println("----------");
          break;  // Break the loop if matched to avoid duplicate entries
        }
      }
    }
};

void setup() {
  Serial.begin(115200);
  Serial.println("Scanning...");

  BLEDevice::init("");
  pBLEScan = BLEDevice::getScan();  // Create new scan
  pBLEScan->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks());
  pBLEScan->setActiveScan(true);  // Active scan uses more power, but gets results faster
  pBLEScan->setInterval(100);
  pBLEScan->setWindow(99);  // Less or equal setInterval value
}

void loop() {
  BLEScanResults foundDevices = pBLEScan->start(scanTime, false);
  Serial.print("Devices found: ");
  Serial.println(foundDevices.getCount());
  Serial.println("Scan done!");
  pBLEScan->clearResults();  // Delete results from BLEScan buffer to release memory
  delay(2000);
}
