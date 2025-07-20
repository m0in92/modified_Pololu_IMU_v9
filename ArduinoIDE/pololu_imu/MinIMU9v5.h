/**
 * @file MinIMU9v5.h
 * @author Moin Ahmed (moinahmed100@gmail.com)
 * @brief Code for obtaining sensor readings from the 9-axis inertial measurement unit (IMU) by Pololu (MinIMU9v5)
 * @version 0.1
 * @date 2025-07-20
 *
 * @copyright Copyright (c) 2025
 *
 */

#ifndef MinIMU9v5_H
#define MinIMU9v5_H

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

enum class LSM6DS33_GYRO_SCALE : uint8_t
{
    SCALE_245 = 0b00,  // [dps]
    SCALE_500 = 0b01,  // [dps]
    SCALE_1000 = 0b10, // [dps]
    SCALE_2000 = 0b11, // [dps]
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
    static const float SENSITIVITY_245 = 8.75e-3;  // [dps/LSM]
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
    };

    LIS3MDL() : bus(&Wire) { m_device_sensitivity = MAG_SENSITIVITY_4GUASS; }
    // LIS3MDL(DeviceState i_device_state, DeviceAddress device_address);
    ~LIS3MDL() {}

    void init();

    // Getters
    uint8_t get_device_address() { return m_device_address; }

    // Helper Methods
    uint8_t read_reg(uint8_t i_reg_addr);
    void read_reg(uint8_t *ptr_data_array, uint8_t i_reg_addr, uint8_t size);
    void write_reg(uint8_t i_reg_addr, uint8_t value);

    void read_mag();

    // Public Variable
    Vector<int> m_raw;
    Vector<float> m;

private:
    DeviceState m_device_state = DeviceState::SA1_HIGH;
    DeviceAddress m_device_address = DeviceAddress::SA1_HIGH_ADDRESS;
    float m_device_sensitivity; //[LSB/guass]

    TwoWire *bus;

    uint8_t m_last_status;

    // Device Specifications
    const float MAG_SENSITIVITY_4GUASS = 6842;
    const float MAG_SENSITIVITY_8GUASS = 3421;
    const float MAG_SENSITIVITY_12GUASS = 2281;
    const float MAG_SENSITIVITY_16GUASS = 1711;
};

class PololuIMUv9
{
public:
    struct ResultVector
    {
        float x;
        float y;
        float z;
    };

    PololuIMUV9() {}
    ~PololuIMUv9() {}

    bool init()
    {
        m_LSM6DS33.begin();
        m_LIS3MDL.init();
    }

    void read_accel()
    {
        m_LSM6DS33.read_accel();
        a.x = m_LSM6DS33.a.x;
        a.y = m_LSM6DS33.a.y;
        a.z = m_LSM6DS33.a.z;
    }

    void read_gyro()
    {
        m_LSM6DS33.read_gyro();
        g.x = m_LSM6DS33.omega.x;
        g.y = m_LSM6DS33.omega.y;
        g.z = m_LSM6DS33.omega.z;
    }

    void read_mag()
    {
        m_LIS3MDL.read_mag();
        m.x = m_LIS3MDL.m.x;
        m.y = m_LIS3MDL.m.y;
        m.z = m_LIS3MDL.m.z;
    }

    void read()
    {
        read_accel();
        read_gyro();
        read_mag();
    }

    // Private Variables
    ResultVector a; // accelerometer result vector [g]
    ResultVector g; // gyroscope result vector [degrees]
    ResultVector m; // magnometer result vector [guass]

private:
    LSM6DS33 m_LSM6DS33;
    LIS3MDL m_LIS3MDL;
};

#endif // MinIMU9v5_H