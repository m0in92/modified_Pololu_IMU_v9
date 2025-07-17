#include <Wire.h>
#include "LSM6DS33.h"
#include "LIS3MDL.h"
// #include <LIS3MDL.h>

LSM6DS33 imu;
LIS3MDL mag;

// device specifications
const int MAG_SENSITIVITY_4GUASS = 6842;

void setup() {
  Serial.begin(9600);
  Wire.begin();
  imu.begin();
  mag.init();

  int imu_device_addr = imu.read_register(LSM6DS33::RegAddr::WHO_AM_I);
  Serial.println(imu_device_addr);

  // mag();
  int mag_device_addr = mag.read_reg(LIS3MDL::RegAddr::WHO_AM_I);
  Serial.println(mag_device_addr);

  delay(1000);
}

int value, temp;
void loop() {
  imu.read_accel();
  imu.read_gyro();
  mag.read_mag();

  delay(100);

  Serial.print(imu.a.x);
  Serial.print(",");
  Serial.print(imu.a.y);
  Serial.print(",");
  Serial.print(imu.a.z);
  Serial.print(",");
  Serial.print(imu.omega.x);
  Serial.print(",");
  Serial.print(imu.omega.y);
  Serial.print(",");
  Serial.print(imu.omega.z);
  Serial.print(",");
  Serial.print(mag.m.x);
  Serial.print(",");
  Serial.print(mag.m.y);
  Serial.print(",");
  Serial.print(mag.m.z);
  Serial.println(",");

  delay(100);
}
