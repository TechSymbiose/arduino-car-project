# **Mecanum Wheels Arduino Robot Car**

Code written by Adrien Louvrier, 14/07/2021

*Objective* : receive the transmitted data and move the arduino robot car according to these data.

card used : Arduino Mega 2560

# **Electronic circuit**

## **List of the components**

# Arduino robot car components

Component | Quantity 
----------|----------
Arduino Mega 2560 | 1
L298N module | 2
DC Motor | 4
(basic or mecanum) wheels | 4
switch | 1
RF module | 1
3 cells battery holder | 1
18650 cells | 3

*input* : 
- GY-521 gyroscope module connected to
- potentiometer (X axis pin -> pin A0, Y axis pin -> pin A1, Vcc pin -> 5V pin, grd pin -> grd pin)
- RF module (CE pin -> pin 8, CSN pin -> pin 9, SCK pin -> 52, M0 pin -> pin 51, M1 pin -> pin 50, Vcc pin -> +5V pin, grd pin -> grd pin)

*output* : 
- L298N module 1 (IN1 pin -> pin 6, IN2 pin -> pin 9, IN3 pin -> pin 10, IN4 pin -> pin 11)
- L298N module 2 (IN1 pin -> pin 5, IN2 pin -> pin 4, IN3 pin -> pin 3, IN4 pin -> pin 2)
)

*motor connections* : 
- motor 1 positive pin -> driver 2 positive output 1
- motor 1 negative pin -> driver 2 negative output 1
- motor 2 positive pin -> driver 2 positive output 2
- motor 2 negative pin -> driver 2 negative output 2
- motor 3 positive pin -> driver 1 positive output 1
- motor 3 negative pin -> driver 1 negative output 1
- motor 4 positive pin -> driver 1 positive output 2
- motor 4 negative pin -> driver 1 negative output 2

*battery connections* :
- positive battery terminal -> central terminal of the switch
- negative battery terminal -> both negative motor driver terminal
- open terminal of the switch -> both 12V motor driver terminal
- 5V pin of the Arduino card -> 5V motor driver 2 terminal