#include <Arduino.h>
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include "arc.h"




//left joy
const int left=A0;
int leftPosition=0;

//right joy
const int right=A2;
int rightPosition=0;


MotorControlData motorControlData;



//transmitter
RF24 radio(9, 10); // CE, CSN         
const byte address[6] = "00001";   //Byte of array representing the address. This is the address where we will send the data. This should be same on the receiving side.

void setup() 
{   
    motorControlData.motor1speed=0;
    motorControlData.motor2speed=0;

    Serial.begin(9600);

    pinMode(left,INPUT);
    pinMode(right,INPUT);

    radio.begin();                  //Starting the Wireless communication
    radio.openWritingPipe(address); //Setting the address where we will send the data
    radio.setPALevel(RF24_PA_MAX);  //You can set it as minimum or maximum depending on the distance between the transmitter and receiver.
    radio.stopListening();          //This sets the module as transmitter

    radio.printDetails();
}


void loop()
{
    leftPosition=analogRead(left);
    rightPosition=analogRead(right);

    motorControlData.motor1speed=analogReadCorrection(map(leftPosition,0,1023,255,-255)); //left
    motorControlData.motor2speed=analogReadCorrection(map(rightPosition,0,1023,255,-255));//right

    //serial debug info
    Serial.print("CONTROLL ");
    Serial.print("L: ");
    Serial.print(motorControlData.motor1speed);
    Serial.print("  P: "); 
    Serial.print(motorControlData.motor2speed);
    Serial.println();
    //

    radio.write(&motorControlData, sizeof(motorControlData));
}
