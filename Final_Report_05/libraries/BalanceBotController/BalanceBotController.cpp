#include "BalancebotController.h"

// constructor
BalanceBotController::BalanceBotController(){
    Kp = 0;
    Ki = 0;
    Kd = 0;
    reference = 0;
    integral = 0;
    preError = 0;
}

// set function
void BalanceBotController::SetPID(const float Kp, const float Ki, const float Kd){
    this->Kp = Kp;
    this->Ki = Ki;
    this->Kd = Kd;
}

void BalanceBotController::SetReference(const float reference){
    this->reference = reference;
}

// get function
String BalanceBotController::GetPID(){
	String data;
	return data;
}

float BalanceBotController::GetReference(){
	return reference;
}

// update
float BalanceBotController::Update(const float feedback, const float dt){
    float error = reference - feedback;

    // Proportional termW
    float pOut = Kp * error;

    // Integral term
    integral += error * dt;
    float iOut = Ki * integral;

    // Derivative term
    float derivative = ( error-preError ) / dt;
    float dOut = Kd * derivative;

    // Total output
    float output = pOut + iOut + dOut;
	
    // record the prevoius error
    preError = error;
    /*
    Serial.println("Controller");
    Serial.println(error);
    Serial.println(preError);
    Serial.println(dt, 4);
    Serial.println(dOut);
    Serial.println(output);
*/
    return output;
}

int BalanceBotController::UpdateTheta(const float feedback){
    float error = reference - feedback;
    
    // Proportional termW
    float pOut = Kp * error;
    
    int pwm = (int)(-pOut* 255/12.0);
   	if(pwm > 255)
		pwm = 255;
	else if(pwm < -255)
		pwm = -255;
		
	
	return pwm;
}

void BalanceBotController::ClearIntegral(){
    integral = 0;
}

