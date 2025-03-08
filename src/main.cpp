#include <Arduino.h>
#include "NetworkInterface.h"
#include <Wire.h>

#include <Adafruit_VL53L0X.h>

//API objects
Adafruit_VL53L0X lox = Adafruit_VL53L0X();
NetworkInterface networkInterface(80);
VL53L0X_RangingMeasurementData_t measure;


//Settings
const int numAverages = 10;
int averagePos[numAverages];
int averageVel[numAverages];
int timeStep = 5;


//Calculators
int averageCtr = 0;
int prevPos = 0;

/**
 * Moving average function
 * @param arr
 * @param size
 * @return
 */
float average(int arr[], int size) {
    int sum = 0;
    for (int i = 0; i < size; i++) {
        sum += arr[i];
    }
    return sum / size;
}


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
    int delta = (value - prevPos) / timeStep;
    prevPos = value;

    if (averageCtr < numAverages) {
        averagePos[averageCtr] = value;      //Change to whatever the current sensor value is
        averageVel[averageCtr] = delta;
        averageCtr++;
    } else {
        // Calculate average
        float posAvg = average(averagePos, numAverages);
        float velAvg = average(averageVel, numAverages);

        Serial.print("Distance (mm): ");
        Serial.println(posAvg);


        Serial.print("Velocity (mm/s): ");
        Serial.println(velAvg);
        networkInterface.addData(posAvg, velAvg);


        // Reset counter
        averageCtr = 0;

        // Directly overwrite the first element to start a new cycle
        averagePos[averageCtr] = value;
        averageCtr++;
    }




    delay(timeStep);

}
