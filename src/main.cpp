#include <Arduino.h>
#include "NetworkInterface.h"
#include <Wire.h>

#include <Adafruit_VL53L0X.h>

Adafruit_VL53L0X lox = Adafruit_VL53L0X();

NetworkInterface networkInterface(80);


void scanI2C() {
    Serial.println("Scanning I2C...");
    byte count = 0;
    for (byte i = 8; i < 120; i++) {
        Wire.beginTransmission(i);
        if (Wire.endTransmission() == 0) {
            Serial.print("I2C device found at 0x");
            Serial.println(i, HEX);
            count++;
        }
    }
    if (count == 0) Serial.println("No I2C devices found!");
}


void setup() {
    Serial.begin(115200);
    Wire.begin(21, 22);
    scanI2C();

    networkInterface.boot();

    // wait until serial port opens for native USB devices
    while (!Serial) {
        delay(1);
    }

    Serial.println("Adafruit VL53L0X test");

    delay(1000);
    if (!lox.begin()) {
        Serial.println(F("Failed to boot VL53L0X"));
        delay(1);
    }
    Serial.println(uxTaskGetStackHighWaterMark(NULL));  // Print free stack space

    Serial.println("VL53L0X ready");
}



void loop() {
    VL53L0X_RangingMeasurementData_t measure;

    Serial.print("Reading a measurement... ");
    lox.rangingTest(&measure, false); // pass in 'true' to get debug data printout!

    if (measure.RangeStatus != 4) {  // phase failures have incorrect data
        Serial.print("Distance (mm): "); Serial.println(measure.RangeMilliMeter);
        networkInterface.data = measure.RangeMilliMeter;
    } else {
        Serial.println(" out of range ");
    }

    delay(100);

}
