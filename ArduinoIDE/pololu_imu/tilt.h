#ifndef TILT_H
#define TILT_H

#include <Arduino.h>
#include <math.h>

#include "PololuIMUv9.h"

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
TiltData tilt_from_accelerometer(PololuIMUv9 *imu)
{
    TiltData data;
    data.roll = atan2(imu->a.x, imu->a.z) * TO_DEG;
    data.pitch = atan2(imu->a.y, imu->a.z) * TO_DEG;

    return data;
}

namespace filter
{
    float _calc_avg(float *ptr_data_array, int SIZE)
    {
        float sum = 0.0;

        for (int i = 0; i < SIZE; i++)
        {
            sum += *(ptr_data_array + i);
        }
        return sum / (float)SIZE;
    }

    TiltData filtered_tilt_accel(PololuIMUv9 *imu, int SIZE)
    {
        float sum_roll = 0;
        float sum_pitch = 0;
        TiltData data;

        for (int i = 0; i < SIZE; i++)
        {
            imu->read();
            data = tilt_from_accelerometer(imu);

            sum_roll += data.roll;
            sum_pitch += data.pitch;

            delay(100);
        }

        TiltData result;
        result.roll = sum_roll / (float)SIZE;
        result.pitch = sum_pitch / (float)SIZE;

        return result;
    }

    TiltData m_avg_accel(PololuIMUv9 *imu, TiltData *data, int SIZE)
    {
        TiltData data_new;
        TiltData result;

        // move the data to create space for the new measurement
        for (int i = 0; i < (SIZE - 1); i++)
        {
            *(data + i) = *(data + (i + 1));
        }

        // take imu reading and store the data to the arrays
        imu->read();
        data_new = tilt_from_accelerometer(imu);
        *(data + (SIZE - 1)) = data_new;

        // Calculate average and return the average
        result.roll = _calc_avg(&data->roll, SIZE);
        result.pitch = _calc_avg(&data->pitch, SIZE);

        return result;
    }
};

#endif // TILT_H