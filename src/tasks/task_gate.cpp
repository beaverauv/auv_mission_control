#include "auv_mission_control/task_gate.h"
#include "auv_mission_control/pid_manager.h"
#include <cmath>

Task_Gate::Task_Gate(){
}


Task_Gate::Task_Gate(Pid_Manager* pm, CameraManager* cam) : pm_(*pm), cam_(*cam){
}

Task_Gate::~Task_Gate(){

}

int Task_Gate::execute(){

  //pm_.pidEnable("ALL", true);//turns on all 6 pid controllers


  pm_.setCamera(INPUT_CAM_FRONT);
  pm_.zero(AXIS_YAW);
  pm_.setpoint_set(AXIS_YAW, INPUT_IMU_POS, 0);
  pm_.setpoint_set(AXIS_HEAVE, INPUT_DEPTH, -1.25);
  pm_.taskDelay(5);

  while(ros::ok){ // change so it's while keep running, some value that determines whether to keep running
  ros::spinOnce();


    if(pm_.getKill()){
      return kill;
    }
    if(getTimeout()){
      return timeout;
    }
    pm_.setpoint_set(AXIS_YAW, INPUT_IMU_POS, 0);

    if (distanceFromEdge_right < 20 && distanceFromEdge_left < 20){//some reasonable deadband
      outOfSight = true;
    }
    else{            ///probs dont want incase state of checked condition changes...
      outOfSight = false;
    }


    if(!outOfSight){


      if (fabs(plantState_sway - setpoint_sway) > 30){//SOME REASONABLE DEADBAND
        pm_.setpoint_set(AXIS_SURGE, INPUT_IMU_VEL, 0); //makes it so the robot doesn't try to move forward if the sway and heave are outside of a DEADBAND
      }

      else{
        pm_.setpoint_set(AXIS_SURGE, INPUT_IMU_VEL, surgeSpeed); //random number for now, will need to be replaced by testing. Will probably be a percent, but idk
      }

      pm_.setpoint_set(AXIS_SWAY, INPUT_CAM_FRONT, 0);

    }

    else{ // if out of sight...


      pm_.setCamera(INPUT_CAM_BTM);
      pm_.pidEnable(AXIS_SWAY, false);
      pm_.pidEnable(AXIS_SURGE, false);//disable PID

      if (true){//check if there is a centroid tracked.... if there is, go to. It will be path marker
        pm_.pidEnable(AXIS_SURGE, true);
        pm_.setpoint_set(AXIS_SURGE, INPUT_CAM_BTM, 240); //set to go straight until centroid is in middle of bottom cam
        if(fabs(plantState_surge - setpoint_surge) < 10){ //if it's pretty close to centered
          sleep(5); //give it some time to finish up
          return succeeded; //yay
        }
      }
      else{
        pm_.controlEffort_set(30);
      }

    }//else
  }//while ros::ok
}//execute
