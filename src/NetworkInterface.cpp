#include "NetworkInterface.h"


// Initialize LittleFS
void NetworkInterface::initLittleFS() {
    if (!LittleFS.begin(true)) {
        Serial.println("An error has occurred while mounting LittleFS");
    }
    Serial.println("LittleFS mounted successfully");
}

// Replaces placeholder with LED state value
String NetworkInterface::processor(const String& var) {
    if(var == "STATE") {
        if(digitalRead(ledPin)) {
            ledState = "ON";
        }
        else {
            ledState = "OFF";
        }
        return ledState;
    }
    return {};
}


void NetworkInterface::configMode() {
    Serial.println("Setting AP (Access Point)");
    // NULL sets an open Access Point
    WiFi.softAP("ESP-DISTANCESENSOR", nullptr);

    IPAddress IP = WiFi.softAPIP();
    Serial.print("AP IP address: ");
    Serial.println(IP);

    // Web Server Root URL
    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
        request->send(LittleFS, "/index.html", "text/html");
    });

    server.on("/sensor", HTTP_GET, [](AsyncWebServerRequest *request){
        float sensorValue = analogRead(34);  // Example: Read from GPIO34
        String jsonResponse = "{\"value\": " + String(sensorValue) + "}";
        request->send(200, "application/json", jsonResponse);
    });



    server.serveStatic("/", LittleFS, "/");

    server.begin();
}


void NetworkInterface::boot() {
    initLittleFS();

    // Set GPIO 2 as an OUTPUT
    pinMode(ledPin, OUTPUT);
    digitalWrite(ledPin, LOW);

    configMode();
}
