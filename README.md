Code that performs various range of motion (ROM) tests for the upper limb.

Description

The project uses an MPU6050 accelerometer and an Arduino Uno or Uno-compatible board. 
The sesnor is placed on a patients arm and they are instructed to perform different ROM tests.
A push button is used to allow users to have control of when to start and stop taking ROM measurements.
The raw acceleration data is obtained from the sesnor and converted into degrees through the use of trigonometric formula in the code.
The maximum and minimum angles reached during the movement are recorded and used to calculate the ROM.

Software

To run the code, Arduino IDE is required. The MPU6050 library is also needed. "Wire.h" library is also used to allow for I2C communication. 

References

This code was done by Ross Mitchell and Arya Naidoo

[1]jrowberg, “jrowberg/i2cdevlib,” GitHub, Aug. 29, 2019. https://github.com/jrowberg/i2cdevlib/tree/master/Arduino/MPU6050

[2]Arduino, “Wire,” Arduino.cc, 2024. https://docs.arduino.cc/language-reference/en/functions/communication/wire/
