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

    if(pm_.getKill()){ //checks for kill switch, enters kill state if activated
      return kill;
    }
    if(getTimeout()){ //checks 15 min timer, if activated signals to enter resurface state
      return timeout;
    }

    switch(action){
      case 0: { //first step, go to depth
        ROS_INFO("Vroom Vroom going do depth");
        if(depthCounter < 1){
          pm_.zero(AXIS_YAW);
          goToDepth_time.start();
        }

        depthCounter ++;
        pm_.setSetPoint(AXIS_YAW, INPUT_IMU_POS, 0);
        pm_.setSetPoint(AXIS_HEAVE, INPUT_DEPTH, -1.25);
        if(goToDepth_time.getTime() >= 20)
          action = 1;
          ROS_INFO("Done going to depth. At depth %f", pm_.getDepth());
        break;
      }

      case 1: {
        ROS_INFO("Vroom Vroom going forwards");
        if (forwardCounter < 1)
          driveForwards_time.start();
        forwardCounter ++;
        if(driveForwards_time.getTime() < 20)
          pm_.setControlEffort(AXIS_SURGE, 30);
        else{
          pm_.setControlEffort(AXIS_SURGE, 90);
          action = 2;
          break;
        }

        case 2: //return succeeded, kill task
          return succeeded;
          break;

      }


    }//else
  }//while ros::ok
}//execute
