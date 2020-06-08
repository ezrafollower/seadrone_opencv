/*balancing moving functions*/
void static_move12(float distance, int direct, float part){    //distance: cm    1:向前 2:向後
  float theta = 0.0, newtheta;
  double new_setpoint = setpoint;             //setpoint;
  float dis = distance*part;               //12.4V: 6/7, 12.3:5/6, 愈走不到分母愈小 
  bool flag = false;
  
  //Serial.println("123");
  motor_A.ClearIntegral();
  motor_B.ClearIntegral();   
  motor_A.Reset();
  motor_B.Reset();
        
  while(distance > 0.0){
    encoder_B.Update(0.01);
    if(direct == 1){    //向前走
      if(distance > dis){
        if(new_setpoint > -0.03){
          new_setpoint -= 0.001;     //psi<0 向前          
        }
      }
      else if(distance <= dis){
        if(new_setpoint < 0.03){
          new_setpoint += 0.0007;     //psi>0向後      
        }
      }
      motor_A.SetPsiController(Kp, Ki, Kd, new_setpoint);
      motor_B.SetPsiController(Kp, Ki, Kd, new_setpoint);
      updating_the_PID(pwm);      
      motor_A.Rotate(pwm[0]);
      motor_B.Rotate(pwm[1]);
      
      newtheta = encoder_B.GetAngle();   //rad
      distance -= (-6.6*(newtheta-theta)/2);
      theta = newtheta;
    }
    else if(direct == 2){       //向後
      if(distance > dis){
        if(new_setpoint < 0.03){
          new_setpoint += 0.0005;     //psi>0向後      
        }
      }
      else if(distance <= dis){
        if(new_setpoint > -0.03){
          new_setpoint -= 0.0007;      //psi<0 向前          
        }
      }
      motor_A.SetPsiController(Kp, Ki, Kd, new_setpoint);
      motor_B.SetPsiController(Kp, Ki, Kd, new_setpoint);
      updating_the_PID(pwm);    
      motor_A.Rotate(pwm[0]);
      motor_B.Rotate(pwm[1]);  

      encoder_B.Update(0.01);
      newtheta = encoder_B.GetAngle();   //rad
      distance -= (-6.6*(newtheta-theta)/2);
      //Serial.println(newtheta-theta);
      theta = newtheta;    
    }
  }
}

void static_turn(float turn_angle, float turn_direction){     //angle:90, 180, 270; direction: 1:turn_right(A), 2:turn_left(B)
  float theta = 0.0, newtheta; 
  double A_setpoint=0.015, B_setpoint=0.015;
  float A_theta = 0.0, B_theta = 0.0, A_theta_new = 0.0, B_theta_new = 0.0;
  float angle = 0.0, angle_new = 0.0;
  int a, b;
  bool flag = false;
  
  //Serial.println("456");
        motor_A.ClearIntegral();
        motor_B.ClearIntegral();    
  while(angle_new < turn_angle){
      encoder_A.Reset();
      encoder_B.Reset();
      encoder_B.Update(0.05);
      encoder_A.Update(0.05);
      
      if(turn_direction == 1){      //turn right
        A_setpoint = 0.005;
        B_setpoint = -0.005;
      }
      else if(turn_direction == 2){    //turn left
        A_setpoint = -0.005;
        B_setpoint = 0.005;
      }
        
        motor_A.SetPsiController(Kp, Ki, Kd, A_setpoint);
        motor_B.SetPsiController(Kp, Ki, Kd, B_setpoint);
        updating_the_PID(pwm);
        motor_A.Rotate(pwm[0]);
        motor_B.Rotate(pwm[1]);

        encoder_B.Update(0.05);
        encoder_A.Update(0.05);
        B_theta = encoder_B.GetAngle();
        A_theta = encoder_A.GetAngle();
        
        if(A_theta>=0 && B_theta<=0){
          angle = A_theta -(B_theta);
          //Serial.println("11111");
        }
        else if(A_theta<0 && B_theta>0){
          angle = -(A_theta) + B_theta;
           //Serial.println("22222");
        }
        else if(A_theta<0 && B_theta<0){
          if(-(B_theta)> -(A_theta)){
            angle = -(B_theta) - (A_theta);
            //Serial.println(B_theta);
            //Serial.println(A_theta);
             //Serial.println("33333");            
          }
          else{
            angle = -A_theta - B_theta;
             //Serial.println("44444");            
          }
         }
        else {
          if((B_theta)>(A_theta)){
            angle = (B_theta) - (A_theta);
             //Serial.println("55555");            
          }
          else{
            angle = A_theta - B_theta;
             //Serial.println("66666");            
          }          
        }
        angle_new += angle;
        //Serial.println(angle_new);
  }
}

