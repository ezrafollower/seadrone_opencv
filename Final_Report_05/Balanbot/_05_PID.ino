/*-------PID and BT_PID, updating_PID----------*/
void SetController(){
    motor_A.Reset();
    motor_B.Reset();

    motor_A.SetControlMode(1);
    motor_A.SetPsiController(Kp, Ki, Kd, setpoint);
    motor_A.SetThetaController(sKp, sKi, sKd, speed_setpoint);
    motor_B.SetControlMode(1);
    motor_B.SetPsiController(Kp, Ki, Kd, setpoint);
    motor_B.SetThetaController(sKp, sKi, sKd, speed_setpoint);    
}

void BT_PID(){              //receiving the data from the phone
  if (BT.available()){
      char c = BT.read();
      bool isEnd = (c == '#')?true:false;
      if(isEnd){
        parsePID();
      }
      else{
        cmdPID += String(c);
      }
  }
}

void parsePID(){              //decoding the info from phone
  int commaAt = -1;
  int dotAt = -1;               //區隔號"^"
  int percentAt = -1;           ////區隔號"%"
  int cmdLen = cmdPID.length();
  for(int i=0; i<cmdLen; ++i){
    if(cmdPID[i] == ','){
      commaAt = i;
    }
    else if(cmdPID[i] == '^'){
      dotAt = i;
    }
    else if(cmdPID[i] == '%'){
      percentAt = i;
    }
  }
  if(commaAt != -1){
    double BTKp = cmdPID.substring(0, commaAt).toDouble();
    double BTKi = cmdPID.substring(commaAt+1, dotAt).toDouble();
    double BTKd = cmdPID.substring(dotAt+1, percentAt).toDouble();
    double BTSetpoint = cmdPID.substring(percentAt+1).toDouble();
    Kp = BTKp;
    Ki = BTKi;
    Kd = BTKd; 
    setpoint = BTSetpoint;
  }
    cmdPID = "";
}

void send_data_to_phone(){                  //sending data to phone
    float psi_degree = (float)GetPsi();     //degree
    double speed_A = encoder_A.GetSpeed();    //rad/s  
    encoder_A.Update(0.01);
/*----sending data to phone-----*/  //BT transfer: println():傳整個字串、write():一次一個字元
    //BT.println("|");
    BT.println(psi_degree);   //degree
    //BT.println("~");
    BT.println(speed_A);
    //Serial.println(psi_degree);    
    //Serial.println(speed_A);   
    
}

void updating_the_PID(int pwm[]){                    //keep updating the state of the car
    float psi = ((float)GetPsi())/180*PI;   //rad
    //Serial.println( "psi" + String(psi, 2) );
    float theta=0.0;
    //encoder_A.Update(0.05);
    //encoder_B.Update(0.05);
    //theta = (float)encoder_A.GetAngle();   //rad
      /*compute the sampling time*/
    currentTime = millis();
    samplingTime = (currentTime - previousTime)/1000.0;   //單位變成ms
    if(samplingTime < 0.001){
      samplingTime = 0.001;
    }
    
    /* update controller*/
    pwm[0] = motor_A.Update(psi, theta, samplingTime);
    pwm[1] = motor_B.Update(psi, theta, samplingTime);
    //Serial.println(pwm[0]);
    //Serial.println(pwm[1]);

    /* record the previous time*/
    previousTime = currentTime; 
}


