#include "auv_mission_control/task_gate.h"
#include "auv_mission_control/pid_manager.h"
#include <cmath>

double surgeSpeed = 40;
double previousDepth;
double distanceFromEdge_left;
double distanceFromEdge_right;
bool outOfSight;
double plantState_sway;
double plantState_heave;
double setpoint_sway;
double setpoint_heave;


Task_Gate::Task_Gate(Pid_Manager* pm){
  this->pm = pm;
}

Task_Gate::~Task_Gate(){
  
}

void Task_Gate::execute(){

  pm->onlyPID_set(true);
  //pm->pidEnable("ALL", true);//turns on all 6 pid controllers
  bool completed = false;


  while(true){ // change so it's while keep running, some value that determines whether to keep running

    pm->setpoint_set(AXIS_ROLL, INPUT_IMU_POS, 0); //roll pitch and yaw always at 0, straight ahead
    pm->setpoint_set(AXIS_PITCH, INPUT_IMU_POS, 0);
    pm->setpoint_set(AXIS_YAW, INPUT_IMU_POS, 0);

    //Create a bounding box around the entire contours. Find the distance from the edges of each to the edges of the screen.

      if (distanceFromEdge_right < 20 && distanceFromEdge_left < 20){//some reasonable deadband
        outOfSight = true;
      }
      else{
        outOfSight = false;
      }


      if(!outOfSight){


        if (fabs(plantState_sway - setpoint_sway) > 20 || fabs(plantState_heave - setpoint_heave) > 20){//SOME REASONABLE DEADBAND
          pm->setpoint_set(AXIS_SURGE, INPUT_IMU_VEL, 0); //makes it so the robot doesn't try to move forward if the sway and heave are outside of a DEADBAND
        }

        else{
          pm->setpoint_set(AXIS_SURGE, INPUT_IMU_VEL, surgeSpeed); //random number for now, will need to be replaced by testing. Will probably be a percent, but idk
        }

        pm->setpoint_set(AXIS_HEAVE, INPUT_CAM_FRONT, 0);
        pm->setpoint_set(AXIS_SWAY, INPUT_CAM_FRONT, 0);

        previousDepth = pm->getDepth();

      }

        else{ // if out of sight...

          pm->zero(AXIS_SURGE);
          pm->setpoint_set(AXIS_HEAVE, INPUT_DEPTH, previousDepth);
          pm->setpoint_set(AXIS_SURGE, INPUT_IMU_VEL, 5); //go forward 5 meters to get through gate
          pm->setpoint_set(AXIS_SWAY, INPUT_IMU_VEL, 0);
        }

    }

}
