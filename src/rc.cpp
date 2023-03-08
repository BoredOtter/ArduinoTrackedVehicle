#include <Arduino.h>
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <structs.h>


RF24 radio(9, 10); // CE, CSN
const byte address[6] = "00001";

//DCmotor control pins etc
MotorControlData motorControlData;

DCMotor motor1; //left
DCMotor motor2; //right

unsigned long radioWaitTime=0;
const unsigned long radioTimeOut=30;

void setup() 
{
  //DC motor setup using structs
  motor1.controlPin=7;
  motor1.pwmPin    =6;

  motor2.controlPin=4;
  motor2.pwmPin    =5;

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
    Serial.print("RC: ");
    Serial.print(motorControlData.motor1speed);
    Serial.print("  ");
    Serial.print(motorControlData.motor2speed);
    Serial.println();
    //

    //choosing motor directions in DFRobot L298N 
    if (motorControlData.motor1speed<0)
    {
      digitalWrite(motor1.controlPin,HIGH);
    }
    else if(motorControlData.motor1speed>0)
    {
      digitalWrite(motor1.controlPin,LOW);
    }
    analogWrite(motor1.pwmPin,abs(motorControlData.motor1speed));
    
    ///2nd motor
    if (motorControlData.motor2speed<0)
    {
      digitalWrite(motor2.controlPin,LOW);
    }
    else if(motorControlData.motor2speed>0)
    {
      digitalWrite(motor2.controlPin,HIGH);
    }
    analogWrite(motor2.pwmPin,abs(motorControlData.motor2speed));

    radioWaitTime=0;
  }
  else
  {
    while (!radio.available())
    {
      delay(1);
      radioWaitTime++;

      if (radioWaitTime>radioTimeOut)
      {
        Serial.print("        Radio non availale: ");
        Serial.print(radioWaitTime);
        Serial.println();

        motorControlData.motor1speed=0;
        motorControlData.motor2speed=0;
      }
      
    }
    
  }
  
  // if (radioWaitTime<=radioTimeOut)
  // {
  //   Serial.print("            RWT: ");
  //   Serial.print(radioWaitTime);
  //   Serial.println();
  // }

}