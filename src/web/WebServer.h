#ifndef WEBSERVER_H
#define WEBSERVER_H

#include <Poco/JSON/Array.h>
#include <Poco/Net/HTTPServer.h>
#include <string>

class WebServer {
public:
    // Constructor takes a reference to the shared JSON array of eBike features.
    WebServer(Poco::JSON::Array::Ptr& ebikes);
    ~WebServer();

    // Start the web server on the specified port.
    void start(int port);

    // Optional: stop the server.
    void stop();

private:
    Poco::JSON::Array::Ptr& _ebikes;
    Poco::Net::HTTPServer* _httpServer;
};

#endif // WEBSERVER_H
