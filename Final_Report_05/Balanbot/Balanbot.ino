#include <I2Cdev.h>
#include <MPU6050.h>
#include <MsTimer2.h>
#include <BalanceBotMotor.h>
#include <Wire.h>
#include <Kalman.h>
#include <SoftwareSerial.h>
#include <Timer.h>

/*-----定義連接藍牙模組的序列埠----*/
SoftwareSerial BT(12, 13);  //Tx, Rx
/*-- pin setup ------------------------------------------------------------------*/
// If batterys are at backward & user see the car from the backward
// motor A(right motor) connected between A01 and A02
// motor B(left  motor) connected between B01 and B02
const int PWMA = 5, PWMB = 9;               // Speed control 
const int AIN1 = 6, BIN1 = 11;              // Direction +
const int AIN2 = 4, BIN2 = 10;              // Direction -
const int STBY = 7;                         // standby(停止)

const int INTA = 2, INTB = 3;               // Encoder interrupt
const int DIRA = A3, DIRB = 8;              // Encoder direction

const float R = 3.25;
const float W = 18.7;
float thetaL;
float thetaR;

/*-----create new objects(motors)---*/
BalanceBotMotor motor_A;
BalanceBotMotor motor_B;
BalanceBotEncoder encoder_A;
BalanceBotEncoder encoder_B;

/*----------MPU6050-------------------*/
 int LEFT = 1;
 int RIGHT = 2;
 float maxPowerRemap = 255/100;

/*---sampling---------*/
unsigned long previousTime;
unsigned long currentTime;
float samplingTime;    //for model ID: 0.01; balancing car: 0.001sec

/*----------PID-------------------*/
void BT_PID();        //BT from phone to change PID of car
void parsePID();      //parsing for BT_PID();
String cmdPID = "";

double Kp = 70, Ki = 550, Kd = 0.8;//Kp = 70, Ki = 550, Kd = 0.8;         //75, 600, 0.8, 0.01     //for psi control
double setpoint = 0.02;//setpoint = 0.01; //平地 -0.01; // 0.015            //psi應該平衡的角度

double sKp=10.00, sKi = 0.00, sKd = 0;           //for theta control(speed)
double speed_setpoint = 0;                //theta應該平衡的角度
int pwm[2] = {0, 0};
float phi;

/*-- 副函式定義 ---------------*/
void parseCmd();        //parsing for phone_bt_control
double GetPsi();        //getting psi (in degree)

int timer_count = 0;
/*----------setup-------------------*/
void setup() 
{
  Serial.begin(9600); 
  BT.begin(57600);        //115200 is too fast, might cause package lost
  Serial.println("BT is ready!");
    SetupMotor();
    SetupMPU6050();
    SetupMsTimer();

    previousTime = millis();  // initialize previous time
}
/*---------loop--------------------*/
bool flag1 = false;
bool flag2 = false;
bool temp_f = false;
String getdata, senddata;
//String cmd2 = "";
char Move, Color;
bool startReadData = false;

void loop() {   
   //send_data_to_phone();
   sei();   
   
   encoder_A.Update(0.05);
   encoder_B.Update(0.05);
   thetaL = encoder_A.GetAngle();
   thetaR = encoder_B.GetAngle();
   phi = R*(thetaL - thetaR)/W;
   
  /*--------第一題----------------*/
 /*  if(!flag1){
   motor_A.ClearIntegral();
   motor_B.ClearIntegral();
   static_move12(100, 1, 0.83);
   static_turn(6, 2);       
   //static_move12(100, 1, 0.7); 
   //static_turn(9.5, 2);
   //static_move12(100, 1, 0.5); 
   flag1 = true;    
   }
    else{
      if(!flag2){
        motor_A.ClearIntegral();
        motor_B.ClearIntegral(); 
        //Serial.println("clear");
        motor_A.SetControlMode(1);
        motor_A.SetPsiController(Kp, Ki, Kd, setpoint);
        motor_B.SetControlMode(1);
        motor_B.SetPsiController(Kp, Ki, Kd, setpoint);
        updating_the_PID(pwm);
        motor_A.Rotate(pwm[1]);
        motor_B.Rotate(pwm[1]);
        flag2 = true;
      }
      
      motor_A.SetControlMode(1);
      motor_A.SetPsiController(Kp, Ki, Kd, setpoint);
      motor_B.SetControlMode(1);
      motor_B.SetPsiController(Kp, Ki, Kd, setpoint);
      updating_the_PID(pwm);
      motor_A.Rotate(pwm[0]);
      motor_B.Rotate(pwm[1]);
      //Serial.println(pwm[0]);
      //Serial.println(pwm[1]);
    }
    */
    /*-----------第二題----------*/
    phone_BT_control();    

    /*-----------第三題----------*/
    //vision_detection();
    
 /*------testing-------------*/   
 /*     motor_A.SetControlMode(1);
      motor_A.SetPsiController(Kp, Ki, Kd, setpoint);
      motor_B.SetControlMode(1);
      motor_B.SetPsiController(Kp, Ki, Kd, setpoint);
      updating_the_PID(pwm);
      motor_A.Rotate(pwm[0]);
      motor_B.Rotate(pwm[1]);
      Serial.println(pwm[0]);
      Serial.println(pwm[1]);*/
      /*
  if(!temp_f){
    static_turn(6.5, 2);    //turn(7, 2) 12.5V,   turn(6.8) 12.2V
    
    motor_A.ClearIntegral();
    motor_B.ClearIntegral();
    motor_A.Reset();
    motor_B.Reset();
    
    timer_count = 0;
    while(timer_count<100){
      motor_A.SetControlMode(1);
      motor_A.SetPsiController(Kp, Ki, Kd, setpoint);
      motor_B.SetControlMode(1);
      motor_B.SetPsiController(Kp, Ki, Kd, setpoint);
      updating_the_PID(pwm);
      motor_A.Rotate(pwm[0]);
      motor_B.Rotate(pwm[1]);      
    } 
   // static_move12(40, 1, 0);        //25是40cm,  40 = 80cm
    temp_f = true;
  }
      motor_A.SetControlMode(1);
      motor_A.SetPsiController(Kp, Ki, Kd, setpoint);
      motor_B.SetControlMode(1);
      motor_B.SetPsiController(Kp, Ki, Kd, setpoint);
      updating_the_PID(pwm);
      motor_A.Rotate(pwm[0]);
      motor_B.Rotate(pwm[1]);  
      */
 /*     motor_A.SetControlMode(1);
      motor_A.SetPsiController(Kp, Ki, Kd, setpoint-0.02);
      motor_B.SetControlMode(1);
      motor_B.SetPsiController(Kp, Ki, Kd, setpoint-0.02);
      updating_the_PID(pwm);
      motor_A.Rotate(pwm[0]);
      motor_B.Rotate(pwm[1]); */
  
/*----------------------*/  
    
}
