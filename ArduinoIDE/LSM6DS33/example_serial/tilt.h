#ifndef TILT_H
#define TILT_H

#include <math.h>

// constants declarations
// const float PI = 3.14159;
const float TO_DEG = 360 / (2 * PI);

struct TiltData
{
    float roll;  // [degrees]
    float pitch; // [degrees]
};

/**
 * @brief calculates the roll and pitch from the accelerometer. It assumes that the following rotational axis:
 * x axis : rotation of axis for the roll
 * y_axis: rotation of axis for the pitch
 *
 * @param a_x
 * @param a_y
 * @param a_z
 * @return TiltData
 */
TiltData tilt_from_accelerometer(float a_x, float a_y, float a_z)
{
    TiltData data;
    data.roll = atan2(a_x, a_z) * TO_DEG;
    data.pitch = atan2(a_y, a_z) * TO_DEG;

    return data;
}

#endif // TILT_H