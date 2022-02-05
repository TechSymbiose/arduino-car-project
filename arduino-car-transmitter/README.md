# **Arduino car transmitter**

Code written by Adrien Louvrier, 14/07/2021

*Objective* : control the arduino robot car thanks a transmitter using RF communication, gyroscope and joystick control.

card used : Arduino Nano

# **Electronic circuit**

## **List of the components**

Component | Quantity 
----------|----------
Arduino Nano | 1
RF module | 1
potentiometer | 1
GY-521 gyroscope module | 1
USB battery | 1

*input* : 
- GY-521 gyroscope module
- potentiometer (X axis pin -> pin A0, Y axis pin -> pin A1, Vcc pin -> 5V pin, grd pin -> grd pin)

*output* :
- RF module (CE pin -> pin 8, CSN pin -> pin 9, SCK pin -> pin 10, M0 pin -> pin 12, M1 pin -> pin 13, Vcc pin -> +5V pin, grd pin -> grd pin)