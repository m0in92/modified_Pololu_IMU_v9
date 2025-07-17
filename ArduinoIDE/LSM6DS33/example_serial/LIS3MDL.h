#ifndef LIS3MDL_H
#define LIS3MDL_H

#include <Arduino.h>
#include <Wire.h>

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
        WHO_AM_I = 0x0F,
        CTRL_REG1 = 0x20,
        CTRL_REG2 = 0x21,
        CTRL_REG3 = 0x22,
        CTRL_REG4 = 0x23,
        CTRL_REG5 = 0x24,
        STATUS_REG = 0x27,
        OUT_X_L = 0x28,
        OUT_X_H = 0x29,
        OUT_Y_L = 0x2A,
        OUT_Y_H = 0x2B,
        OUT_Z_L = 0x2C,
        OUT_Z_H = 0x2D,
        TEMP_OUT_L = 0x2E,
        TEMP_OUT_H = 0x2F,
        INT_CFG = 0x30,
        INT_SRC = 0x31,
        INT_THS_L = 0x32,
        INT_THS_H = 0x33
    };

    template <typename T>
    struct Vector
    {
        T x;
        T y;
        T z;
    }

    LIS3MDL();
    // LIS3MDL(DeviceState i_device_state, DeviceAddress device_address);

    void init();

    // Getters
    uint8_t get_device_address() { return m_device_address; }

    // Helper Methods
    uint8_t read_register(uint8_t i_reg_addr);
    void read_register(uint8_t *ptr_data_array, uint8_t i_reg_addr, uint8_t size);

    void read_mag();

    // Public Variable
    Vector<float> m;

private:
    DeviceState m_device_state = DeviceState::SA1_HIGH;
    DeviceAddress m_device_address = DeviceAddress::SA1_HIGH_ADDRESS;

    TwoWire bus;
};

#endif // LIS3MDL_h