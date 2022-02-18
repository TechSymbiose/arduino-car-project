#include <SPI.h>
#include <RF24.h>
#include <nRF24L01.h>
#include "Wire.h" // This library allows you to communicate with I2C devices.

#define pinCE 8 // RF module pins
#define pinCSN 9

#define pinPotX A0
#define pinPotY A1

RF24 radio(pinCE, pinCSN);

const uint64_t pipe = 0xE8E8F0F0E1LL;
const int MPU_ADDR = 0x68; // I2C address of the MPU-6050. If AD0 pin is set to HIGH, the I2C address will be 0x69.

struct Values {
  int pot_x;
  int pot_y;

  int16_t gyro_x;
  int16_t gyro_y;
  int16_t gyro_z;
};

struct Values values = {506, 506, -3000, 0};

void setup() {
  pinMode(pinPotX, INPUT);
  pinMode(pinPotY, INPUT);
  
  radio.begin();
  radio.openWritingPipe(pipe);
  radio.setChannel(100);

  Wire.begin();
  Wire.beginTransmission(MPU_ADDR); // Begins a transmission to the I2C slave (GY-521 board)
  Wire.write(0x6B); // PWR_MGMT_1 register
  Wire.write(0); // set to zero (wakes up the MPU-6050)
  Wire.endTransmission(true);

  delay(50);
}

void loop() {
  values.pot_x = analogRead(pinPotX);
  values.pot_y = analogRead(pinPotY);

  // This part of the code wasn't written by myself
  Wire.beginTransmission(MPU_ADDR);
  Wire.write(0x3B); // starting with register 0x3B (ACCEL_XOUT_H) [MPU-6000 and MPU-6050 Register Map and Descriptions Revision 4.2, p.40]
  Wire.endTransmission(false); // the parameter indicates that the Arduino will send a restart. As a result, the connection is kept active.
  Wire.requestFrom(MPU_ADDR, 7*2, true); // request a total of 7*2=14 registers
  
  // "Wire.read()<<8 | Wire.read();" means two registers are read and stored in the same variable
  values.gyro_x = Wire.read()<<8 | Wire.read(); // reading registers: 0x43 (GYRO_XOUT_H) and 0x44 (GYRO_XOUT_L)
  values.gyro_y = Wire.read()<<8 | Wire.read(); // reading registers: 0x45 (GYRO_YOUT_H) and 0x46 (GYRO_YOUT_L)
  values.gyro_z = Wire.read()<<8 | Wire.read(); // reading registers: 0x47 (GYRO_ZOUT_H) and 0x48 (GYRO_ZOUT_L)
  
  // ************ Sending ************
  radio.write(&values, sizeof(values));

  delay(5);
}
