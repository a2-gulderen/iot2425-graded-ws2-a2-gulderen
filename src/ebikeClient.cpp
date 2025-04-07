#include <iostream>
#include <chrono>
#include <iomanip>
#include <thread>
#include <sstream>
#include <cstring>
#include <arpa/inet.h>
#include "GPSSensor.h"
#include "hal/CSVHALManager.h"
#include "sim/socket.h"
#include <Poco/JSON/Object.h>
#include <Poco/JSON/Parser.h>
#include <Poco/JSON/Stringifier.h>

// Utility function for logging current timestamp
static std::string getCurrentTimestampLog() {
    auto now = std::chrono::system_clock::now();
    std::time_t now_c = std::chrono::system_clock::to_time_t(now);
    std::tm timeStruct;
#ifdef _WIN32
    localtime_s(&timeStruct, &now_c);
#else
    localtime_r(&now_c, &timeStruct);
#endif
    std::ostringstream oss;
    oss << "[" << std::put_time(&timeStruct, "%Y-%m-%d %H:%M:%S") << "]";
    return oss.str();
}

int main(int argc, char* argv[]) {
    if (argc != 5) {
        std::cerr << "Usage: " << argv[0] 
                  << " <gateway_ip> <ebike_id> <csv_file> <gateway_port>" << std::endl;
        return 1;
    }

    std::string gatewayIP = argv[1];
    std::string ebikeID = argv[2];
    std::string csvFile = argv[3];
    int gatewayPort = std::stoi(argv[4]);

    // Instantiate CSVHALManager (assuming it needs one integer argument)
    CSVHALManager halManager(1);

    // Create GPSSensor with CSV file, gateway port, and eBike ID.
    GPSSensor GPSSensor(csvFile, gatewayPort, ebikeID);
    GPSSensor.attach(gatewayPort);

    // Create a UDP socket for sending data.
    sim::socket udpSocket(AF_INET, SOCK_DGRAM, 0);
    struct sockaddr_in gatewayAddr;
    gatewayAddr.sin_family = AF_INET;
    gatewayAddr.sin_port = htons(gatewayPort);
    if (inet_pton(AF_INET, gatewayIP.c_str(), &gatewayAddr.sin_addr) <= 0) {
        std::cerr << "Invalid gateway IP address" << std::endl;
        return 1;
    }

    // Loop to send several GPS readings.
    for (int i = 0; i < 5; i++) {
        std::this_thread::sleep_for(std::chrono::seconds(5));

        std::string sensorJson = GPSSensor.readGPSData();
        if (sensorJson.empty()) {
            std::cout << "No more GPS data available." << std::endl;
            break;
        }

        // Parse sensor JSON.
        Poco::JSON::Parser parser;
        Poco::Dynamic::Var var = parser.parse(sensorJson);
        Poco::JSON::Object::Ptr sensorObj = var.extract<Poco::JSON::Object::Ptr>();

        // Create final JSON with ebike_id.
        Poco::JSON::Object::Ptr finalObj = new Poco::JSON::Object();
        finalObj->set("ebike_id", std::stoi(ebikeID));
        finalObj->set("timestamp", sensorObj->getValue<std::string>("timestamp"));
        finalObj->set("latitude", sensorObj->getValue<double>("latitude"));
        finalObj->set("longitude", sensorObj->getValue<double>("longitude"));

        std::ostringstream oss;
        finalObj->stringify(oss);
        std::string message = oss.str();

        std::cout << getCurrentTimestampLog() << " Sending JSON: " << message << std::endl;
        udpSocket.sendto(message.c_str(), message.size(), 0, gatewayAddr);

        // Wait for acknowledgment.
        char buffer[1024];
        struct sockaddr_in fromAddr;
        ssize_t received = udpSocket.recvfrom(buffer, sizeof(buffer) - 1, 0, fromAddr);
        if (received > 0) {
            buffer[received] = '\0';
            std::cout << "Received acknowledgment: " << buffer << std::endl;
        }
    }

    GPSSensor.release();
    return 0;
}
