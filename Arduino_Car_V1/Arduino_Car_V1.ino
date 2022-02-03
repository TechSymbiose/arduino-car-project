/* 
 *  ************ Arduino Car Project ************
 *  
 *  Auteur : Adrien Louvrier
 *  Date de création : 11/07/2021 - 14h59
 *  Carte utilisée : Arduino Uno
 *  Circuit :
 *  -
 *  -
 *  -
 *  -
 *  -
 *  -
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

#define upValue 600
#define downValue 400
#define rightValue 1000
#define leftValue 100

const uint64_t pipe = 0xE8E8F0F0E1LL;

RF24 radio(pinCE, pinCSN);

struct Values {
  int value_x;
  int value_y;
};

struct Values values = {506, 506};

float coeff = 1.0;
int sens = 1;
int directionValue = 0;
const int speedValue = 255;
float turnValue = 0;

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

  // ************ Action ************
  if (values.value_y >= rightValue) {
    directionValue = 1;
    turnValue = 0;
    //turnValue = map (values.value_y, 512, 1023, 0, 1000) / 1000;
  }
  if (values.value_y <= leftValue) {
    directionValue = 0;
    turnValue = 0;
    //turnValue = map(values.value_y, 0, 500, 1000, 0) / 1000;
  }
  
  if (values.value_y > 500 && values.value_y < 512) {
    turnValue = 1;
  }
  
  if (values.value_x >= upValue) {
    //speedValue = map(values.value_x, 512, 1023, 50, 255);
    if (!sens) {
      sens = 1;
      digitalWrite(MOTOR1_neg, LOW);
      digitalWrite(MOTOR2_neg, LOW);
      digitalWrite(MOTOR3_neg, LOW);
      digitalWrite(MOTOR4_neg, LOW);
    }
    if (directionValue) {
      analogWrite(MOTOR1_pos, turnValue*speedValue);
      analogWrite(MOTOR2_pos, speedValue);
      analogWrite(MOTOR3_pos, turnValue*speedValue);
      analogWrite(MOTOR4_pos, speedValue);
    }
    else {
      analogWrite(MOTOR1_pos, speedValue);
      analogWrite(MOTOR2_pos, turnValue*speedValue);
      analogWrite(MOTOR3_pos, speedValue);
      analogWrite(MOTOR4_pos, turnValue*speedValue);
    }
    
  }
 if (values.value_x <= downValue) {
  //speedValue = map (values.value_x, 0, 500, 255, 50);
  if (sens) {
    sens = 0;
    digitalWrite(MOTOR1_pos, LOW);
    digitalWrite(MOTOR2_pos, LOW);
    digitalWrite(MOTOR3_pos, LOW);
    digitalWrite(MOTOR4_pos, LOW);
  }
  if (directionValue) {
    analogWrite(MOTOR1_neg, turnValue*speedValue);
    analogWrite(MOTOR2_neg, speedValue);
    analogWrite(MOTOR3_neg, turnValue*speedValue);
    analogWrite(MOTOR4_neg, speedValue);
  }
  else {
    analogWrite(MOTOR1_neg, speedValue);
    analogWrite(MOTOR2_neg, turnValue*speedValue);
    analogWrite(MOTOR3_neg, speedValue);
    analogWrite(MOTOR4_neg, turnValue*speedValue);
  }
  
 }
 if (values.value_x > downValue && values.value_x < upValue) {
    //speedValue = 0;
    digitalWrite(MOTOR1_pos, LOW);
    digitalWrite(MOTOR2_pos, LOW);
    digitalWrite(MOTOR3_pos, LOW);
    digitalWrite(MOTOR4_pos, LOW);
    digitalWrite(MOTOR1_neg, LOW);
    digitalWrite(MOTOR2_neg, LOW);
    digitalWrite(MOTOR3_neg, LOW);
    digitalWrite(MOTOR4_neg, LOW);
  }
}
