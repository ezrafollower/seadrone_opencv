#include "BalanceBotEncoder.h"

// constructor
BalanceBotEncoder::BalanceBotEncoder(){
    position = 0;
    angle = 0;
    speed = 0;
}

// set function
void BalanceBotEncoder::SetMotorSide(const int side){
    motorSide = side;
}

void BalanceBotEncoder::SetPin(const int pin1, const int pin2){
    interruptPin = pin1;
    
    directionPin = pin2;
    pinMode(directionPin, INPUT);
}

// get function
int BalanceBotEncoder::GetMotorSide(){
    return motorSide;
}

int BalanceBotEncoder::GetInterruptPin(){
    return interruptPin;
}

float BalanceBotEncoder::GetAngle(){
    return angle * motorSide;
}

float BalanceBotEncoder::GetSpeed(){
    return speed * motorSide;
}

// update function
void BalanceBotEncoder::UpdatePosition(){  // interrupt
    digitalRead(directionPin) == HIGH ? position++ : position--;
}

void BalanceBotEncoder::Update(const float samplingTime){  // motor update
    float newAngle = 2 * PI * ((float)position / (float)PPR);			//rad
	speed = (newAngle - angle) / samplingTime;
    angle = newAngle;
    
}

// reset funciton
void BalanceBotEncoder::Reset(){
    position = 0;
    angle = 0.0;
    speed = 0.0;
}
