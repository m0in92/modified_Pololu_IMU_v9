#include <Wire.h>
// #include "LSM6DS33.h"
// #include "LIS3MDL.h"

#include "PololuIMUv9.h"
#include "tilt.h"


PololuIMUv9 imu;

float gyro_roll, gyro_pitch, gyro_yaw;
float accel_roll, accel_pitch;
float comp_roll, comp_pitch;

// moving average variables
const int SIZE = 10;
float array_accel_roll[SIZE];
float array_accel_pitch[SIZE];

// complimentary filter parameters
const float ALPHA = 0.9;

unsigned long time_present, time_prev;
float dt;

TiltData accel_tilt, gyro_tilt;

void setup() {
  Serial.begin(9600);
  Wire.begin();
  imu.init();

  delay(1000);

  accel_tilt = filter::filtered_tilt_accel(&imu, 10);
  Serial.print(accel_tilt.roll);
  Serial.print(",");
  Serial.println(accel_tilt.pitch);

  time_prev = micros();

  gyro_roll = accel_roll;
  gyro_pitch = accel_pitch;
}

int value, temp;
void loop() {
  accel_tilt = filter::filtered_tilt_accel(&imu, 1);
  Serial.print(accel_tilt.roll);
  Serial.print(",");
  Serial.println(accel_tilt.pitch);
  // imu.read();
  // delay(100);

  // accel_roll = tilt_from_accelerometer(&imu).roll;
  // accel_pitch = tilt_from_accelerometer(&imu).pitch;

  // time_present = micros();
  // dt = (time_present - time_prev) / (float) 1e6;  // [s]

  // gyro_roll = gyro_roll + (-imu.g.y) * dt;
  // gyro_pitch = gyro_pitch + imu.g.x * dt;

  // comp_roll = 0.5 * accel_roll + 0.5 * gyro_roll;
  // comp_pitch = 0.5 * accel_pitch + 0.5 * gyro_pitch;

  // Serial.print(accel_roll);
  // Serial.print(",");
  // Serial.println(accel_pitch);
  // Serial.print(",");

  // Serial.print(gyro_roll);
  // Serial.print(",");
  // Serial.print(gyro_pitch);
  // Serial.print(",");

  // Serial.print(comp_roll);
  // Serial.print(",");
  // Serial.println(comp_pitch);

  // update variable
  // time_prev = time_present;

  // Serial.print(imu.g.x);
  // Serial.print(",");
  // Serial.print(imu.g.y);
  // Serial.print(",");
  // Serial.print(imu.g.z);
  // Serial.print(",");

  // Serial.print(imu.m.x);
  // Serial.print(",");
  // Serial.print(imu.m.y);
  // Serial.print(",");
  // Serial.println(imu.m.z);

  // delay(100);
}
