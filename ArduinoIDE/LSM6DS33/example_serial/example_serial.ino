#include <Wire.h>
// #include "LSM6DS33.h"
// #include "LIS3MDL.h"

#include "PololuIMUv9.h"
#include "tilt.h"


PololuIMUv9 imu;

float gyro_roll, gyro_pitch, gyro_yaw;
float gyro_roll_prev, gyro_pitch_prev, gyro_yaw_prev;

float roll, pitch;

unsigned long time_present, time_prev;
float dt;

TiltData tilt_data;

void setup() {
  Serial.begin(9600);
  Wire.begin();
  imu.init();

  delay(1000);

  imu.read();
  roll = tilt_from_accelerometer(imu.a.x, imu.a.y, imu.a.z).roll;
  pitch = tilt_from_accelerometer(imu.a.x, imu.a.y, imu.a.z).pitch;
  delay(100);

  time_prev = micros();
  gyro_roll_prev = roll;
  gyro_pitch_prev = pitch;
}

int value, temp;
void loop() {
  imu.read();

  delay(100);

  float roll = tilt_from_accelerometer(imu.a.x, imu.a.y, imu.a.z).roll;
  float pitch = tilt_from_accelerometer(imu.a.x, imu.a.y, imu.a.z).pitch;

  time_present = micros();
  dt = (time_present - time_prev) / (float) 1e6;  // [s]

  gyro_roll = gyro_roll_prev + imu.g.x * dt;
  gyro_pitch = gyro_pitch_prev + imu.g.y * dt;

  Serial.print(roll);
  Serial.print(",");
  Serial.print(pitch);
  Serial.print(",");

  Serial.print(gyro_roll);
  Serial.print(",");
  Serial.println(gyro_pitch);

  // update variables
  gyro_roll_prev = gyro_roll;
  gyro_pitch_prev = gyro_pitch;

  time_prev = time_present;
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

  delay(100);
}
