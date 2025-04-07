#ifndef GPSSENSOR_H
#define GPSSENSOR_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <chrono>
#include <ctime>
#include "hal/ISensor.h"  // Assumed to define: virtual int getId() const = 0; and virtual int getDimension() const = 0;

class GPSSensor : public ISensor {
private:
    std::ifstream gpsFile;
    // You can remove sensorID if unused.
    std::string ebikeID;  // We'll use this to return the eBike ID as an int.
    int port;           // The port provided during construction (if needed)

public:
    // Constructor: takes the CSV file path, a port, and the eBike ID (as a string).
    GPSSensor(const std::string &filePath, int portNumber, const std::string &ebikeID)
      : gpsFile(filePath), ebikeID(ebikeID), port(portNumber)
    {
        if (!gpsFile.is_open()) {
            throw std::runtime_error("Failed to open GPS data file: " + filePath);
        }
    }

    ~GPSSensor() {
        if (gpsFile.is_open())
            gpsFile.close();
    }

    // Implement the base interface's function.
    // Assuming the base expects: virtual int getId() const = 0;
    int getId() const override {
        return std::stoi(ebikeID);
    }

    int getDimension() const override { 
        return 2; 
    }
    
    // Reads one GPS record from the CSV file and returns a JSON string.
    std::string readGPSData() {
        if (!gpsFile.good() || gpsFile.eof()) return "";
        std::string line;
        std::getline(gpsFile, line);
        if (line.empty()) return "";
        std::istringstream iss(line);
        std::string latStr, lonStr;
        if (!std::getline(iss, latStr, ',') || !std::getline(iss, lonStr))
            return "";
        double latitude = std::stod(latStr);
        double longitude = std::stod(lonStr);

        // Build current timestamp in ISO format.
        auto now = std::chrono::system_clock::now();
        std::time_t now_c = std::chrono::system_clock::to_time_t(now);
        std::tm timeStruct;
#ifdef _WIN32
        localtime_s(&timeStruct, &now_c);
#else
        localtime_r(&now_c, &timeStruct);
#endif
        char timeStr[30];
        std::strftime(timeStr, sizeof(timeStr), "%Y-%m-%dT%H:%M:%SZ", &timeStruct);

        // Create a JSON string with timestamp, latitude, and longitude.
        std::ostringstream oss;
        oss << "{"
            << "\"timestamp\": \"" << timeStr << "\", "
            << "\"latitude\": " << latitude << ", "
            << "\"longitude\": " << longitude
            << "}";
        return oss.str();
    }

    // If your base class does not define attach and release, remove override.
    void attach(int port) { 
        // Implement if needed.
    }

    void release() { 
        // Implement if needed.
    }
};

#endif // GPSSENSOR_H
