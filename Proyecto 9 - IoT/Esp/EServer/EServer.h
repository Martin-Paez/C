#ifndef _E_SERVER_H
#define _E_SERVER_H

#include <WiFi.h> 

class EServer {

    private:
        WiFiServer & server;
        Stream & Serialx;
        void handler(WiFiClient &, String);
        String read(WiFiClient &client);

    public:
        EServer(Stream& serialx, int port, void handler(WiFiClient &, String));
        bool begin();
        void loop();
        void send(WiFiClient &client, String page);
};

#include "EServer.cpp"      // En Arduino es necesario aca

#endif