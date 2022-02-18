# **Arduino Car Transmitter**

Code written by Adrien Louvrier, 14/07/2021

*Objective* : control the arduino robot car thanks a transmitter using RF communication, gyroscope and joystick control.

card used : Arduino Nano

---

## **List of the components**

Component | Quantity 
----------|----------
Arduino Nano | 1
RF module | 1
potentiometer | 1
GY-521 gyroscope module | 1
USB battery | 1

# **Electronic circuit**

## *input* : 

### GY-521 gyroscope module

GY-521 pins | Arduino Nano pins
--- | ---
SCL |A5
SDA | A4
Vcc | +5V
grd | grd

### Potentiometer

Potentiometer pins | Arduino Nano pins
--- | ---
X axis | A0
Y axis | A1
Vcc | +5V
grd | grd

## *output* :

RF module pins | Arduino Nano pins
--- | ---
CE | 8
CSN | 9
SCK | 10
M0 | 12
M1 | 13
Vcc | +5V
grd | grd