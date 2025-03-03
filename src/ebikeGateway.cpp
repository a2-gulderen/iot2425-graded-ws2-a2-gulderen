#include "web/WebServer.h"
#include <iostream>

int main() {
    // Create a reference to a Poco JSON array to store the ebike objects
    Poco::JSON::Array::Ptr ebikes = new Poco::JSON::Array();

    try {
        //Replace 0 with your allocated port as per specifications.
        int port = 0;
        
        // Create instances of the server class
        WebServer webServer(ebikes);

        // Start the server 
        webServer.start(port);
    
        return 0;
    } catch (const Poco::Exception& ex) {
        std::cerr << "Server error (Poco): " << ex.displayText() << std::endl;
        return 1;
    }
    return 0;
}