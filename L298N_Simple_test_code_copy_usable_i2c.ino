// Include the library
#include <L298N.h>
#include <Wire.h>
#include "i2c_proto.h"


// Pin definition
const unsigned int IN1 = 8;
const unsigned int IN2 = 7;
const unsigned int EN1 = 3;
const unsigned int EN2 = 5;
const unsigned int IN3 = 4;
const unsigned int IN4 = 2;
const unsigned int EN3 = 6;
const unsigned int IN5 = 12;
const unsigned int IN6 = 11;
const unsigned int EN4 = 10;
const unsigned int IN7 = 9;
const unsigned int IN8 = 13;

L298N motor[] {{EN1, IN1, IN2},
                {EN2, IN3, IN4},
                {EN3, IN5, IN6},
                {EN4, IN7, IN8},
                };
// Create one motor instance
L298N motor0(EN1, IN1, IN2);
L298N motor1(EN2, IN3, IN4);
L298N motor2(EN3, IN5, IN6);
L298N motor3(EN4, IN7, IN8);

byte RxByte[3];
 
void I2C_RxHandler(int numBytes)
{
  int i=0;
  Serial.println("RxHandler");
  while(Wire.available()) {  // Read Any Received Data
    RxByte[i] = Wire.read();
    Serial.println(RxByte[i]+" ");
    i++;
  }
}

// Return the status on request... TODO
void I2C_RqHandler() {
  Wire.write(0x00); // idle
}

void setup()
{
  Serial.begin(9600);
  Wire.begin(0x11); // Initialize I2C (Slave Mode)
  Wire.onReceive(I2C_RxHandler);
  Wire.onRequest(I2C_RqHandler);
 
  // Set initial speed
  motor[0].setSpeed(255);
  motor[1].setSpeed(255);
  motor[2].setSpeed(255);
  motor[3].setSpeed(255);
  motor0.setSpeed(255);
  motor1.setSpeed(255);
  motor2.setSpeed(255);
  motor3.setSpeed(255);
  Serial.println("Initialization Complete!  Let's drink!");
}

void dispenseComplete() {
  Wire.write(
}

void loop()
{
  int pump = RxByte[1];
  int dispcount = (RxByte[2]*2300);
  int primetime = (20000);
  if(RxByte[0] == DISPENSE) {
    motor[pump].forward();
    RxByte[0] = 0,
    delay(dispcount),
    motor[pump].stop();
  }

  if(RxByte[0] == PRIME) {
    motor0.forward();
    motor1.forward();
    motor2.forward();
    motor3.forward();
    RxByte[0] = 0;
    delay(10000);
    motor0.stop();
    motor1.stop();
    motor2.stop();
    motor3.stop();
  }

  if(RxByte[0] == DRAIN) {
    motor0.backward();
    motor1.backward();
    motor2.backward();
    motor3.backward();
    delay(30000);
    RxByte[0] = 0;
    motor0.stop();
    motor1.stop();
    motor2.stop();
    motor3.stop();
  }
}

