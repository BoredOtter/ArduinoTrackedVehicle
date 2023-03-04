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

void setup() 
{
  //DC motor setup using structs


  motor1.controlPin=7;
  motor1.pwmPin    =6;

  motor2.controlPin=4;
  motor2.pwmPin    =5;

  pinMode(motor1.controlPin,OUTPUT);
  pinMode(motor2.controlPin,OUTPUT);
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
      analogWrite(motor1.pwmPin,abs(motorControlData.motor1speed));
    }
    else if(motorControlData.motor1speed>0)
    {
      digitalWrite(motor1.controlPin,LOW);
      analogWrite(motor1.pwmPin,abs(motorControlData.motor1speed));
    }
    else if (motorControlData.motor1speed==0)
    {
      analogWrite(motor1.pwmPin,0);
    }
    
    ///2nd motor
    if (motorControlData.motor2speed<0)
    {
      digitalWrite(motor2.controlPin,LOW);
      analogWrite(motor2.pwmPin,abs(motorControlData.motor2speed));
    }
    else if(motorControlData.motor2speed>0)
    {
      digitalWrite(motor2.controlPin,HIGH);
      analogWrite(motor2.pwmPin,abs(motorControlData.motor2speed));
    }
    else if (motorControlData.motor2speed==0)
    {
      analogWrite(motor2.pwmPin,0);
    }
  }

  Serial.println("          !Radio");
}