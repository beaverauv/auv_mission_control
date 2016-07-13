#include "task_gate.h"
#include "pid_manager.h"

double surgeSpeed = 40;
double previousDepth;

Task_Gate::Task_Gate(pid_manager* pm){
  this->pm = pm;
}

void task_buoy::execute(){

  pm.onlyPID_set(true);
  pm.pidEnable("ALL", true);//turns on all 6 pid controllers
  bool completed = false;


  while(true){ // change so it's while keep running, some value that determines whether to keep running

    pm.setpoint_set("ROLL", "IMU_POS", 0); //roll pitch and yaw always at 0, straight ahead
    pm.setpoint_set("PITCH", "IMU_POS", 0);
    pm.setpoint_set("YAW", "IMU_POS", 0);

    //Create a bounding box around the entire contours. Find the distance from the edges of each to the edges of the screen.

      if (distanceFromEdge_right < 20 && distanceFromEdge_left < 20){//some reasonable deadband
        outOfSight = true;
      }
      else{
        outOfSight = false;
      }


      if(!outOfSight){


        if (abs(plantState_sway - setpoint_sway) > 20 || abs(plantState_heave - setpoint_heave) > 20){//SOME REASONABLE DEADBAND
          pm.setpoint_set("SURGE", "IMU_VEL", 0); //makes it so the robot doesn't try to move forward if the sway and heave are outside of a DEADBAND
        }

        else{
          pm.setpoint_set("SURGE", "IMU_VEL", surgeSpeed); //random number for now, will need to be replaced by testing. Will probably be a percent, but idk
        }

        pm.setpoint_set("HEAVE", "CAM_FRONT", 0);
        pm.setpoint_set("SWAY", "CAM_FRONT", 0);

        previousDepth = pm.getDepth();

      }

        else{ // if out of sight...

          zero("IMU_SURGE");
          pm.setpoint_set("HEAVE", "DEPTH_SENSOR", previousDepth);
          pm.setpoint_set("SURGE", "IMU_POS", 5); //go forward 5 meters to get through gate
          pm.setpoint_set("SWAY", "IMU_VEL", 0);
        }

    }

}
