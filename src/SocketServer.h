#ifndef SOCKETSERVER_H
#define SOCKETSERVER_H

#include <thread>
#include <atomic>
#include "sim/socket.h"
#include "MessageHandler.h"
#include <Poco/JSON/Array.h>
#include <iostream>
#include <cstring>
#include <arpa/inet.h>

class SocketServer {
public:
    // Constructor with default port 8080 if not provided
    SocketServer(Poco::JSON::Array::Ptr& ebikes, int port = 8080)
        : ebikeList(ebikes), serverPort(port), running(false) {
        // Create and bind the UDP socket (using the simulated socket API)
        udpSocket = new sim::socket(AF_INET, SOCK_DGRAM, 0);
        struct sockaddr_in addr;
        addr.sin_family = AF_INET;
        addr.sin_port = htons(serverPort);
        addr.sin_addr.s_addr = INADDR_ANY;  // Bind to all interfaces
        udpSocket->bind(addr);
    }

    ~SocketServer() {
        stop();
        delete udpSocket;
    }

    void start() {
        running = true;
        serverThread = std::thread(&SocketServer::run, this);
    }

    void stop() {
        running = false;
        if (serverThread.joinable()) {
            serverThread.join();
        }
    }

private:
    // Reorder members to match the initializer list
    Poco::JSON::Array::Ptr& ebikeList;
    int serverPort;
    std::atomic<bool> running;
    sim::socket* udpSocket;
    std::thread serverThread;

    void run() {
        std::cout << "Socket Server waiting for messages..." << std::endl;
        char buffer[1024];
        struct sockaddr_in clientAddr;
        while (running) {
            ssize_t received = udpSocket->recvfrom(buffer, sizeof(buffer) - 1, 0, clientAddr);
            if (received > 0) {
                buffer[received] = '\0';
                std::cout << "Received message: " << buffer << std::endl;
                // Process message and convert to a GeoJSON feature
                MessageHandler handler(ebikeList);
                const char* response = handler.handleMessage(buffer,
                                                               inet_ntoa(clientAddr.sin_addr),
                                                               ntohs(clientAddr.sin_port));
                // Send acknowledgment back to the client
                udpSocket->sendto(response, strlen(response), 0, clientAddr);
            }
        }
    }
};

#endif
