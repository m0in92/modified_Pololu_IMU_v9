#ifndef LIS3MDL_H
#define LIS3MDL_H

class LIS3MDL
{
public:
    /**
     * @brief The device can have two I2C addresses based on whether the SA1 connection pin on the device is pulled high or low.
     *
     */
    enum DeviceState
    {
        SA1_LOW = 0,
        SA1_HIGH = 1,
    };

    enum DeviceAddress
    {
        SA1_LOW_ADDRESS = B0011100,
        SA1_HIGH_ADDRESS = B0011110
    };

    enum RegAddr
    {

    };

    LIS3MDL() {};
    LIS3MDL(DeviceState i_device_state, DeviceAddress device_address);

    // Getters
    uint8_t get_device_address() { return (uint8_t)&m_device_address; }

private:
    DeviceState m_device_state = DeviceState::SA1_HIGH;
    DeviceAddress m_device_address = DeviceAddress::SA1_HIGH_ADDRESS;
};

#endif // LIS3MDL_h