#include <SPI.h>
#include <RF24.h>
#include <nRF24L01.h>

#define pinCE 7 // RF communication pins
#define pinCSN 8

#define MOTOR1_pos 9 // motor driver pins
#define MOTOR1_neg 6

#define MOTOR2_pos 11
#define MOTOR2_neg 10

#define MOTOR3_pos 3
#define MOTOR3_neg 2

#define MOTOR4_pos 5
#define MOTOR4_neg 4

#define PotRight 600 // border values of the potentiometer
#define PotLeft 400
#define PotDown 400
#define PotUp 600

#define GyroUp 6000 // border value of the gyroscope
#define GyroDown -8000
#define GyroRight 8000
#define GyroLeft -8000
#define GyroMin -12000
#define GyroMax 12000

#define SPEED 170 // Speed of the motors

const uint64_t pipe = 0xE8E8F0F0E1LL; // pipe for RF communications

RF24 radio(pinCE, pinCSN); // Initialisation of the RF communication

// structure for RF communication data
struct Data {
  int pot_x;
  int pot_y;

  int16_t gyro_x;
  int16_t gyro_y;
  int16_t gyro_z;
};

struct Data dataValues = {506, 506, 0, 0, 0}; // Initialization of the data

// direction possibilities
enum directions{
  front, back, left, right,
  frontLeft, frontRight, backLeft, backRight,
  turnLeft, turnRight, stationary
} ;

enum directions movement;

// Prototype of the functions

/**
 * @brief : Receive the data and store it in the data structure
 */
void receiveData();

/**
 * @brief : get the movement the Arduino Car must do according to the data received
 */
void getMovement();

/**
 * @brief : move the car according to the movement the car must do
 */
void moveCar();

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

  // Start the RF communication in receiving mode
  radio.begin();
  radio.openReadingPipe(1, pipe);
  radio.setChannel(100);
  radio.startListening();

  delay(1000);
}

void loop() {
  // Get the data
  receiveData();

  // Crop the x and y axis values of the gyroscope
  if (dataValues.gyro_x > GyroMax) {
    dataValues.gyro_x = 12000;
  } else if (dataValues.gyro_x < GyroMin) {
    dataValues.gyro_x = -12000;
  }
  
  if (dataValues.gyro_y > GyroMax) {
    dataValues.gyro_y = 12000;   
  } else if (dataValues.gyro_y < GyroMin) {
    dataValues.gyro_y = -12000;
  }

  // Get the movement according to the values received
  getMovement();

  // Move the car with the right movement
  moveCar();
}

void receiveData() {
  // *********** Receiveing ************
  if (radio.available()) {
    while (radio.available()) {
      radio.read(&dataValues, sizeof(dataValues));
    }
    delay(20);
  }
  delay(5);
}

void getMovement() {
  if (dataValues.pot_x <= PotDown && dataValues.gyro_x >= GyroUp && dataValues.gyro_y > GyroLeft && dataValues.gyro_y < GyroRight) {
    movement = front;
    
  } else if (dataValues.pot_x <= PotDown && dataValues.gyro_x <= GyroDown && dataValues.gyro_y > GyroLeft && dataValues.gyro_y < GyroRight) {
    movement = back;
    
  } else if (dataValues.pot_x <= PotDown && dataValues.gyro_x > GyroDown && dataValues.gyro_x < GyroUp && dataValues.gyro_y <= GyroLeft) {
    movement = left;
    
  } else if (dataValues.pot_x <= PotDown && dataValues.gyro_x > GyroDown && dataValues.gyro_x < GyroUp && dataValues.gyro_y >= GyroRight) {
    movement = right;
    
  } else if (dataValues.pot_x <= PotDown && dataValues.gyro_x > GyroUp && dataValues.gyro_y < GyroLeft) {
    movement = frontLeft;
    
  } else if (dataValues.pot_x <= PotDown && dataValues.gyro_x > GyroUp && dataValues.gyro_y > GyroRight) {
    movement = frontRight;
    
  } else if (dataValues.pot_x <= PotDown && dataValues.gyro_x < GyroDown && dataValues.gyro_y < GyroLeft) {
    movement = backLeft;
    
  } else if (dataValues.pot_x <= PotDown && dataValues.gyro_x < GyroDown && dataValues.gyro_y > GyroRight) {
    movement = backRight;
    
  } else if (dataValues.pot_x <= PotDown && dataValues.pot_y <= PotLeft && dataValues.gyro_x > GyroDown && dataValues.gyro_x < GyroUp && dataValues.gyro_y > GyroLeft && dataValues.gyro_y < GyroRight) {
    movement = turnLeft;
    
  } else if (dataValues.pot_x <= PotDown && dataValues.pot_y >= PotRight && dataValues.gyro_x > GyroDown && dataValues.gyro_x < GyroUp && dataValues.gyro_y > GyroLeft && dataValues.gyro_y < GyroRight) {
    movement = turnRight;
    
  } else {
    movement = stationary;
  }
}

