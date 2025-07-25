/**
 * @file LSM6DS33.h
 * @author Moin Ahmed (moinahmed100@gmail.com)
 * @brief Arduino library for reading data from the STElectronic's LSM6DS33 6-axis interial measurement unit (IMU) sensor.
 * @version 0.1
 * @date 2025-07-17
 *
 * @copyright Copyright (c) 2025
 *
 */

#ifndef LSM6DS33_H
#define LSM6DS33_H

#include <Arduino.h>
#include <Wire.h>

/**
 * @brief Accelerometer power configuration stored in binary numbers.
 *
 */
enum class LSM6DS33_ACCEL_POWER_MODES : uint8_t
{
    POWER_DOWN = 0b0000,              // Power-down mode
    LOW_POWER_13Hz = 0b0001,          // 13 Hz (Low Power mode)
    LOW_POWER_26Hz = 0b0010,          // 26 Hz (Low Power mode)
    LOW_POWER_52Hz = 0b0011,          // 52 Hz (Low Power mode)
    NORMAL_104Hz = 0b0100,            // 104 Hz (Normal mode)
    HIGH_PERFORMANCE_208Hz = 0b0101,  // 208 Hz (High-performance mode)
    HIGH_PERFORMANCE_416Hz = 0b0110,  // 416 Hz (High-performance mode)
    HIGH_PERFORMANCE_833Hz = 0b0111,  // 833 Hz (High-performance mode)
    HIGH_PERFORMANCE_1660Hz = 0b1000, // 1.66 kHz (High-performance mode)
    HIGH_PERFORMANCE_3330Hz = 0b1001, // 3.33 kHz (High-performance mode)
    HIGH_PERFORMANCE_6660Hz = 0b1010  // 6.66 kHz (High-performance mode)
};

enum class LSM6DS33_GYRO_POWER_MODES : int8_t
{
    POWER_DOWN = 0b0000,         // Gyroscope powered down
    LOW_POWER_13Hz = 0b0001,     // 13 Hz
    LOW_POWER_26Hz = 0b0010,     // 26 Hz
    LOW_POWER_52Hz = 0b0011,     // 52 Hz
    NORMAL_POWER_104Hz = 0b0100, // 104 Hz
    NORMAL_POWER_208Hz = 0b0101, // 208 Hz
    HIGH_POWER_416Hz = 0b0110,   // 416 Hz
    HIGH_POWER_833Hz = 0b0111,   // 833 Hz
    HIGH_POWER_1660Hz = 0b1000,  // 1.66 kHz
};

/**
 * @brief Register values for different available accelerometer scales on LSM6DS33
 *
 */
enum class LSM6D33_ACCEL_SCALE : uint8_t
{
    SCALE_2G = 0b00,  // ±2g
    SCALE_16G = 0b01, // ±16g
    SCALE_4G = 0b10,  // ±4g
    SCALE_8G = 0b11   // ±8g
};

/**
 * @brief Register values for different accelerometer low-pass filters
 *
 */
enum class LSM6D33_ACCEL_ANTIALIASING_BW : uint8_t
{
    BW_400Hz = 0b00, // 400 Hz
    BW_200Hz = 0b01, // 200 Hz
    BW_100Hz = 0b10, // 100 Hz
    BW_50Hz = 0b11   // 50 Hz
};

/**
 * @brief Different register addresses on LSM6DS33, for e.g., for settings and reading sensor output values
 *
 */
enum class LSM6D33_REG_ADDR : uint8_t
{
    CTRL1_XL = 0x10,
    CTRL2_G = 0x11,
    WHO_AM_I = 0x0F,

    OUT_TEMP_L = 0x20,
    OUT_TEMP_H = 0x21,

    OUTX_L_G = 0x22,
    OUTX_L_XL = 0x28,
};

class LSM6DS33
{
public:
    /**
     * @brief
     * LSM6DS33 can have two unique I2C addresses based on the electric schematic of this device.
     * The datasheet mentions the addresses of the device based on the congiruation of the SDO Pin.
     * The SA0 pin can by configured to be pulled high or pulled low to the GND.
     *
     */
    enum DeviceState
    {
        DEVICE_STATE_LOW = 0,
        DEVICE_STATE_HIGH
    };

    /**
     * @brief Device I2C addresses based on the configuration of the SA1 pin.
     *
     */
    enum DeviceI2CAddress
    {
        SA0_LOW_ADDRESS = 0b1101010,
        SA0_HIGH_ADDRESS = 0b1101011
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
    DeviceI2CAddress get_device_address() { return m_device_address; }

    // Helpers Methods

    void begin();
    void _accel_config();
    void _gyro_config();

    /**
     * @brief reads register address on the device and returns the byte stored on that address.
     *
     * @param reg register address
     * @return uint8_t data (byte sized) stored in that address
     */
    uint8_t read_register(uint8_t i_reg_addr);                                     // USEFUL if a single byte is to be read from the inputted register address.
    void read_register(uint8_t *ptr_data_array, uint8_t i_reg_addr, uint8_t size); // USEFUL if many bytes are to be read from the inputted and subsequent registers address.

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

    /**
     * @brief reads gyro variables from the relevant IMU register and stores the raw and calculated angular velocity in the
     * relevant instance variables.
     *
     */
    void read_gyro();

    // Result varables
    int temp;            // in degrees C
    Vector<int> raw_a;   // raw data from the accelerometer
    Vector<float> a;     // accelerometer values in g [g]
    Vector<int> raw_g;   // gyroscope raw data
    Vector<float> omega; // calculated angular speeds

private:
    // LSM6D33 Device Specifications
    static const float SENSITIVITY_2G = 0.061 * 1e-3;  // [LSM/g]
    static const float SENSITIVITY_4G = 0.122 * 1e-3;  // [LSM/g]
    static const float SENSITIVITY_8G = 0.244 * 1e-3;  // [LSM/g]
    static const float SENSITIVITY_16G = 0.488 * 1e-3; // [LSM/g]

    static const float TYPICAL_LINEAR_OFFSET = 0.04; // [g]

    static const float SENSITIVITY_125 = 4.375e-3; // [dps/LSM]
    static const float SENSITIVITY_425 = 8.75e-3;  // [dps/LSM]
    static const float SENSITIVITY_500 = 17.5 - 3; // [dps/LSM]
    static const float SENSITIVITY_1000 = 35e-3;   // [dps/LSM]
    static const float SENSITIVITY_2000 = 70e-3;   // [dps/LSM]

    // private member variables
    TwoWire *bus;
    DeviceState m_device_state = 0;
    DeviceI2CAddress m_device_address = SA0_HIGH_ADDRESS;

    float m_device_accel_sensitvity;

    uint8_t m_last_status;
};

#endif // LSM6DS33_H
