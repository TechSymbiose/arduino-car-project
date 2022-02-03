/*
 *  ************ Arduino Car Project ************

    Auteur : Adrien Louvrier
    Date de création : 17/07/2021 - 19h43
    Carte utilisée : Arduino Mega
    Circuit :
    -
    -
    -
    -
    -
    -
*/

// Pins correspondants aux moteurs sur les motor drivers

#include <SPI.h>
#include <RF24.h>
#include <nRF24L01.h>

#define pinCE 7
#define pinCSN 8

#define MOTOR1_pos 9
#define MOTOR1_neg 6

#define MOTOR2_pos 11
#define MOTOR2_neg 10

#define MOTOR3_pos 3
#define MOTOR3_neg 2

#define MOTOR4_pos 5
#define MOTOR4_neg 4

#define PotUp 600
#define PotDown 400
#define PotRight 600
#define PotLeft 400

#define GyroUp 5000
#define GyroDown -5000
#define GyroRight 5000
#define GyroLeft -5000
#define GyroMin -12000
#define GyroMax 12000

const uint64_t pipe = 0xE8E8F0F0E1LL;

RF24 radio(pinCE, pinCSN);

struct Values {
  int pot_x;
  int pot_y;

  int16_t gyro_x;
  int16_t gyro_y;
  int16_t gyro_z;
};

struct Values values = {506, 506, 0, 0, 0};

bool sens = true; // front = true, back = false
int directionValue = 0; // aucune direction = 0, left = 1, right = 2
int speedValue = 0;

void setup() {
  pinMode(MOTOR1_pos, OUTPUT);
  pinMode(MOTOR1_neg, OUTPUT);
  pinMode(MOTOR2_pos, OUTPUT);
  pinMode(MOTOR2_neg, OUTPUT);
  pinMode(MOTOR3_pos, OUTPUT);
  pinMode(MOTOR3_neg, OUTPUT);
  pinMode(MOTOR4_pos, OUTPUT);
  pinMode(MOTOR4_neg, OUTPUT);

  digitalWrite(MOTOR1_pos, LOW);
  digitalWrite(MOTOR1_neg, LOW);
  digitalWrite(MOTOR2_pos, LOW);
  digitalWrite(MOTOR2_neg, LOW);
  digitalWrite(MOTOR3_pos, LOW);
  digitalWrite(MOTOR3_neg, LOW);
  digitalWrite(MOTOR4_pos, LOW);
  digitalWrite(MOTOR4_neg, LOW);

  Serial.begin(9600);
  radio.begin();
  radio.openReadingPipe(1, pipe);
  radio.setChannel(100);
  radio.startListening();

  delay(3500);
}

void loop() {
  // *********** Receiveing ************
  if (radio.available()) {
    while (radio.available()) {
      radio.read(&values, sizeof(values));
    }
    delay(20);
  }
  delay(5);

  if (values.gyro_x > GyroMax) {
    values.gyro_x = 12000;
  }
  if (values.gyro_x < GyroMin) {
    values.gyro_x = -12000;
  }

  // Ne pas bouger
  if (values.gyro_x >= GyroDown && values.gyro_x <= GyroUp) {
    digitalWrite(MOTOR1_pos, LOW);
    digitalWrite(MOTOR1_neg, LOW);
    digitalWrite(MOTOR2_pos, LOW);
    digitalWrite(MOTOR2_neg, LOW);
    digitalWrite(MOTOR3_pos, LOW);
    digitalWrite(MOTOR3_neg, LOW);
    digitalWrite(MOTOR4_pos, LOW);
    digitalWrite(MOTOR4_neg, LOW);
  }

  // Avancer
  if (values.gyro_x < GyroDown) {
    if (!sens) {
      sens = true;
      digitalWrite(MOTOR1_neg, LOW);
      digitalWrite(MOTOR2_neg, LOW);
      digitalWrite(MOTOR3_neg, LOW);
      digitalWrite(MOTOR4_neg, LOW);
    }
    digitalWrite(MOTOR1_pos, HIGH);
    digitalWrite(MOTOR2_pos, HIGH);
    digitalWrite(MOTOR3_pos, HIGH);
    digitalWrite(MOTOR4_pos, HIGH);
  }

  // Reculer
  if (values.gyro_x > GyroUp) {
    if (sens) {
      sens = false;
      digitalWrite(MOTOR1_pos, LOW);
      digitalWrite(MOTOR2_pos, LOW);
      digitalWrite(MOTOR3_pos, LOW);
      digitalWrite(MOTOR4_pos, LOW);
    }
    
    digitalWrite(MOTOR1_neg, HIGH);
    digitalWrite(MOTOR2_neg, HIGH);
    digitalWrite(MOTOR3_neg, HIGH);
    digitalWrite(MOTOR4_neg, HIGH);
  }
}
