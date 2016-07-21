#include "auv_mission_control/Follow_Marker.h"
#include <cmath>

Follow_Marker::Follow_Marker(){
}


Follow_Marker::Follow_Marker(Pid_Manager* pm, CameraManager* cam) : pm_(*pm), cam_(*cam){
}

Follow_Marker::~Follow_Marker(){

}

int Follow_Marker::execute(){

  pm_.setCamera(INPUT_CAM_BTM);
  pm_.pidEnable(AXIS_SURGE, false);
  pm_.pidEnable(AXIS_SWAY, true);
  pm_.pidEnable(AXIS_YAW, true);
  pm_.pidEnable(AXIS_HEAVE, true);

  pm_.zero(AXIS_YAW);
  pm_.setpoint_set(AXIS_YAW, INPUT_IMU_POS, 0);

  switch(action){
    case 0:{
      int counter = 0;
      if(counter < 1)
        depthTimer.start();
      counter ++;
      pm_.setpoint_set(AXIS_HEAVE, INPUT_DEPTH, -2.45);
      if(depthTimer.getTime() >= 10){
        action = 1;
        ROS_INFO("At depth, moving on");
        break
      }
    }

    case 1:{
      pm_.setpoint_set(AXIS_SWAY, INPUT_CAM_BTM, 320);
      setpoint_sway = 320;
      int counter = 0;
      if (counter < 1)
        swayTimer.start();
      counter ++;

      if(swayTimer.getTime() >= 5){
        action = 2;
        ROS_INFO("Sway in place, moving on to yaw");
        break;
      }

      }
    }

    case 2:{
      int counter = 0;
      if counter < 1
        yawTimer.start();
      counter ++;
      if(yawTimer.getTime() <= 5){
        pm_.setpoint_set(AXIS_YAW, INPUT_CAM_BTM, 0);
      }
      else{
        pm_.zero(AXIS_YAW);
        pm_.setpoint_set(AXIS_YAW, INPUT_IMU_POS, 0);
        action = 3;
        ROS_INFO("Yaw alighned, locking position and moving forwards");
        break;
      }
    }

    case 3:{

      

  }
}
