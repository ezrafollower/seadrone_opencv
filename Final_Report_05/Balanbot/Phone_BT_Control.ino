//phone BT control 手機搖桿
String cmd = "";
bool cmdNotEnd = false;
int val; // 儲存接收資料的變數

void phone_BT_control(){  
  bool fl = false; 
  if(!fl){
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
      fl = true;
  }
  if(BT.available()>0)
  {
    val = BT.read();    //是BT.read()不是Serial.read()
    delay(1000);
    switch(val){
      case 85:            //'U':   // car forward
                motor_A.ClearIntegral();
                motor_B.ClearIntegral();
                motor_A.Reset();
                motor_B.Reset();               

                motor_A.SetPsiController(Kp, Ki, Kd, -2.85);
                motor_B.SetPsiController(Kp, Ki, Kd, -2.85);
                updating_the_PID(pwm);
                motor_A.Rotate(pwm[0]);
                motor_B.Rotate(pwm[1]);  
                break;
                
      case 68:   // car backward
                motor_A.ClearIntegral();
                motor_B.ClearIntegral();
                motor_A.Reset();
                motor_B.Reset();
                timer_count = 0;
                  motor_A.SetPsiController(Kp, Ki, Kd, 3.5);
                  motor_B.SetPsiController(Kp, Ki, Kd, 3.5);
                  updating_the_PID(pwm);
                  motor_A.Rotate(pwm[0]);
                  motor_B.Rotate(pwm[1]);                    
                break;
                
      case 76:                // car turn left
                motor_A.ClearIntegral();
                motor_B.ClearIntegral();
                motor_A.Reset();
                motor_B.Reset();
                
                motor_A.SetPsiController(Kp, Ki, Kd, 0.5);
                motor_B.SetPsiController(Kp, Ki, Kd, 2.0);
                updating_the_PID(pwm);
                motor_A.Rotate(pwm[0]);
                motor_B.Rotate(pwm[1]);
                break;
                
      case 82:   // car turn right
                motor_A.ClearIntegral();
                motor_B.ClearIntegral();
                motor_A.Reset();
                motor_B.Reset();
                
                motor_A.SetPsiController(Kp, Ki, Kd, 2.0);
                motor_B.SetPsiController(Kp, Ki, Kd, 0.5);
                updating_the_PID(pwm);
                motor_A.Rotate(pwm[0]);
                motor_B.Rotate(pwm[1]);
                break;
                
      case 80:   // car stop
                //Serial.println("stop");
                motor_A.ClearIntegral();
                motor_B.ClearIntegral();
                motor_A.Reset();
                motor_B.Reset();
                
                  motor_A.SetPsiController(Kp, Ki, Kd, setpoint);
                  motor_B.SetPsiController(Kp, Ki, Kd, setpoint);
                  updating_the_PID(pwm);
                  motor_A.Rotate(pwm[0]);
                  motor_B.Rotate(pwm[1]);    
                
                break;
      case 70:         //fall
                motor_A.ClearIntegral();
                motor_B.ClearIntegral();
                motor_A.Reset();
                motor_B.Reset();
                motor_A.Rotate(0);
                motor_B.Rotate(0);          
    }
  }
  if(GetPsi()>30){
        motor_A.Rotate(pwm[0]);
        motor_B.Rotate(pwm[1]);
  }
  else{
        motor_A.SetControlMode(1);
        motor_A.SetPsiController(Kp, Ki, Kd, setpoint);
        motor_B.SetControlMode(1);
        motor_B.SetPsiController(Kp, Ki, Kd, setpoint);
        updating_the_PID(pwm);
        motor_A.Rotate(pwm[0]);
        motor_B.Rotate(pwm[1]);       
  }
}
