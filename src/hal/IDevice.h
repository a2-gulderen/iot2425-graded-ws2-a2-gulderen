#ifndef IDEVICE_H
#define IDEVICE_H

class IDevice {
public:
    virtual ~IDevice() = default;
    // getId() returns the starting column index for CSV reading.
    virtual int getId() const = 0;
};

#endif // IDEVICE_H
