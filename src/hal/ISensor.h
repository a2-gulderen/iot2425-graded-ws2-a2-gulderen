#ifndef ISENSOR_H
#define ISENSOR_H

#include "IDevice.h"
#include <vector>
#include <string>

class ISensor : public IDevice {
public:
    virtual ~ISensor() = default;
    // Returns the number of data items (columns) the sensor uses.
    virtual int getDimension() const = 0;
    // Format the raw byte data (read from CSVHALManager) into a string output.
    virtual std::string format(const std::vector<uint8_t>& reading) = 0;
};

#endif // ISENSOR_H
