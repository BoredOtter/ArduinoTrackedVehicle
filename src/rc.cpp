#include <Arduino.h>
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include "arc.h"


RF24 radio(9, 10); // CE, CSN
const byte address[6] = "00001";

//DCmotor control pins etc
MotorControlData motorControlData;

DCMotor motor1; //left
DCMotor motor2; //right

unsigned long radioWaitCycle=0;
const unsigned long radioTimeOut=30;

void setup() 
{
  //DC motor setup using structs
  // motor1.controlPin=7;
  // motor1.pwmPin    =6;

  // motor2.controlPin=4;
  // motor2.pwmPin    =5;

  motor1.controlPin=4;
  motor1.pwmPin    =5;

  motor2.controlPin=7;
  motor2.pwmPin    =6;

  pinMode(motor1.controlPin,OUTPUT);
  pinMode(motor2.controlPin,OUTPUT);

  pinMode(motor1.pwmPin,OUTPUT);
  pinMode(motor2.pwmPin,OUTPUT);
  //

  Serial.begin(9600);
  radio.begin();
  radio.openReadingPipe(0, address);   //Setting the address at which we will receive the data
  radio.setPALevel(RF24_PA_MAX);       //You can set this as minimum or maximum depending on the distance between the transmitter and receiver.
  radio.startListening();              //This sets the module as receiver


}


void loop()
{

  if (radio.available())              //Looking for the data.
  {

    radio.read(&motorControlData, sizeof(motorControlData));    //Reading the data


    //serial debug info
    // Serial.print("RC: ");
    // Serial.print(motorControlData.motor1speed);
    // Serial.print("  ");
    // Serial.print(motorControlData.motor2speed);
    // Serial.println();
    //

    //motorHandling
    motorHandling(motorControlData,motor1,motor2);


    radioWaitCycle=0;
  }
  else
  {
    while (!radio.available())
    {
      delay(1);
      radioWaitCycle++;

      if (radioWaitCycle>radioTimeOut)
      {
        // Serial.print("        Radio non availale: ");
        // Serial.print(radioWaitCycle);
        // Serial.println();

        motorControlData.motor1speed=0;
        motorControlData.motor2speed=0;
        motorHandling(motorControlData,motor1,motor2);
      }
      
    }
    
  }

}