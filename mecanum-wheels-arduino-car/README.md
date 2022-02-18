# **Mecanum Wheels Arduino Robot Car**

Code written by Adrien Louvrier, 14/07/2021

*Objective* : receive the transmitted data and move the arduino robot car according to these data.

card used : Arduino Mega 2560

# **List of the components**

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

# **Electronic circuit**

## *input* : 

### RF module

 RF module pins| Arduino Mega 2560 pins
 --- | ---
 CE | 8
 CSN | 9
 SCK | 52
 M0 | 51
 M1 | 50
 Vcc | +5V
 grd | grd

## *output* : 

### L298N motor driver (x2)

L298N(1) pins | Arduino Mega 2560 pins
 --- | ---
 IN1 | 6
 IN2 | 9
 IN3 | 10
 IN4 | 11

 L298N(2) pins | Arduino Mega 2560 pins
 --- | ---
 IN1 | 5
 IN2 | 4
 IN3 | 3
 IN4 | 2

---

### Motors connections

Motor 1 pins | Motor driver 2 pins
--- | ---
positive pin | positive output 1
negative pin | negative output 2

Motor 2 pins | Motor driver 2 pins
--- | ---
positive pin | positive output 2
negative pin | negative output 2

Motor 3 pins | Motor driver 1 pins
--- | ---
positive pin | positive output 1
negative pin | negative output 1

Motor 4 pins | Motor driver 1 pins
--- | ---
positive pin | positive output 2
negative pin | negative output 2

### Batterie connection

Component 1 | Component 2
--- | ---
positive battery terminal | central terminal of the switch
negative battery terminal | both negative motor driver terminal
open terminal of the switch | both 12V motor driver terminal
5V pin of the Arduino card | 5V motor driver 2 terminal