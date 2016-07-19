#include "auv_mission_control/task_gate.h"
#include "auv_mission_control/pid_manager.h"
#include <iostream>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <cmath>


Task_Gate::Task_Gate(){
}


Task_Gate::Task_Gate(Pid_Manager* pm, Camera* cam) : pm_(*pm), cam_(*cam){
  ROS_ERROR("TASK GATE INIT");
}

Task_Gate::~Task_Gate(){

}

int Task_Gate::execute(){

  //pm_.pidEnable("ALL", true);//turns on all 6 pid controllers

  while(ros::ok){ // change so it's while keep running, some value that determines whether to keep running
  ros::spinOnce();

    if(pm_.getKill()){
      return kill;
    }
    if(getTimeout()){
      return timeout;
    }

    switch(action){
      case 0: {
        ROS_INFO("Vroom Vroom going do depth");
        pm_.zero(AXIS_YAW);
        pm_.setpoint_set(AXIS_YAW, INPUT_IMU_POS, 0);
        pm_.setpoint_set(AXIS_HEAVE, INPUT_DEPTH, -1.25);
        goToDepth_time.start();
        if(goToDepth_time.getTime() >= 20)
          action = 1;
          ROS_INFO("Done going to depth. At depth %f", pm_.getDepth());
        break;
      }

      case 1: {
        ROS_INFO("Vroom Vroom going forwards");
        driveForwards_time.start();
        if(driveForwards_time.getTime() < 20)
          pm_.controlEffort_set(AXIS_SURGE, 30);
        else{
          pm_.controlEffort_set(AXIS_SURGE, 90);
          action = 2;
          break;
        }

        case 2:
          return succeeded;
          break;

      }


    }//else
  }//while ros::ok
}//execute
