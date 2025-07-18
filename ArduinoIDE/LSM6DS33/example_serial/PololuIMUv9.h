#ifndef PololuIMUv9_H
#define PololuIMUv9_H

#include <Arduino.h>
#include <Wire.h>

#include "LSM6DS33.h"
#include "LIS3MDL.h"

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

#endif // PololuIMUv9_H