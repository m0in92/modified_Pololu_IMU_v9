#include "LIS3MDL.h"

LIS3MDL::LIS3MDL() : bus(&Wire) {}

void LIS3MDL::init()
{
    // 0x70 = 0b01110000
    // OM = 11 (ultra-high-performance mode for X and Y); DO = 100 (10 Hz ODR)
    write_register(CTRL_REG1, 0x70);

    // 0x00 = 0b00000000
    // FS = 00 (+/- 4 gauss full scale)
    write_register(CTRL_REG2, 0x00);

    // 0x00 = 0b00000000
    // MD = 00 (continuous-conversion mode)
    write_register(CTRL_REG3, 0x00);

    // 0x0C = 0b00001100
    // OMZ = 11 (ultra-high-performance mode for Z)
    write_register(CTRL_REG4, 0x0C);

    // 0x40 = 0b01000000
    // BDU = 1 (block data update)
    write_register(CTRL_REG5, 0x40);
}

uint8_t LIS3MDL::read_register(uint8_t i_reg_addr)
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
void LIS3MDL::read_register(uint8_t ptr_data_array, uint8_t i_reg_addr, uint8_t size)
{
    uint8_t data_array[size];

    bus->beginTransmission(m_device_address);
    bus->write(i_reg_addr);
    bus->endTransmission();
}

void LIS3MDL::read_mag()
{
    const uint8_t DATA_SIZE = 6;
    uint8_t data_array[DATA_SIZE];

    read_register(&data_array[0], OUT_X_L, DATA_SIZE);

    m.x = (data_array[1] << 8) | data_array[0];
    m.x = (data_array[3] << 8) | data_array[2];
    m.x = (data_array[5] << 8) | data_array[4];
}