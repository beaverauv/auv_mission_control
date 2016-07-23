#include "auv_mission_control/Follow_Marker.h"
#include <cmath>

Follow_Marker::Follow_Marker(){
}


Follow_Marker::Follow_Marker(PidManager* pm, CameraManager* cam) : pm_(*pm), cam_(*cam){
}

Follow_Marker::~Follow_Marker(){

}

int Follow_Marker::execute(){

  pm_.setCamera(INPUT_CAM_BTM);
  pm_.setPidEnabled(AXIS_SURGE, false);
  pm_.setPidEnabled(AXIS_SWAY, true);
  pm_.setPidEnabled(AXIS_YAW, true);
  pm_.setPidEnabled(AXIS_HEAVE, true);

  pm_.setZero(AXIS_YAW);
  pm_.setpoint_set(AXIS_YAW, INPUT_IMU_POS, 0);

  switch(action){
    case 0:{
      depthTimer.start();
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
      if(fabs(plantState_sway - setpoint_sway) <= 20){
        if (counter < 1){
          swayTimer.start();
        }
        if(swayTimer.getTime() >= 5){
          action = 2;
          ROS_INFO("Sway in place, moving on to yaw");
          break;
        }
      }
    }

    case 2:{
      yawTimer.start();
      if(yawTimer.getTime() <= 5){
        pm_.setpoint_set(AXIS_YAW, INPUT_CAM_BTM, 0);
      }
      else{
        action = 3;
        ROS_INFO("Yaw alighned, locking position and moving forwards");
        break;
      }
    }

    case 3:

  }
}
