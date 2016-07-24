#include <auv_mission_control/TaskMarker.h>

TaskMarker::TaskMarker(){
}


TaskMarker::TaskMarker(PidManager* pm, Camera* cam) : pm_(*pm), cam_(*cam){
}

TaskMarker::~TaskMarker(){

}

int TaskMarker::execute(){

  pm_.setPidEnabled(AXIS_SURGE, false);
  pm_.setPidEnabled(AXIS_SWAY, true);
  pm_.setPidEnabled(AXIS_YAW, true);
  pm_.setPidEnabled(AXIS_HEAVE, true);

  pm_.setZero(AXIS_YAW);
  pm_.setSetpoint(AXIS_YAW, INPUT_IMU_POS, 0);

  switch(action){
    case 0:{
      int counter = 0;
      if(counter < 1)
        depthTimer.start();
      counter ++;
      pm_.setSetpoint(AXIS_HEAVE, INPUT_DEPTH, -2.45);
      if(depthTimer.getTime() >= 10){
        action = 1;
        ROS_INFO("At depth, moving on");
        break;
      }
    }

    case 1:{
      pm_.setSetpoint(AXIS_SWAY, INPUT_CAM_BTM, 320);
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

    case 2:{
      int counter = 0;
      if (counter < 1)
        yawTimer.start();
      counter ++;
      if(yawTimer.getTime() <= 5){
        pm_.setSetpoint(AXIS_YAW, INPUT_CAM_BTM, 0);
      }
      else{
        pm_.setZero(AXIS_YAW);
        pm_.setSetpoint(AXIS_YAW, INPUT_IMU_POS, 0);
        action = 3;
        ROS_INFO("Yaw alighned, locking position and moving forwards");
        break;
      }
    }

    case 3:{
      break;
    }
  }
}
