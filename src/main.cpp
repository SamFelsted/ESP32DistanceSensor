#include <Arduino.h>
#include "NetworkInterface.h"
#include <Wire.h>
#include "movingAverage.h"

#include <Adafruit_VL53L0X.h>

//API objects
Adafruit_VL53L0X lox = Adafruit_VL53L0X();
NetworkInterface networkInterface(80);
VL53L0X_RangingMeasurementData_t measure;


//Settings
const int window_size = 10;
MovingAverage filter(window_size);


/**
 * Scans IC bus and prints addresses
 */
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
    Wire.setClock(50000);
    scanI2C();

    networkInterface.boot();


    Serial.println("Adafruit VL53L0X test.");

    delay(1000);
    if (!lox.begin(0x29, true)) {
        Serial.println(F("Failed to boot VL53L0X"));
        while(true);
    }

    Serial.println(F("VL53L0X API Continuous Ranging example\n\n"));

    //lox.setDeviceMode(Adafruit_VL53L0X::VL53L0X_SENSE_LONG_RANGE);
    lox.configSensor(Adafruit_VL53L0X::VL53L0X_SENSE_LONG_RANGE);


    //lox.startRangeContinuous();
    lox.setMeasurementTimingBudgetMicroSeconds(200000);  // 200ms for better range detection

    Serial.println(uxTaskGetStackHighWaterMark(nullptr));  // Print free stack space



    Serial.println("VL53L0X ready");
}



void loop() {
    lox.rangingTest(&measure, false); // pass in 'true' to get debug data printout!

    int value =  measure.RangeMilliMeter;
    double avg = filter.update(value);

    Serial.println(avg);
    networkInterface.addData(avg);

    delay(100);

}
