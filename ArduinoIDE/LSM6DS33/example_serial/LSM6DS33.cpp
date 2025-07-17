#include "LSM6DS33.h"

LSM6DS33::LSM6DS33() : bus(&Wire)
{
}

LSM6DS33::LSM6DS33(DeviceState i_device_state) : bus(&Wire)
{
    m_device_state = i_device_state;
    if (m_device_state == DeviceState::DEVICE_STATE_HIGH)
    {
        m_device_address = DeviceI2CAddress::SA0_HIGH_ADDRESS;
    }
    else if (m_device_state == DeviceState::DEVICE_STATE_LOW)
    {
        m_device_address = DeviceI2CAddress::SA0_LOW_ADDRESS;
    }
    else
    {
        // TO DO: throw an exception here.
    }
}

/**
 * @brief initialization for the IMU device. Writes the relevant configurations/settings to the relevant registers of the IMU
 *
 */
void LSM6DS33::begin()
{
    write_register(RegAddr::CTRL1_XL, 0x80);
    write_register(RegAddr::CTRL2_G, 0x80);
}

/**
 * @brief reads and returns the byte stored in the input register address.
 *        USEFUL if a single byte of data is to be read from the input register.
 *
 * @param i_reg_addr device register address
 * @return uint8_t byte stored in the required register address
 */
uint8_t LSM6DS33::read_register(uint8_t i_reg_addr)
{
    uint8_t value;

    bus->beginTransmission(m_device_address);
    bus->write(i_reg_addr);
    bus->endTransmission();

    bus->requestFrom(m_device_address, (uint8_t)1);
    value = bus->read();
    return value;
}

void LSM6DS33::read_register(uint8_t *ptr_data_array, uint8_t i_reg_addr, uint8_t size)
{
    bus->beginTransmission(m_device_address);
    bus->write(i_reg_addr);
    bus->endTransmission();

    bus->requestFrom(m_device_address, (uint8_t)size);
    for (int i = 0; i < size; i++)
    {
        *(ptr_data_array + i) = bus->read();
    }
}

void LSM6DS33::write_register(uint8_t i_reg_addr, uint8_t value)
{
    bus->beginTransmission(m_device_address);
    bus->write(i_reg_addr);
    bus->write(value);
    m_last_status = bus->endTransmission();
}

/** read_temp
 * @brief reads the temperature values from the device from its relevent registers.
 *
 */
void LSM6DS33::read_temp()
{
    bus->beginTransmission(m_device_address);
    // automatic increment of register address is enabled by default (IF_INC in CTRL3_C)
    bus->write(RegAddr::OUT_TEMP_L);
    bus->endTransmission();

    bus->requestFrom(m_device_address, (uint8_t)2);
    uint8_t temp_l = bus->read();
    uint8_t temp_h = bus->read();

    // combine high and low bytes
    int temp_value = (int16_t)(temp_h << 8 | temp_l); // in F
    temp = temp_value / 16.0 + 25.0;                  // in C
}

/**
 * @brief Reads the accelerometer values from the device.
 *
 */
void LSM6DS33::read_accel()
{
    uint8_t data_size = 6; // [bytes]
    uint8_t data_array[data_size];
    read_register(&data_array[0], RegAddr::OUTX_L_XL, data_size);

    // combine high and low bytes
    raw_a.x = (int16_t)(data_array[1] << 8 | data_array[0]);
    raw_a.y = (int16_t)(data_array[3] << 8 | data_array[2]);
    raw_a.z = (int16_t)(data_array[5] << 8 | data_array[4]);

    // Calculate the accelerometer g values [g]
    a.x = raw_a.x / SENSITIVITY_2G;
    a.y = raw_a.y / SENSITIVITY_2G;
    a.z = raw_a.z / SENSITIVITY_2G;
}

void LSM6DS33::read_gyro()
{
    uint8_t data_size = 6; // [bytes]
    uint8_t data_array[data_size];
    read_register(&data_array[0], RegAddr::OUTX_L_G, 6);

    // combine high and low bytes
    raw_g.x = (int16_t)(data_array[1] << 8 | data_array[0]);
    raw_g.y = (int16_t)(data_array[3] << 8 | data_array[2]);
    raw_g.z = (int16_t)(data_array[5] << 8 | data_array[4]);

    // calculate angular speeds [degrees per second]
    omega.x = raw_g.x * SENSITIVITY_125;
    omega.y = raw_g.y * SENSITIVITY_125;
    omega.z = raw_g.z * SENSITIVITY_125;
}
