#include "BalanceBotMotor.h"

// constructor
BalanceBotMotor::BalanceBotMotor(){
    angle = 0.0;
    speed = 0.0;
    controlMode = 0;
}

// setup functions
void BalanceBotMotor::SetPwmPin(const int pin){
    pwmPin = pin;
    pinMode(pwmPin, OUTPUT);
}

void BalanceBotMotor::SetDirectionPins(const int pinA,
                                       const int pinB){
	directionPinA = pinA;
	directionPinB = pinB;
	pinMode(directionPinA, OUTPUT);
	pinMode(directionPinB, OUTPUT);
}

void BalanceBotMotor::SetStandbyPin(const int pin){
    standbyPin = pin;
    pinMode(standbyPin, OUTPUT);
}

void BalanceBotMotor::SetControlMode(int mode){
	controlMode = mode;
}

void BalanceBotMotor::SetPsiController(const float kp,
                                       const float ki,
									   const float kd,
									   const float reference){
    psiController.SetPID(kp, ki, kd);
    psiController.SetReference(reference);
}

void BalanceBotMotor::SetThetaController(const float kp,
                                         const float ki,
										 const float kd,
										 const float reference){
    thetaController.SetPID(kp, ki, kd);
    thetaController.SetReference(reference);
}

// update function
int BalanceBotMotor::UpdateControl(const float psi, const float theta,
                                    const float samplingTime){  // motor update
	float output = 0.0;
	float output2 = 0.0;
	
	switch(controlMode){
		case 0:
			output = 0;
			break;
		case 1:
			output = -psiController.Update(psi, samplingTime);
			break;/*
		case 2:
			output = -psiController.Update(psi, samplingTime);
			output2 = thetaController.Update(theta, samplingTime);
			output += output2;
			break;*/
	}
	
	int pwm = (int)(output* 255/12.0);
   	if(pwm > 255)
		pwm = 255;
	else if(pwm < -255)
		pwm = -255;
	
	
	return pwm;
    //Rotate( (int)(output* voltage2Pwm) );
}

int BalanceBotMotor::Update(float psi, float theta, float samplingTime){  // loop function
    return UpdateControl(psi, theta, samplingTime);
}

// reset function
void BalanceBotMotor::Reset(){
	angle = 0.0;
    speed = 0.0;
    //controlMode = 0;
}

// move function
void BalanceBotMotor::Rotate(int pwm){
	// Saturation
	// Max output: 255
	// Min output: -255
	if(pwm > MAX_OUTPUT)
		pwm = MAX_OUTPUT;
	else if(pwm < MIN_OUTPUT)
		pwm = MIN_OUTPUT;
	
    // speed: 0 ~ 255
    // direction: -1-> clockwise, 1-> counter-clockwise
    boolean pin1 = LOW;   //initial rotate direction: clockwise
    boolean pin2 = HIGH;
    if(pwm < 0){
        pin1 = HIGH;
        pin2 = LOW;
        pwm = -pwm;
    }

    digitalWrite(standbyPin, HIGH);
    digitalWrite(directionPinA, pin1);
    digitalWrite(directionPinB, pin2);
    analogWrite(pwmPin, pwm);
}

void BalanceBotMotor::ClearIntegral(){
	psiController.ClearIntegral();
}

int BalanceBotMotor::UpdateTheta(const float phi){  // loop function
    return thetaController.UpdateTheta(phi);
}
