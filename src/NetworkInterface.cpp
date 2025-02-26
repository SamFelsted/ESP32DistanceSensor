#include "NetworkInterface.h"


// Initialize LittleFS
void NetworkInterface::initLittleFS() {
    if (!LittleFS.begin(true)) {
        Serial.println("An error has occurred while mounting LittleFS");
    }
    Serial.println("LittleFS mounted successfully");
}


void NetworkInterface::configMode() {
    Serial.println("Setting AP (Access Point)");
    // NULL sets an open Access Point
    WiFi.softAP("ESP", nullptr);

    IPAddress IP = WiFi.softAPIP();
    Serial.print("AP IP address: ");
    Serial.println(IP);

    // Web Server Root URL
    server.on("/", HTTP_GET, [this](AsyncWebServerRequest *request){
        request->send(LittleFS, "/index.html", "text/html");
    });

    server.on("/sensor", HTTP_GET, [this](AsyncWebServerRequest *request){
        String jsonResponse = "{\"value\": " + String(data) + "}";
        request->send(200, "application/json", jsonResponse);
    });


    server.serveStatic("/", LittleFS, "/").setDefaultFile("index.html");

    server.begin();
}


void NetworkInterface::boot() {
    initLittleFS();

    configMode();
}
