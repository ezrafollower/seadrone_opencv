//bool temp_f = false;        
char cmd2;
void vision_detection(){
    cmd2 = Serial.read();
   
  if(cmd2 == '1' || cmd2 == '2' || cmd2 == '3'){
      //senddata = "stop 1 second";    
      motor_A.ClearIntegral();
      motor_B.ClearIntegral();
      motor_A.Reset();
      motor_B.Reset();
      
      motor_A.SetControlMode(1);
      motor_A.SetPsiController(Kp, Ki, Kd, setpoint);
      motor_B.SetControlMode(1);
      motor_B.SetPsiController(Kp, Ki, Kd, setpoint);
      updating_the_PID(pwm);
      motor_A.Rotate(pwm[0]);
      motor_B.Rotate(pwm[1]);
  }
 /*   else if (cmd2 == '2'){
      //senddata = "stop 2 second";
      motor_A.ClearIntegral();
      motor_B.ClearIntegral();
      motor_A.Reset();
      motor_B.Reset();

      motor_A.SetControlMode(1);
      motor_A.SetPsiController(Kp, Ki, Kd, setpoint);
      motor_B.SetControlMode(1);
      motor_B.SetPsiController(Kp, Ki, Kd, setpoint);
      updating_the_PID(pwm);
      motor_A.Rotate(pwm[0]);
      motor_B.Rotate(pwm[1]);
  }

  else if (cmd2 == '3'){
      //senddata = "stop 3 second";
      motor_A.ClearIntegral();
      motor_B.ClearIntegral();
      motor_A.Reset();
      motor_B.Reset();
      
      motor_A.SetControlMode(1);
      motor_A.SetPsiController(Kp, Ki, Kd, setpoint);
      motor_B.SetControlMode(1);
      motor_B.SetPsiController(Kp, Ki, Kd, setpoint);
      updating_the_PID(pwm);
      motor_A.Rotate(pwm[0]);
      motor_B.Rotate(pwm[1]);
  }*/
 
  else if (cmd2 == '4'){
    //senddata = "Turn Left and Move 1";
    static_turn(7, 2);    //turn(7, 2) 12.5V
    
    motor_A.ClearIntegral();
    motor_B.ClearIntegral();
    motor_A.Reset();
    motor_B.Reset();
    
    timer_count = 0;
    while(timer_count<300){
      motor_A.SetControlMode(1);
      motor_A.SetPsiController(Kp, Ki, Kd, setpoint);
      motor_B.SetControlMode(1);
      motor_B.SetPsiController(Kp, Ki, Kd, setpoint);
      updating_the_PID(pwm);
      motor_A.Rotate(pwm[0]);
      motor_B.Rotate(pwm[1]);      
    } 
    //static_move12(25, 1, 0);
  }
  else if (cmd2 == '5'){      
    //senddata = "Turn Left and Move 2";
    static_turn(7, 2);    //turn(7, 2) 12.5V
    
    motor_A.ClearIntegral();
    motor_B.ClearIntegral();
    motor_A.Reset();
    motor_B.Reset();
    
    timer_count = 0;
    while(timer_count<300){
      motor_A.SetControlMode(1);
      motor_A.SetPsiController(Kp, Ki, Kd, setpoint);
      motor_B.SetControlMode(1);
      motor_B.SetPsiController(Kp, Ki, Kd, setpoint);
      updating_the_PID(pwm);
      motor_A.Rotate(pwm[0]);
      motor_B.Rotate(pwm[1]);      
    } 
    //static_move12(25, 1, 0);
  }

  else if (cmd2 == '6'){
    //senddata = "Turn Left and Move 3";
    //Serial.println(senddata);

    static_turn(7, 2);    //turn(7, 2) 12.5V,   turn(6.8) 12.2V
    
    motor_A.ClearIntegral();
    motor_B.ClearIntegral();
    motor_A.Reset();
    motor_B.Reset();
    
    timer_count = 0;
    while(timer_count<300){
      motor_A.SetControlMode(1);
      motor_A.SetPsiController(Kp, Ki, Kd, setpoint);
      motor_B.SetControlMode(1);
      motor_B.SetPsiController(Kp, Ki, Kd, setpoint);
      updating_the_PID(pwm);
      motor_A.Rotate(pwm[0]);
      motor_B.Rotate(pwm[1]);      
    } 
    //static_move12(50, 1, 0);        //25是40cm,  40 = 80cm
  }
  
  else if (cmd2 == '7'){
    //senddata = "Turn Right and Move 1";
    static_turn(6.8, 1);    //turn(7, 2) 12.5V,   turn(6.8) 12.2V
    
    motor_A.ClearIntegral();
    motor_B.ClearIntegral();
    motor_A.Reset();
    motor_B.Reset();
    
    timer_count = 0;
    while(timer_count<300){
      motor_A.SetControlMode(1);
      motor_A.SetPsiController(Kp, Ki, Kd, setpoint);
      motor_B.SetControlMode(1);
      motor_B.SetPsiController(Kp, Ki, Kd, setpoint);
      updating_the_PID(pwm);
      motor_A.Rotate(pwm[0]);
      motor_B.Rotate(pwm[1]);      
    }
    //static_move12(25, 1, 0);
  }
  
  else if (cmd2 == '8'){
    //senddata = "Turn Right and Move 2";
    static_turn(6.8, 1);    //turn(7, 2) 12.5V,   turn(6.8) 12.2V
    
    motor_A.ClearIntegral();
    motor_B.ClearIntegral();
    motor_A.Reset();
    motor_B.Reset();
    
    timer_count = 0;
    while(timer_count<300){
      motor_A.SetControlMode(1);
      motor_A.SetPsiController(Kp, Ki, Kd, setpoint);
      motor_B.SetControlMode(1);
      motor_B.SetPsiController(Kp, Ki, Kd, setpoint);
      updating_the_PID(pwm);
      motor_A.Rotate(pwm[0]);
      motor_B.Rotate(pwm[1]);      
    }
    //static_move12(40, 1, 0);
  }

  else if (cmd2 == '9'){
    //senddata = "Turn Right and Move 3";
    static_turn(6.5, 1);    //turn(7, 2) 12.5V,   turn(6.8) 12.2V
    
    motor_A.ClearIntegral();
    motor_B.ClearIntegral();
    motor_A.Reset();
    motor_B.Reset();
    
    while(timer_count<300){
      motor_A.SetControlMode(1);
      motor_A.SetPsiController(Kp, Ki, Kd, setpoint);
      motor_B.SetControlMode(1);
      motor_B.SetPsiController(Kp, Ki, Kd, setpoint);
      updating_the_PID(pwm);
      motor_A.Rotate(pwm[0]);
      motor_B.Rotate(pwm[1]);      
    } 
    //static_move12(50, 1, 0);        //25是40cm,  40 = 80cm
  }

      timer_count = 0;
      while(timer_count < 200){
        motor_A.SetControlMode(1);
        motor_A.SetPsiController(Kp, Ki, Kd, setpoint-0.005);   //-0.018
        motor_B.SetControlMode(1);
        motor_B.SetPsiController(Kp, Ki, Kd, setpoint-0.005);
        updating_the_PID(pwm);
        motor_A.Rotate(pwm[0]);     //pwm[0]*0.811
        motor_B.Rotate(pwm[1]);         
      }
       
      timer_count = 0;
      while(timer_count < 100){
        motor_A.SetControlMode(1);
        motor_A.SetPsiController(Kp, Ki, Kd, setpoint);
        motor_B.SetControlMode(1);
        motor_B.SetPsiController(Kp, Ki, Kd, setpoint);
        updating_the_PID(pwm);
        motor_A.Rotate(pwm[0]);
        motor_B.Rotate(pwm[1]);   
      }
      cmd2 = "";
}
