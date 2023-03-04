#include <Arduino.h>

struct MotorControlData
{
    int motor1speed;
    int motor2speed;
};

struct DCMotor
{
    int controlPin; //idk why cant use static 
    int pwmPin;
};