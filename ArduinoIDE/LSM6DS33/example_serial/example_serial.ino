#include <Wire.h>
// #include "LSM6DS33.h"
// #include "LIS3MDL.h"

#include "PololuIMUv9.h"
#include "tilt.h"


PololuIMUv9 imu;

float angle_x; angle_y; angle_z;
unsigned long time_present, time_prev;
float dt;

void setup() {
  Serial.begin(9600);
  Wire.begin();
  imu.init();

  delay(1000);

  time_prev = micros();
}

int value, temp;
void loop() {
  imu.read();

  delay(100);

  float roll = tilt_from_accelerometer(imu.a.x, imu.a.y, imu.a.z).roll;
  float pitch = tilt_from_accelerometer(imu.a.x, imu.a.y, imu.a.z).pitch;

  time_present = micros();
  dt = (time_present - time_prev) / (float) 1e6;  // [s]

  Serial.print(roll);
  Serial.print(",");
  Serial.print(pitch);
  Serial.print(",");

  Serial.print(imu.g.x);
  Serial.print(",");
  Serial.print(imu.g.y);
  Serial.print(",");
  Serial.print(imu.g.z);
  Serial.print(",");

  Serial.print(imu.m.x);
  Serial.print(",");
  Serial.print(imu.m.y);
  Serial.print(",");
  Serial.println(imu.m.z);

  delay(100);
}
