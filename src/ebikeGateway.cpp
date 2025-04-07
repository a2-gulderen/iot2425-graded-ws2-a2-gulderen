#include "web/WebServer.h"
#include "SocketServer.h"
#include <Poco/JSON/Array.h>
#include <iostream>
#include <thread>

int main() {
    // Create a shared JSON array for eBike features.
    Poco::JSON::Array::Ptr ebikes = new Poco::JSON::Array();
    int webPort = 8080; // or 9853, whichever you want
    WebServer webServer(ebikes);
    webServer.start(webPort);


    int udpPort = 8081;   // UDP socket server port

    // Create one instance of SocketServer using udpPort.
    SocketServer socketServer(ebikes, udpPort);
    socketServer.start();

    // Create one instance of WebServer.
    WebServer webServer(ebikes);
    webServer.start(webPort);

    return 0;
}
