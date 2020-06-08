/*--------anything related to motor----------*/
void SetupMotor(){
    // left motor
    motor_A.SetPwmPin(PWMA);
    motor_A.SetDirectionPins(AIN1, AIN2);
    motor_A.SetStandbyPin(STBY);

    encoder_A.SetMotorSide(-1);
    encoder_A.SetPin(INTA, DIRA);
    attachInterrupt(digitalPinToInterrupt(encoder_A.GetInterruptPin()), Encoder_A_Interrupt, RISING);

    motor_A.SetControlMode(1);                       // psi
    motor_A.SetPsiController(Kp, Ki, Kd, setpoint);  // KP, KI, KD, Reference
    
    // right motor
    motor_B.SetPwmPin(PWMB);
    motor_B.SetDirectionPins(BIN1, BIN2);
    motor_B.SetStandbyPin(STBY);

    encoder_B.SetMotorSide(1);
    encoder_B.SetPin(INTB, DIRB);
    attachInterrupt(digitalPinToInterrupt(encoder_B.GetInterruptPin()), Encoder_B_Interrupt, RISING);

    motor_B.SetControlMode(1);                       // psi
    motor_B.SetPsiController(Kp, Ki, Kd, setpoint);  // KP, KI, KD, Reference
}

void initMotor() 
{
  SetupMotor();
  pinMode(STBY, OUTPUT);                                                    // for standby
  pinMode(PWMA, OUTPUT);  pinMode(AIN1, OUTPUT);  pinMode(AIN2, OUTPUT);    // for motor A
  pinMode(PWMB, OUTPUT);  pinMode(BIN1, OUTPUT);  pinMode(BIN2, OUTPUT);    // for motor B
}

void StableVoltage(const int voltage){        // voltage>0：move forward；voltage<0：move backward
    motor_A.Rotate( voltage * 255/12.0 );     
    motor_B.Rotate( voltage * 255/12.0 );     
}


/*-------move for phone_bt_control-----------*/
void move(int motor, int speed, int direction) 
{
 // Move specific motor at speed and direction
 // motor: 1 for A(left), 2 for B(right)
 // speed: 0 is off, and 255 is full speed
 // direction: 0 clockwise, 1 counter-clockwise
  boolean inPin1 = LOW;                         // 初始轉動方向為 clockwise
  boolean inPin2 = HIGH;
  //digitalWrite(STBY, HIGH);                     // disable standby(可動)
  if(direction == 1) 
  {                          // 若 direction 指定為 1 時，轉動方向為 counterclockwise
    inPin1 = HIGH;
    inPin2 = LOW;
  }
  if(motor == 1) 
  {                              // 指定 motor A left
    digitalWrite(AIN1, inPin1);
    digitalWrite(AIN2, inPin2);
    analogWrite(PWMA, speed);
  }
  else if(motor == 2) 
  {                         // 指定 motor B right
    digitalWrite(BIN1, inPin1);
    digitalWrite(BIN2, inPin2);
    analogWrite(PWMB, speed);
  }
}

void moveMotor(int whichMotor, float power)
{
  bool powerIsNeg = power < 0;
  power *= powerIsNeg?-1.0:1.0;
  if(whichMotor == RIGHT)
  {
    move(RIGHT, (byte)power, powerIsNeg?1:0);
  } 
  else if(whichMotor == LEFT)
  {
    move(LEFT, (byte)power, powerIsNeg?0:1);    
  }
}
