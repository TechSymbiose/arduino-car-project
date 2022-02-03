/*
 *  ************ Arduino Car Project ************

    Auteur : Adrien Louvrier
    Date de création : 14/07/2021 - 10h48
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

#define PotRight 600
#define PotLeft 400

#define GyroUp 6000
#define GyroDown -8000
#define GyroRight 8000
#define GyroLeft -8000
#define GyroMin -12000
#define GyroMax 12000

#define MaxSpeed 255

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
int directionValue = 0; // no direction = 0, left = 1, right = 2

enum directions{
  left, right, front, back,
  frontLeft, frontRight, backLeft, backRight,
  stationary
};

enum directions movement;

void setup() {
  // Setup the motor drivers pins in output mode
  pinMode(MOTOR1_pos, OUTPUT);
  pinMode(MOTOR1_neg, OUTPUT);
  pinMode(MOTOR2_pos, OUTPUT);
  pinMode(MOTOR2_neg, OUTPUT);
  pinMode(MOTOR3_pos, OUTPUT);
  pinMode(MOTOR3_neg, OUTPUT);
  pinMode(MOTOR4_pos, OUTPUT);
  pinMode(MOTOR4_neg, OUTPUT);

  // Turn off motors
  digitalWrite(MOTOR1_pos, LOW);
  digitalWrite(MOTOR1_neg, LOW);
  digitalWrite(MOTOR2_pos, LOW);
  digitalWrite(MOTOR2_neg, LOW);
  digitalWrite(MOTOR3_pos, LOW);
  digitalWrite(MOTOR3_neg, LOW);
  digitalWrite(MOTOR4_pos, LOW);
  digitalWrite(MOTOR4_neg, LOW);

  radio.begin();
  radio.openReadingPipe(1, pipe);
  radio.setChannel(100);
  radio.startListening();

  delay(1000);
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

  // Crop the x and y values of the gyroscope
  if (values.gyro_x > GyroMax) {
    values.gyro_x = 12000;
    
  } else if (values.gyro_x < GyroMin) {
    values.gyro_x = -12000;
    
  }
  
  if (values.gyro_y > GyroMax) {
    values.gyro_y = 12000;
    
  } else if (values.gyro_y < GyroMin) {
    values.gyro_y = -12000;
  }

  if (values.gyro_y < GyroLeft) { // Turn left
    directionValue = 1;
    
  } else if (values.gyro_y > GyroRight) { // Turn right
    directionValue = 2;
    
  } else if (values.gyro_y <= GyroRight && values.gyro_y >= GyroLeft) { // Do not turn
    directionValue = 0;
  }

  // Stay put
  if (values.gyro_x >= GyroDown && values.gyro_x <= GyroUp) {
    
    if (values.pot_y <= PotRight && values.pot_y >= PotLeft) { // Do not turn
      digitalWrite(MOTOR1_pos, LOW);
      digitalWrite(MOTOR1_neg, LOW);
      digitalWrite(MOTOR2_pos, LOW);
      digitalWrite(MOTOR2_neg, LOW);
      digitalWrite(MOTOR3_pos, LOW);
      digitalWrite(MOTOR3_neg, LOW);
      digitalWrite(MOTOR4_pos, LOW);
      digitalWrite(MOTOR4_neg, LOW);
      
    } else if (values.pot_y > PotRight) { // Turn left
      digitalWrite(MOTOR1_pos, HIGH);
      digitalWrite(MOTOR1_neg, LOW);
      digitalWrite(MOTOR2_pos, LOW);
      digitalWrite(MOTOR2_neg, HIGH);
      digitalWrite(MOTOR3_pos, HIGH);
      digitalWrite(MOTOR3_neg, LOW);
      digitalWrite(MOTOR4_pos, LOW); 
      digitalWrite(MOTOR4_neg, HIGH);
     
    } else if (values.pot_y < PotLeft) { // Turn right
      digitalWrite(MOTOR1_pos, LOW);
      digitalWrite(MOTOR1_neg, HIGH);
      digitalWrite(MOTOR2_pos, HIGH);
      digitalWrite(MOTOR2_neg, LOW);
      digitalWrite(MOTOR3_pos, LOW);
      digitalWrite(MOTOR3_neg, HIGH);
      digitalWrite(MOTOR4_pos, HIGH);
      digitalWrite(MOTOR4_neg, LOW); 
    }
    
  } else if (values.gyro_x < GyroDown) { // Go foreward
    
    if (!sens) {
      sens = true;
      digitalWrite(MOTOR1_neg, LOW);
      digitalWrite(MOTOR2_neg, LOW);
      digitalWrite(MOTOR3_neg, LOW);
      digitalWrite(MOTOR4_neg, LOW);
    }

    if (directionValue == 2) { // turn right
      digitalWrite(MOTOR1_pos, LOW);
      digitalWrite(MOTOR2_pos, HIGH);
      digitalWrite(MOTOR3_pos, LOW);
      digitalWrite(MOTOR4_pos, HIGH);
      
    } else if (directionValue == 1) { //turn left
      digitalWrite(MOTOR1_pos, HIGH);
      digitalWrite(MOTOR2_pos, LOW);
      digitalWrite(MOTOR3_pos, HIGH);
      digitalWrite(MOTOR4_pos, LOW);
      
    } else if (directionValue == 0) { // go foreward without turning
      digitalWrite(MOTOR1_pos, HIGH);
      digitalWrite(MOTOR2_pos, HIGH);
      digitalWrite(MOTOR3_pos, HIGH);
      digitalWrite(MOTOR4_pos, HIGH);
    }
  } else if (values.gyro_x > GyroUp) { // Go backward
    
    if (sens) {
      sens = false;
      digitalWrite(MOTOR1_pos, LOW);
      digitalWrite(MOTOR2_pos, LOW);
      digitalWrite(MOTOR3_pos, LOW);
      digitalWrite(MOTOR4_pos, LOW);
    }

    if (directionValue == 2) { // turn right
      digitalWrite(MOTOR1_neg, LOW);
      digitalWrite(MOTOR2_neg, HIGH);
      digitalWrite(MOTOR3_neg, LOW);
      digitalWrite(MOTOR4_neg, HIGH);
      
    } else if (directionValue == 1) { // turn left
      digitalWrite(MOTOR1_neg, HIGH);
      digitalWrite(MOTOR2_neg, LOW);
      digitalWrite(MOTOR3_neg, HIGH);
      digitalWrite(MOTOR4_neg, LOW);
      
    } else if (directionValue == 0) { // go backward without turning
      digitalWrite(MOTOR1_neg, HIGH);
      digitalWrite(MOTOR2_neg, HIGH);
      digitalWrite(MOTOR3_neg, HIGH);
      digitalWrite(MOTOR4_neg, HIGH);
    }
  }
}
