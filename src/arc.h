#include <Arduino.h>

struct MotorControlData
{
    int motor1speed=0;
    int motor2speed=0;
};

struct DCMotor
{
    int controlPin; //idk why cant use static 
    int pwmPin;
};

int analogReadCorrection(int x,int error=20)
{
    if ( abs(x) < error )
    {
        x = 0;
    }
    return x;
}

void motorHandling(MotorControlData &motorControlData,DCMotor &motor1, DCMotor &motor2)
{
    //choosing motor directions in DFRobot L298N 
    if (motorControlData.motor1speed<0)
    {
      digitalWrite(motor1.controlPin,LOW);
    }
    else if(motorControlData.motor1speed>0)
    {
      digitalWrite(motor1.controlPin,HIGH);
    }
    analogWrite(motor1.pwmPin,abs(motorControlData.motor1speed));
    
    ///2nd motor
    if (motorControlData.motor2speed<0)
    {
      digitalWrite(motor2.controlPin,HIGH);
    }
    else if(motorControlData.motor2speed>0)
    {
      digitalWrite(motor2.controlPin,LOW);
    }
    analogWrite(motor2.pwmPin,abs(motorControlData.motor2speed));
}




