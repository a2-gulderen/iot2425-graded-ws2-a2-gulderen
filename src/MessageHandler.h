#ifndef MESSAGEHANDLER_H
#define MESSAGEHANDLER_H

#include <iostream>
#include <sstream>
#include <cstring>
#include <arpa/inet.h>
#include <Poco/JSON/Parser.h>
#include <Poco/JSON/Object.h>
#include <Poco/JSON/Stringifier.h>
#include <Poco/JSON/Array.h>

class MessageHandler {
public:
    MessageHandler(Poco::JSON::Array::Ptr& ebikes) : _ebikes(ebikes) {}

    // Processes the received JSON message, converts it into a GeoJSON Feature,
    // and adds it to the shared eBikes list.
    const char* handleMessage(const char* message, const char* clientIp, uint16_t clientPort) {
        std::cout << "Handling message from " << clientIp << ":" << clientPort 
                  << " - " << message << std::endl;
        try {
            Poco::JSON::Parser parser;
            Poco::Dynamic::Var result = parser.parse(message);
            Poco::JSON::Object::Ptr jsonObj = result.extract<Poco::JSON::Object::Ptr>();

            // Extract fields: ebike_id, timestamp, latitude, longitude
            int ebike_id = jsonObj->getValue<int>("ebike_id");
            std::string timestamp = jsonObj->getValue<std::string>("timestamp");
            double latitude = jsonObj->getValue<double>("latitude");
            double longitude = jsonObj->getValue<double>("longitude");

            // Create a GeoJSON Feature
            Poco::JSON::Object::Ptr feature = new Poco::JSON::Object();
            feature->set("type", "Feature");

            // Set geometry (GeoJSON uses [longitude, latitude])
            // Example from MessageHandler.h or handleMessage method
            Poco::JSON::Object::Ptr properties = new Poco::JSON::Object();
            properties->set("ebike_id", ebike_id);
            properties->set("timestamp", timestamp);
// If you want to track whether it’s locked/unlocked
            properties->set("status", "unlocked");

            feature->set("properties", properties);

            // Set properties with eBike ID, timestamp, and default status
            Poco::JSON::Object::Ptr properties = new Poco::JSON::Object();
            properties->set("ebike_id", ebike_id);
            properties->set("timestamp", timestamp);
            // Optional: If you want to track status or lock state
            properties->set("status", "unlocked");

            feature->set("properties", properties);

            // Add the feature to the shared eBikes list
            _ebikes->add(feature);
            std::cout << "Added GeoJSON feature for eBike " << ebike_id << std::endl;

        } catch (Poco::Exception& ex) {
            std::cerr << "JSON Parsing error: " << ex.displayText() << std::endl;
            return "ERROR";
        }
        return "OK";
    }

private:
    Poco::JSON::Array::Ptr& _ebikes;
};

#endif
