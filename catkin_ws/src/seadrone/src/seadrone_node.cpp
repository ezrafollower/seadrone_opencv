#include "ros/ros.h"
#include "std_msgs/String.h"
#include "sensor_msgs/Joy.h"
// udpUser
#include "CUDPCommUser.hpp"
#include <iostream>
#include <math.h>
using namespace std;
int arm_flag = -1;
int led_brightness_top = 0;
float gimbal_Pitch = 0;
float c_forces_moments_[2]={0};
double max_rotor = 3;
int depth_flag = 0;
int last_depth_flag = 0;
double delta_depth = 0;
double yaw = 0;

void joycallback(const sensor_msgs::Joy::ConstPtr& msg)
{
	//ROS_INFO("callback triggered");
  if (msg->buttons[7]){
    arm_flag = -arm_flag;
    if (arm_flag==1)
      ROS_INFO("rotors armed");
    else
      ROS_INFO("rotors disarmed");
    
  }
  if (msg->axes[1]==-1 && gimbal_Pitch<1.57)
    gimbal_Pitch+=0.314;
  if (msg->axes[1]==1 && gimbal_Pitch>0)
    gimbal_Pitch-=0.314;
  if (msg->axes[0]==-1 && led_brightness_top<100)
    led_brightness_top+=10;
  if (msg->axes[0]==1 && led_brightness_top>0)
    led_brightness_top-=10;
//motor
  c_forces_moments_[1] = (msg->axes[3])*max_rotor;
  c_forces_moments_[0] = (msg->axes[4])*max_rotor;
  //cout << c_forces_moments_[0] << endl;
  
  if(msg->axes[5]!=1 && msg->axes[2]==1){
    delta_depth=(msg->axes[5]-1); 
    delta_depth=abs(delta_depth);
    depth_flag=1;

  }
  else if(msg->axes[2]!=1 && msg->axes[5]==1){   
    delta_depth=(msg->axes[2]-1);   
    depth_flag=1;  
  }
  else{
    delta_depth=0;
    depth_flag=0;
  }
  if(msg->axes[6]==-1){
    yaw-=30;
  }
  if(msg->axes[6]==1){
    yaw+=30;
  }
  cout << delta_depth << " " << yaw << endl;
  
}


int main(int argc, char **argv) {

  ros::init(argc, argv, "seadrone_node");
  ros::NodeHandle n;
  ros::Subscriber sub = n.subscribe("joy", 1000, joycallback);
  printf("udp test started!\n");

  /** udp communication */
  orobotix::CUDPCommUser udpUser_;
  orobotix::SRobotDataUser robotData;

  // initialize the flag
  robotData.flag_enable_camera_[0] = 1;
  robotData.flag_enable_camera_[1] = 0;
  robotData.flag_enable_camera_[2] = 0;
  robotData.flag_enable_individual_control_ = false;
  robotData.flag_enable_thruster_ = false;
  //robotData.flag_change_control_gain_ = true;
  bool flag_run_ = true;

  // set communication
  robotData.droneIP_ = "192.168.0.122";
  //robotData.droneIP_ = "192.168.1.122";
  robotData.dronePort_ = 8090;
  robotData.config_id_ = 1;

  robotData.drone_please_power_off = false;

  
  //robotData.ctr_kp_[0] = 1;
  //robotData.ctr_kv_[0] = 1;
  //robotData.ctr_ki_[0] = 1;

  //robotData.goal_depth_ = -0.5;		//depth
  //robotData.depth_vel_ = 0.05;
  //robotData.gimbal_Pitch_ = 0;

  //robotData.ctr_kp_[1] = 1;
  //robotData.ctr_kp_[2] = 1;
  //robotData.ctr_kp_[3] = 1;
  //robotData.ctr_kp_[4] = 1;
  
  //robotData.led_brightness_top_ = 10;
  //robotData.led_brightness_bot_ = 10;

  // thruster
  robotData.initThrusterInfo(5);
  
  //robotData.c_forces_moments_[0] = 10;
  //robotData.goal_depth_ = 0;

  //robotData.goal_Eular_[0] = 90;		//right angle degree
  
  // initialize the udp communication
  udpUser_.init(&robotData);
  //robotData.flag_change_control_gain_ = true;
  //robotData.ctr_kp_[0] = 0.1;
  
  int count = 0;
  while (flag_run_) {

    robotData.gimbal_Pitch_ = gimbal_Pitch;
    robotData.led_brightness_top_ = led_brightness_top;
    if (arm_flag==1){
      robotData.flag_enable_thruster_ = true;
      robotData.c_forces_moments_[0]=c_forces_moments_[0];
      robotData.c_forces_moments_[1]=c_forces_moments_[1]; 
      robotData.c_forces_moments_[2] = delta_depth/200;


      robotData.goal_Eular_[0] = yaw;
      //robotData.goal_depth; 
      

      if (depth_flag == 0 && last_depth_flag == 1){
        robotData.flag_enable_thruster_ = false;
      };
      last_depth_flag = depth_flag;
    }
    else{
      robotData.flag_enable_thruster_ = false;

    }
    //cout << robotData.depth_ << endl;
    /*
    if (count >= 5 && count < 10)
        robotData.gimbal_Pitch_ += 0.314;	//camera platform
    if (count == 10)
        robotData.led_brightness_top_ = 10;	//0~100 led brghtness
    if (count == 13)
        robotData.led_brightness_top_ = 0;
    if (count == 15)
        robotData.flag_enable_thruster_ = true;	//unlock
        robotData.c_forces_moments_[0] = 5;
    if (count == 18)
        robotData.flag_enable_thruster_ = false;

        robotData.c_forces_moments_[0] = 0;	//0 for x axis, + for right, 1 for Y axis (y for the camera sight)
    */
    udpUser_.thread_communication_send();
    udpUser_.thread_communication_rev();

    ros::spinOnce();
    usleep(50); // wrong sleeping interval may cause rotors to fail

    //count++;
  }
  
  printf("\nMain: udpUser_.closeConnection() called!");
  //udpUser_.closeSocket();

  printf("\nTerminated robotDemo app!\n");
  return EXIT_SUCCESS;
}
