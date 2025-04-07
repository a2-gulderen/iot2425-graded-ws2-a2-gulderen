#include <iostream>
#include <sstream>
#include <chrono>
#include <iomanip>
#include <vector>
#include <cstring>
#include <arpa/inet.h>
#include <unistd.h> // for close()
#include <Poco/JSON/Object.h>
#include <Poco/JSON/Array.h>
#include <Poco/JSON/Stringifier.h>

// Utility function to get current timestamp in ISO format.
static std::string getTimestamp() {
    auto now = std::chrono::system_clock::now();
    std::time_t now_c = std::chrono::system_clock::to_time_t(now);
    char buf[30];
    std::tm timeStruct;
#ifdef _WIN32
    localtime_s(&timeStruct, &now_c);
#else
    localtime_r(&now_c, &timeStruct);
#endif
    std::strftime(buf, sizeof(buf), "%Y-%m-%dT%H:%M:%SZ", &timeStruct);
    return std::string(buf);
}

int main(int argc, char* argv[]) {
    if (argc < 5) {
        std::cerr << "Usage: " << argv[0] << " <gateway_ip> <directive> <action> <ebike_id1> [ebike_id2 ...]" << std::endl;
        return 1;
    }

    std::string gatewayIP = argv[1];
    std::string directive = argv[2]; // e.g., "COMMAND"
    std::string action = argv[3];    // e.g., "lock" or "unlock"

    // Gather ebike IDs from the command-line arguments.
    std::vector<int> ebike_ids;
    for (int i = 4; i < argc; i++) {
        ebike_ids.push_back(std::stoi(argv[i]));
    }

    // Build the JSON message.
    Poco::JSON::Object::Ptr jsonObj = new Poco::JSON::Object();
    jsonObj->set("directive", directive);
    jsonObj->set("timestamp", getTimestamp());
    jsonObj->set("action", action);

    Poco::JSON::Array::Ptr idArray = new Poco::JSON::Array();
    for (auto id : ebike_ids) {
        idArray->add(id);
    }
    jsonObj->set("ebike_ids", idArray);

    std::ostringstream oss;
    jsonObj->stringify(oss);
    std::string message = oss.str();

    // Prepare the UDP socket.
    int port = 8080; // Make sure this matches the gateway's UDP port.
    struct sockaddr_in gatewayAddr;
    gatewayAddr.sin_family = AF_INET;
    gatewayAddr.sin_port = htons(port);
    if (inet_pton(AF_INET, gatewayIP.c_str(), &gatewayAddr.sin_addr) <= 0) {
        std::cerr << "Invalid gateway IP address" << std::endl;
        return 1;
    }

    // Create a UDP socket.
    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        std::cerr << "Error creating socket" << std::endl;
        return 1;
    }

    // Send the JSON message.
    ssize_t sent = sendto(sockfd, message.c_str(), message.size(), 0,
                          (struct sockaddr*)&gatewayAddr, sizeof(gatewayAddr));
    if (sent < 0) {
        std::cerr << "Error sending message" << std::endl;
        close(sockfd);
        return 1;
    }
    std::cout << "Sent message: " << message << std::endl;

    close(sockfd);
    return 0;
}
