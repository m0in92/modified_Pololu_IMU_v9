#include <Wire.h>
#include "LSM6DS33.h"

LSM6DS33 imu;

void setup() {
  Serial.begin(9600);
  Wire.begin();
  imu.begin();

  int value = imu.read_register(WHO_AM_I);
  Serial.println(value);
  delay(1000);
}

int value, temp;
void loop() {
  imu.read_accel();
  imu.read_temp();

  Serial.print(imu.a.x);
  Serial.print(",");
  Serial.print(imu.a.y);
  Serial.print(",");
  Serial.print(imu.a.z);
  Serial.print(",");
  Serial.println(imu.temp);
  delay(100);
}
