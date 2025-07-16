#ifndef LSM6DS33_H
#define LSM6DS33_H

#include <Arduino.h>
#include <Wire.h>

// LSM6DS33 can have two unique I2C addresses based on the electric schematic of this device.
// The datasheet mentions the addresses of the device based on the congiruation of the SDO Pin.
// The SA0 pin can by configured to be pulled high or pulled low to the GND.
static uint8_t SA0_HIGH_ADDRESS = 0b1101011;
static uint8_t SA0_LOW_ADDRESS = 0b1101010;

// LSM6D33 Device Address
static uint8_t CTRL1_XL = 0x10;
static uint8_t WHO_AM_I = 0x0F;

static const uint8_t OUT_TEMP_L = 0x20;
static const uint8_t OUT_TEMP_H = 0x21;

static const uint8_t OUTX_L_XL = 0x28;

// LSM6D33 Device Specifications
static const float SENSITIVITY_2G = 16393.4;     // [g/LSM]
static const float TYPICAL_LINEAR_OFFSET = 0.04; // [g]

class LSM6DS33
{
public:
    enum DeviceState
    {
        DEVICE_STATE_LOW = 0,
        DEVICE_STATE_HIGH
    };

    template <typename T>
    struct Vector
    {
        T x;
        T y;
        T z;
    };

    LSM6DS33();
    LSM6DS33(DeviceState i_device_state);
    ~LSM6DS33() {};

    // Getters

    DeviceState get_device_state() { return m_device_state; }
    uint8_t get_device_address() { return m_device_address; }

    // Helpers Methods

    void begin()
    {
        write_register(CTRL1_XL, 0x80);
    }

    /**
     * @brief reads register address on the device and returns the byte stored on that address.
     *
     * @param reg register address
     * @return uint8_t data (byte sized) stored in that address
     */
    uint8_t read_register(uint8_t i_reg_addr);

    void write_register(uint8_t i_reg_addr, uint8_t value);

    /**
     * @brief reads the temperature value from the directly from the device and updates the relevant instance variable.
     *        The temperature value is stored in the units of degrees C.
     *
     */
    void read_temp();

    /**
     * @brief reads the accelerometer values from the device and then updates the relevant instance variables.
     *        The accelerometer values in g values are also calculated and stored in the relevant instance variables.
     *
     */
    void read_accel();

    // Result varables
    int temp;          // in degrees C
    Vector<int> raw_a; // raw data from the accelerometer
    Vector<float> a;   // accelerometer values in g [g]

private:
    TwoWire *bus;
    DeviceState m_device_state = 0;
    uint8_t m_device_address = SA0_HIGH_ADDRESS;

    uint8_t m_last_status;
};

#endif // LSM6DS33_H
