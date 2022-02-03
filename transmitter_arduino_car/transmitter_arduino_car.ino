#include <SPI.h>
#include <RF24.h>
#include <nRF24L01.h>

#define pinCE 8
#define pinCSN 9

#define potValueX A0
#define potValueY A1

RF24 radio(pinCE, pinCSN);

const uint64_t pipe = 0xE8E8F0F0E1LL;

struct Values {
  int value_x;
  int value_y;
};

struct Values values = {506, 506};

void setup() {
  //Serial.begin(9600);
  pinMode(potValueX, INPUT);
  pinMode(potValueY, INPUT);
  
  radio.begin();
  radio.openWritingPipe(pipe);
  radio.setChannel(100);
}

void loop() {
  values.value_x = analogRead(potValueX);
  values.value_y = analogRead(potValueY);

  //Serial.println(values.value_x);
  //Serial.println(values.value_y);
  
  // ************ Sending ************
  radio.write(&values, sizeof(values));

  delay(5);
}
