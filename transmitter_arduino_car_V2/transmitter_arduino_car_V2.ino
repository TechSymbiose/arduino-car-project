#include <SPI.h>
#include <RF24.h>
#include <nRF24L01.h>

#define pinCE 8
#define pinCSN 9

#define potValueX A0
#define potValueY A1
#define pinButton1 2
#define pinButton2 4

RF24 radio(pinCE, pinCSN);

const uint64_t pipe = 0xE8E8F0F0E1LL;

struct Values {
  int value_x;
  int value_y;
  bool button1;
  bool button2;
};

struct Values values = {506, 506, false, false};

void setup() {
  //Serial.begin(9600);
  pinMode(potValueX, INPUT);
  pinMode(potValueY, INPUT);
  pinMode(pinButton1, INPUT);
  pinMode(pinButton2, INPUT);
  
  radio.begin();
  radio.openWritingPipe(pipe);
  radio.setChannel(100);
}

void loop() {
  values.value_x = analogRead(potValueX);
  values.value_y = analogRead(potValueY);
  values.button1 = digitalRead(pinButton1);
  values.button2 = digitalRead(pinButton2);

  //Serial.println(values.button2);

  //Serial.println(values.value_x);
  //Serial.println(values.value_y);
  
  // ************ Sending ************
  radio.write(&values, sizeof(values));

  delay(5);
}
