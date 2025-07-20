#include <Wire.h>

#include "MinIMU9v5.h"
// #include "tilt.h"


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

// TiltData accel_tilt, gyro_tilt;

void setup() {
  Serial.begin(9600);
  Wire.begin();
  imu.init();

  delay(1000);
}

int value, temp;
void loop() {
  imu.read();

  Serial.print(imu.a.x);
  Serial.print(",");
  Serial.print(imu.a.y);
  Serial.print(",");
  Serial.print(imu.a.z);
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

