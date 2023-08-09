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
unsigned long timeOut = 20; //time in ms with no connection with rc after motorhandligdata will be set to 0

void setup() 
{
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

  if (radio.available()) 
  {
    radio.read(&motorControlData, sizeof(motorControlData));
    lastReceiveTime = millis(); //time of last reveice of data
    
    motorHandling(motorControlData,motor1,motor2);
  }
  else
  {
    currentTime = millis();
    waitingTime = ( currentTime - lastReceiveTime );

    Serial.print(" WAIT_TIME: ");
    Serial.print( waitingTime );
    Serial.println();

    if ( waitingTime > timeOut ) 
    { 
      motorControlData.motor1speed=0;
      motorControlData.motor2speed=0;
      motorHandling(motorControlData,motor1,motor2);
    }

  }

  // idk why waitingTime only works correctly when displayed from else statement
  // in case like below:
  // Serial.print(" WAIT_TIME: ");
  // Serial.print( waitingTime );
  // blocks itself on 1700+ value
  Serial.print(motorControlData.motor1speed);
  Serial.print("  ");
  Serial.print(motorControlData.motor2speed);
  Serial.println();

}