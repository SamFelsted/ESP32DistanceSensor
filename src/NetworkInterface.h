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

        double pos = 0;


    public:
        NetworkInterface(int port) : server(port), subnet(255, 255, 0, 0){};
        void boot();

        void addData(double d);
        static void initLittleFS();

        void activeMode();
};


#endif //SWCCSYNC_NETWORKINTERFACE_H
