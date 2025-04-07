#include "EbikeHandler.h"
#include <Poco/Net/HTTPServerResponse.h>
#include <Poco/Net/HTTPRequestHandler.h>
#include <fstream>
#include <sstream>

// EBikeHandler implementation.
EBikeHandler::EBikeHandler(Poco::JSON::Array::Ptr& ebikes)
    : _ebikes(ebikes)
{
}

void EBikeHandler::handleRequest(Poco::Net::HTTPServerRequest& request, 
                                 Poco::Net::HTTPServerResponse& response)
{
    // Build a simple FeatureCollection JSON from the shared eBike array.
    Poco::JSON::Object::Ptr featureCollection = new Poco::JSON::Object();
    featureCollection->set("type", "FeatureCollection");
    featureCollection->set("features", _ebikes);
    
    std::ostringstream oss;
    featureCollection->stringify(oss);

    response.setContentType("application/json");
    response.setStatus(Poco::Net::HTTPResponse::HTTP_OK);
    std::ostream& out = response.send();
    out << oss.str();
}

// FileHandler implementation for static files.
FileHandler::FileHandler(const std::string& filePath)
    : _filePath(filePath)
{
}

void FileHandler::handleRequest(Poco::Net::HTTPServerRequest& request, 
                                Poco::Net::HTTPServerResponse& response)
{
    std::ifstream file(_filePath.c_str());
    if (!file) {
        response.setStatus(Poco::Net::HTTPResponse::HTTP_NOT_FOUND);
        response.send() << "File not found.";
        return;
    }
    response.setContentType("text/html");
    response.setStatus(Poco::Net::HTTPResponse::HTTP_OK);
    std::ostream& out = response.send();
    out << file.rdbuf();
}

// RequestHandlerFactory implementation.
RequestHandlerFactory::RequestHandlerFactory(Poco::JSON::Array::Ptr& ebikes)
    : _ebikes(ebikes)
{
}

Poco::Net::HTTPRequestHandler* RequestHandlerFactory::createRequestHandler(const Poco::Net::HTTPServerRequest& request)
{
    // For simplicity, if the URI is "/ebikes", return an EBikeHandler; otherwise serve map.html.
    if (request.getURI() == "/ebikes") {
        return new EBikeHandler(_ebikes);
    } else {
        return new FileHandler("src/html/map.html");
    }
}
