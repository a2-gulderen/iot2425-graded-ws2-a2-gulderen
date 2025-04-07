#include "WebServer.h"
#include "EbikeHandler.h"  // This header should define RequestHandlerFactory.
#include <Poco/Net/ServerSocket.h>
#include <Poco/Net/HTTPServerParams.h>
#include <Poco/Thread.h>
#include <iostream>

WebServer::WebServer(Poco::JSON::Array::Ptr& ebikes)
    : _ebikes(ebikes), _httpServer(nullptr)
{
}

WebServer::~WebServer() {
    if (_httpServer) {
        _httpServer->stop();
        delete _httpServer;
    }
}

void WebServer::start(int port)
{
    Poco::Net::ServerSocket svs(port); // use the argument
    // ...
    std::cout << "Server started on http://localhost:" << port << std::endl;
    // ...
}

    
    // Create HTTPServer parameters.
    Poco::Net::HTTPServerParams* pParams = new Poco::Net::HTTPServerParams();
    pParams->setMaxQueued(100);
    pParams->setMaxThreads(16);

    // Create and start the HTTPServer.
    // RequestHandlerFactory (defined in EbikeHandler.h) handles incoming requests.
    _httpServer = new Poco::Net::HTTPServer(new RequestHandlerFactory(_ebikes), svs, pParams);
    _httpServer->start();

    std::cout << "Server started on http://localhost:" << port << std::endl;
    std::cout << "Press Ctrl+C to stop the server..." << std::endl;

    // Block indefinitely. (You might want a better mechanism in a production system.)
    while (true) {
        Poco::Thread::sleep(1000);
    }


void WebServer::stop() {
    if (_httpServer) {
        _httpServer->stop();
    }
}
