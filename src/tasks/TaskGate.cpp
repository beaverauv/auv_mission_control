<<<<<<< HEAD:src/tasks/task_gate.cpp
#include "auv_mission_control/task_gate.h"
#include "auv_mission_control/pid_manager.h"
#include <iostream>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <cmath>
=======
#include <auv_mission_control/TaskGate.h>
>>>>>>> aecae700d55f4c671ffd932ac9a8c9c3b7a29d3d:src/tasks/TaskGate.cpp


TaskGate::TaskGate(){
}


<<<<<<< HEAD:src/tasks/task_gate.cpp
Task_Gate::Task_Gate(Pid_Manager* pm, Camera* cam) : pm_(*pm), cam_(*cam){
=======
TaskGate::TaskGate(PidManager* pm, Camera* cam) : pm_(*pm), cam_(*cam){
>>>>>>> aecae700d55f4c671ffd932ac9a8c9c3b7a29d3d:src/tasks/TaskGate.cpp
  ROS_ERROR("TASK GATE INIT");
}

TaskGate::~TaskGate(){

}

int TaskGate::execute(){

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
        if(depthCounter < 1 && startTimer = true){
          pm_.zero(AXIS_YAW);
          goToDepth_time.start();
          depthCounter ++;
        }

        pm_.setSetPoint(AXIS_YAW, INPUT_IMU_POS, 0);
        pm_.setSetPoint(AXIS_HEAVE, INPUT_DEPTH, -1.25);

        if(fabs(pm_.getDepth() - -1.25) <= .05)
          startTimer = true;
        else
          startTimer = false;

        if(startTimer){
          ROS_INFO("Going to depth for T-%f more seconds", 20=goToDepth_time.getTime());
        }

        ROS_INFO("At depth of %f", )
        if(goToDepth_time.getTime() >= 3){
          action = 1;
          ROS_INFO("Done going to depth. At depth %f", pm_.getDepth());
          break;
        }
      }


      case 1: {
        ROS_INFO("Vroom Vroom going forwards");
        if (forwardCounter < 1)
          driveForwards_time.start();
        forwardCounter++;

        if(driveForwards_time.getTime() < 20){
          pm_.setControlEffort(AXIS_SURGE, 30);
          ROS_INFO("Driving forwards for T-%f seconds", 20-driveForwards.getTime());
        }
        else{
          pm_.setControlEffort(AXIS_SURGE, 0);
          action = 2;
          break;
          ROS_INFO("I THINK that I'm through the gate. My scincerest apologies if I dun goofed. But it's your fault anyways, so BUZZ OFF.")
        }
<<<<<<< HEAD:src/tasks/task_gate.cpp

        case 2:{ //return succeeded, and please proceed to the nearest task as quickly and calmly as possible, keeping in mind that it may be behind you
          return succeeded;
          break;
        }
=======
>>>>>>> aecae700d55f4c671ffd932ac9a8c9c3b7a29d3d:src/tasks/TaskGate.cpp
      }

      case 2: {//return succeeded, kill task
        return succeeded;
        break;
      }

      }//else
  }//while ros::ok
}//execute
