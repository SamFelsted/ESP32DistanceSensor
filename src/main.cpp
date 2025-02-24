#include <Arduino.h>
#include "NetworkInterface.h"


NetworkInterface networkInterface(80);

void setup() {
    networkInterface.boot();
}

void loop() {

}