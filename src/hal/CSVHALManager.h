#ifndef CSVHALMANAGER_H
#define CSVHALMANAGER_H

#include "ISensor.h"
#include "IActuator.h"
#include <unordered_map>
#include <stdexcept>
#include <iostream>
#include <fstream>
#include <sstream>
#include <memory>
#include <vector>

class CSVHALManager {
private:
    std::vector<std::vector<std::string>> data; // CSV data
    std::unordered_map<int, std::shared_ptr<IDevice>> portDeviceMap; // Port ID -> Device pointer
    size_t sequence; // Current row index in the CSV data
    int numPorts; // Total number of ports

    // Convert vector of strings to a vector of bytes.
    std::vector<uint8_t> convertToByteVector(const std::vector<std::string>& stringVector) {
        std::vector<uint8_t> byteVector;
        for (const auto& str : stringVector) {
            for (char ch : str) {
                byteVector.push_back(static_cast<uint8_t>(ch));
            }
        }
        return byteVector;
    }

public:
    // Constructor – ensure at least one port.
    CSVHALManager(int numPorts) : sequence(0), numPorts(numPorts) {
        if (numPorts <= 0) {
            throw std::invalid_argument("Number of ports must be greater than 0.");
        }
    }
    
    // Initialise and load CSV data.
    void initialise(const std::string& filePath) {
        std::ifstream csvFile(filePath);
        if (!csvFile.is_open()) {
            throw std::runtime_error("Failed to open CSV file: " + filePath);
        }

        std::string line;
        while (std::getline(csvFile, line)) {
            std::istringstream lineStream(line);
            std::string cell;
            std::vector<std::string> row;
            while (std::getline(lineStream, cell, ',')) {
                row.push_back(cell);
            }
            data.push_back(row);
        }
        csvFile.close();
    }

    // Attach a device to a port.
    void attachDevice(int portId, const std::shared_ptr<IDevice>& device) {
        if (portId < 0 || portId >= numPorts) {
            throw std::out_of_range("Invalid port ID.");
        }
        if (portDeviceMap.find(portId) != portDeviceMap.end()) {
            throw std::runtime_error("Port is already busy.");
        }
        portDeviceMap[portId] = device;
        std::cout << "[CSVHALManager] Device attached to port " << portId << ".\n";
    }

    // Release a device from a port.
    void releaseDevice(int portId) {
        if (portDeviceMap.find(portId) == portDeviceMap.end()) {
            throw std::runtime_error("No device attached to port.");
        }
        portDeviceMap.erase(portId);
        std::cout << "[CSVHALManager] Device released from port " << portId << ".\n";
    }

    // Check if a port is busy.
    bool isBusy(int portId) const {
        return portDeviceMap.find(portId) != portDeviceMap.end();
    }

    // Read a row from the CSV file for a sensor, then use the sensor's format function
    // to produce a formatted string (e.g., with a timestamp and proper labels).
    std::string readFormatted(int portId) {
        if (portDeviceMap.find(portId) == portDeviceMap.end()) {
            throw std::runtime_error("No device attached to the specified port.");
        }

        auto sensor = std::dynamic_pointer_cast<ISensor>(portDeviceMap[portId]);
        if (!sensor) {
            throw std::runtime_error("The device attached to the port is not a sensor and cannot read data.");
        }

        if (sequence >= data.size()) {
            throw std::out_of_range("No more data available.");
        }

        std::vector<std::string> result;
        // Use sensor->getId() as the starting column index.
        for (int i = 0; i < sensor->getDimension(); ++i) {
            int columnIndex = sensor->getId() + i;
            if (columnIndex >= static_cast<int>(data[0].size())) {
                throw std::out_of_range("Column index out of range.");
            }
            result.push_back(data[sequence][columnIndex]);
        }
        sequence++; // Move to the next row for subsequent reads.
        std::vector<uint8_t> rawData = convertToByteVector(result);
        return sensor->format(rawData);
    }

    // Write data to an actuator.
    void write(int portId, const std::vector<uint8_t>& dataToWrite) {
        if (portDeviceMap.find(portId) == portDeviceMap.end()) {
            throw std::runtime_error("No device attached to port.");
        }

        auto device = portDeviceMap[portId];
        auto actuator = std::dynamic_pointer_cast<IActuator>(device);
        if (actuator) {
            actuator->send(dataToWrite);
        } else {
            throw std::runtime_error("The device attached to the port is not an actuator and cannot send data.");
        }
    }
};

#endif // CSVHALMANAGER_H
