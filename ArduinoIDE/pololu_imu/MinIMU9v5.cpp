#include "MinIMU9v5.h"

LSM6DS33::LSM6DS33() : bus(&Wire), m_device_accel_sensitvity(SENSITIVITY_2G)
{
}

LSM6DS33::LSM6DS33(DeviceState i_device_state) : bus(&Wire), m_device_accel_sensitvity(SENSITIVITY_2G)
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
    _accel_config();
    write_register(static_cast<uint8_t>(LSM6D33_REG_ADDR::CTRL2_G), 0x80);
}

void LSM6DS33::_accel_config()
{
    uint8_t power_mode = static_cast<uint8_t>(LSM6DS33_ACCEL_POWER_MODES::HIGH_PERFORMANCE_1660Hz);
    uint8_t scale = static_cast<uint8_t>(LSM6D33_ACCEL_SCALE::SCALE_2G);

    uint8_t data = (power_mode << 4) | (scale << 2);
    write_register(static_cast<uint8_t>(LSM6D33_REG_ADDR::CTRL1_XL), data);
}

void LSM6DS33::_gyro_config()
{
    uint8_t power_mode = static_cast<uint8_t>(LSM6DS33_GYRO_POWER_MODES::HIGH_POWER_1660Hz);
    uint8_t scale = static_cast<uint8_t>(LSM6DS33_GYRO_SCALE::SCALE_245);

    uint8_t data = (power_mode << 4) | (scale << 2);
    write_register(static_cast<uint8_t>(LSM6D33_REG_ADDR::CTRL2_G), data);
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
    bus->write(static_cast<uint8_t>(LSM6D33_REG_ADDR::OUT_TEMP_L));
    bus->endTransmission();

    bus->requestFrom(m_device_address, (uint8_t)2);
    uint8_t temp_l = bus->read();
    uint8_t temp_h = bus->read();

    // combine high and low bytes
    int temp_value = (int16_t)(temp_h << 8 | temp_l); // in F
    temp = temp_value;                                // in C
}

/**
 * @brief Reads the accelerometer values from the device.
 *
 */
void LSM6DS33::read_accel()
{
    uint8_t data_size = 6; // [bytes]
    uint8_t data_array[data_size];
    read_register(&data_array[0], static_cast<uint8_t>(LSM6D33_REG_ADDR::OUTX_L_XL), data_size);

    // combine high and low bytes
    raw_a.x = (int16_t)(data_array[1] << 8 | data_array[0]);
    raw_a.y = (int16_t)(data_array[3] << 8 | data_array[2]);
    raw_a.z = (int16_t)(data_array[5] << 8 | data_array[4]);

    // Calculate the accelerometer g values [g]
    a.x = raw_a.x * m_device_accel_sensitvity;
    a.y = raw_a.y * m_device_accel_sensitvity;
    a.z = raw_a.z * m_device_accel_sensitvity;
}

void LSM6DS33::read_gyro()
{
    uint8_t data_size = 6; // [bytes]
    uint8_t data_array[data_size];
    read_register(&data_array[0], static_cast<uint8_t>(LSM6D33_REG_ADDR::OUTX_L_G), 6);

    // combine high and low bytes
    raw_g.x = (int16_t)(data_array[1] << 8 | data_array[0]);
    raw_g.y = (int16_t)(data_array[3] << 8 | data_array[2]);
    raw_g.z = (int16_t)(data_array[5] << 8 | data_array[4]);

    // calculate angular speeds [degrees per second]
    omega.x = raw_g.x * SENSITIVITY_245;
    omega.y = raw_g.y * SENSITIVITY_245;
    omega.z = raw_g.z * SENSITIVITY_245;
}

void LIS3MDL::init()
{
    // 0x70 = 0b01110000
    // OM = 11 (ultra-high-performance mode for X and Y); DO = 100 (10 Hz ODR)
    write_reg(CTRL_REG1, 0x70);

    // 0x00 = 0b00000000
    // FS = 00 (+/- 4 gauss full scale)
    write_reg(CTRL_REG2, 0x00);

    // 0x00 = 0b00000000
    // MD = 00 (continuous-conversion mode)
    write_reg(CTRL_REG3, 0x00);

    // 0x0C = 0b00001100
    // OMZ = 11 (ultra-high-performance mode for Z)
    write_reg(CTRL_REG4, 0x0C);

    // 0x40 = 0b01000000
    // BDU = 1 (block data update)
    write_reg(CTRL_REG5, 0x40);
}

uint8_t LIS3MDL::read_reg(uint8_t i_reg_addr)
{
    uint8_t value;

    bus->beginTransmission(m_device_address);
    bus->write(i_reg_addr);
    bus->endTransmission();

    bus->requestFrom(m_device_address, (uint8_t)1);
    value = bus->read();
    return value;
}

/**
 * @brief Read the data from the inputted register and the subsequent registers
 *
 * @param ptr_data_array
 * @param i_reg_addr
 * @param size
 */
void LIS3MDL::read_reg(uint8_t *ptr_data_array, uint8_t i_reg_addr, uint8_t size)
{
    uint8_t data_array[size];

    bus->beginTransmission(m_device_address);
    bus->write(i_reg_addr);
    bus->endTransmission();

    bus->requestFrom(m_device_address, (uint8_t)size);
    for (int i = 0; i < size; i++)
    {
        data_array[i] = bus->read();
    }
}

void LIS3MDL::write_reg(uint8_t i_reg_addr, uint8_t value)
{
    bus->beginTransmission(m_device_address);
    bus->write(i_reg_addr);
    bus->write(value);
    m_last_status = bus->endTransmission();
}

void LIS3MDL::read_mag()
{
    const uint8_t DATA_SIZE = 6;
    Wire.beginTransmission(m_device_address);
    // assert MSB to enable subaddress updating
    Wire.write(OUT_X_L | 0x80);
    Wire.endTransmission();

    Wire.requestFrom(m_device_address, (uint8_t)DATA_SIZE);
    uint8_t xlm = Wire.read();
    uint8_t xhm = Wire.read();
    uint8_t ylm = Wire.read();
    uint8_t yhm = Wire.read();
    uint8_t zlm = Wire.read();
    uint8_t zhm = Wire.read();

    // combine high and low bytes
    m_raw.x = (int16_t)(xhm << 8 | xlm);
    m_raw.y = (int16_t)(yhm << 8 | ylm);
    m_raw.z = (int16_t)(zhm << 8 | zlm);

    // Calculate the magnetic flux [guass]
    m.x = m_raw.x / MAG_SENSITIVITY_4GUASS;
    m.y = m_raw.y / MAG_SENSITIVITY_4GUASS;
    m.z = m_raw.z / MAG_SENSITIVITY_4GUASS;
}
