#include <Arduino.h>
#include <Wire.h>

class LSM6DS33()
{
public:
    enum DeviceState
    {
        low=0,
        high
    };

    private : DeviceState m_device_state;
    uint8_t m_device_address;
}
