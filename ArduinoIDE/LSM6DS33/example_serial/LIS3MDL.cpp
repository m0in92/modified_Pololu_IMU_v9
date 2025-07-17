#include "LIS3MDL.h"

// LIS3MDL::LIS3MDL() : bus(&Wire) {}

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