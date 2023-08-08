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

unsigned long lastReceiveTime = 0;
unsigned long currentTime = 0;
unsigned long waitingTime = 0;

bool logging = false; //enables loggin based on Serial.available()

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

  if (Serial.available())
  {
    logging = true;
  }
  
}


void loop()
{

  if (radio.available()) 
  {
    lastReceiveTime = millis();

    radio.read(&motorControlData, sizeof(motorControlData));
    motorHandling(motorControlData,motor1,motor2);
  }
  else
  {
    currentTime = millis();

    //Serial.print("WAIT_TIME: ");
    waitingTime = (currentTime - lastReceiveTime);
    //Serial.print(currentTime - lastReceiveTime);
    //Serial.println();

    if ( currentTime - lastReceiveTime > 50 ) 
    { 
      motorControlData.motor1speed=0;
      motorControlData.motor2speed=0;
      motorHandling(motorControlData,motor1,motor2);
    }

  }


  Serial.print(waitingTime);
  Serial.print("  ");
  Serial.print(motorControlData.motor1speed);
  Serial.print("  ");
  Serial.print(motorControlData.motor2speed);
  Serial.print("  ");
  Serial.println();
  delay(10);
}