void moveCar() {
  switch (movement) {
    case (front):
      analogWrite(MOTOR1_neg, SPEED);
      analogWrite(MOTOR2_neg, SPEED);
      analogWrite(MOTOR3_neg, SPEED);
      analogWrite(MOTOR4_neg, SPEED);

      digitalWrite(MOTOR1_pos, LOW);
      digitalWrite(MOTOR2_pos, LOW);
      digitalWrite(MOTOR3_pos, LOW);
      digitalWrite(MOTOR4_pos, LOW);
      break;
      
    case (back):
      digitalWrite(MOTOR1_neg, LOW);
      digitalWrite(MOTOR2_neg, LOW);
      digitalWrite(MOTOR3_neg, LOW);
      digitalWrite(MOTOR4_neg, LOW);

      analogWrite(MOTOR1_pos, SPEED);
      analogWrite(MOTOR2_pos, SPEED);
      analogWrite(MOTOR3_pos, SPEED);
      analogWrite(MOTOR4_pos, SPEED);
      break;

     case (right):
      digitalWrite(MOTOR1_neg, LOW);
      analogWrite(MOTOR2_neg, SPEED);
      analogWrite(MOTOR3_neg, SPEED);
      digitalWrite(MOTOR4_neg, LOW);

      analogWrite(MOTOR1_pos, SPEED);
      digitalWrite(MOTOR2_pos, LOW);
      digitalWrite(MOTOR3_pos, LOW);
      analogWrite(MOTOR4_pos, SPEED);
      break;

    case (left):
      analogWrite(MOTOR1_neg, SPEED);
      digitalWrite(MOTOR2_neg, LOW);
      digitalWrite(MOTOR3_neg, LOW);
      analogWrite(MOTOR4_neg, SPEED);

      digitalWrite(MOTOR1_pos, LOW);
      analogWrite(MOTOR2_pos, SPEED);
      analogWrite(MOTOR3_pos, SPEED);
      digitalWrite(MOTOR4_pos, LOW);
      break;

    case (frontRight):
      digitalWrite(MOTOR1_pos, LOW);
      digitalWrite(MOTOR2_pos, LOW);
      digitalWrite(MOTOR3_pos, LOW);
      digitalWrite(MOTOR4_pos, LOW);

      digitalWrite(MOTOR1_neg, LOW);
      analogWrite(MOTOR2_neg, SPEED);
      analogWrite(MOTOR3_neg, SPEED);
      digitalWrite(MOTOR4_neg, LOW);
      break;

    case (frontLeft):
      digitalWrite(MOTOR1_pos, LOW);
      digitalWrite(MOTOR2_pos, LOW);
      digitalWrite(MOTOR3_pos, LOW);
      digitalWrite(MOTOR4_pos, LOW);

      analogWrite(MOTOR1_neg, SPEED);
      digitalWrite(MOTOR2_neg, LOW);
      digitalWrite(MOTOR3_neg, LOW);
      analogWrite(MOTOR4_neg, SPEED);
      break;
      
    case (backRight):
      digitalWrite(MOTOR1_neg, LOW);
      digitalWrite(MOTOR2_neg, LOW);
      digitalWrite(MOTOR3_neg, LOW);
      digitalWrite(MOTOR4_neg, LOW);

      analogWrite(MOTOR1_pos, SPEED);
      digitalWrite(MOTOR2_pos, LOW);
      digitalWrite(MOTOR3_pos, LOW);
      analogWrite(MOTOR4_pos, SPEED);
      break;
      
    case (backLeft):
      digitalWrite(MOTOR1_neg, LOW);
      digitalWrite(MOTOR2_neg, LOW);
      digitalWrite(MOTOR3_neg, LOW);
      digitalWrite(MOTOR4_neg, LOW);

      digitalWrite(MOTOR1_pos, LOW);
      analogWrite(MOTOR2_pos, SPEED);
      analogWrite(MOTOR3_pos, SPEED);
      digitalWrite(MOTOR4_pos, LOW);
      break;

    case (turnLeft):
      digitalWrite(MOTOR1_pos, LOW);
      analogWrite(MOTOR1_neg, SPEED);
      analogWrite(MOTOR2_pos, SPEED);
      digitalWrite(MOTOR2_neg, LOW);
      digitalWrite(MOTOR3_pos, LOW);
      analogWrite(MOTOR3_neg, SPEED);
      analogWrite(MOTOR4_pos, SPEED);
      digitalWrite(MOTOR4_neg, LOW);
      break;

    case (turnRight):
      analogWrite(MOTOR1_pos, SPEED);
      digitalWrite(MOTOR1_neg, LOW);
      digitalWrite(MOTOR2_pos, LOW);
      analogWrite(MOTOR2_neg, SPEED);
      analogWrite(MOTOR3_pos, SPEED);
      digitalWrite(MOTOR3_neg, LOW);
      digitalWrite(MOTOR4_pos, LOW); 
      analogWrite(MOTOR4_neg, SPEED);
      break;

    case (stationary):
      digitalWrite(MOTOR1_pos, LOW);
      digitalWrite(MOTOR1_neg, LOW);
      digitalWrite(MOTOR2_pos, LOW);
      digitalWrite(MOTOR2_neg, LOW);
      digitalWrite(MOTOR3_pos, LOW);
      digitalWrite(MOTOR3_neg, LOW);
      digitalWrite(MOTOR4_pos, LOW);
      digitalWrite(MOTOR4_neg, LOW);
      break;
  }
}
