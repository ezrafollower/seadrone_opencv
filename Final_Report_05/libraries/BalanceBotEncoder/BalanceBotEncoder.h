#ifndef BALANCEBOTENCODER_H
#define BALANCEBOTENCODER_H

#include <Arduino.h>
#define PPR 390		//轉一圈encoder 數390次 

class BalanceBotEncoder{
  private:
    int motorSide;    // left: -1, right: 1
    // pin
    int interruptPin;
    int directionPin;
    
    // others
    int position;      // wheel angle (PPR)
    float angle;       // wheel angle (rad)
    float speed;       // wheel speed (rad/s)
    
   
  public:
    // constructor
    BalanceBotEncoder(); 

    // set funciton
    void SetMotorSide(const int side);
    void SetPin(const int pin1, const int pin2);
    
    // get function
    int GetMotorSide();
    int GetInterruptPin();
    float GetAngle();
    float GetSpeed();
    
    // update fuction
    void UpdatePosition();
    void Update(const float samplingTime);
    
    // reset function
    void Reset();
};

#endif //BALANCEBOTENCODER_H
