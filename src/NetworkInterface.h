#ifndef SWCCSYNC_NETWORKINTERFACE_H
#define SWCCSYNC_NETWORKINTERFACE_H

#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <AsyncTCP.h>
#include "LittleFS.h"

class NetworkInterface {

    private:
        AsyncWebServer server;

        String ssid;
        String pass;
        String ip;
        String gateway;


        IPAddress localIP;
        //IPAddress localIP(192, 168, 1, 200); // hardcoded

        // Set your Gateway IP address
        IPAddress localGateway;
        //IPAddress localGateway(192, 168, 1, 1); //hardcoded
        IPAddress subnet;

        // Timer variables
        unsigned long previousMillis = 0;
        const long interval = 10000;  // interval to wait for Wi-Fi connection (milliseconds)

        // Set LED GPIO
        const int ledPin = 2;
        // Stores LED state
        String ledState;

    public:
        NetworkInterface(int port) : server(port), subnet(255, 255, 0, 0){};
        void boot();

        static void initLittleFS();

        String processor(const String &var);

        bool testState;
        void configMode();
};


#endif //SWCCSYNC_NETWORKINTERFACE_H
