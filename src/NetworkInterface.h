#ifndef SWCCSYNC_NETWORKINTERFACE_H
#define SWCCSYNC_NETWORKINTERFACE_H

#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <AsyncTCP.h>
#include "LittleFS.h"

class NetworkInterface {

    private:
        AsyncWebServer server;


        IPAddress localIP;
        //IPAddress localIP(192, 168, 1, 200); // hardcoded

        // Set your Gateway IP address
        IPAddress localGateway;
        //IPAddress localGateway(192, 168, 1, 1); //hardcoded
        IPAddress subnet;


        // Stores LED state
        String ledState;

    public:
        NetworkInterface(int port) : server(port), subnet(255, 255, 0, 0){};
        void boot();

        int data = 0;
        static void initLittleFS();

        bool testState;
        void configMode();
};


#endif //SWCCSYNC_NETWORKINTERFACE_H
