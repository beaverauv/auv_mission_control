#include "auv_mission_control/task_gate_vision.h"
#include <cmath>

Task_Gate_Vision::Task_Gate_Vision(){
}


Task_Gate_Vision::Task_Gate_Vision(Pid_Manager* pm, Camera* cam) : pm_(*pm), cam_(*cam){
}

Task_Gate_Vision::~Task_Gate_Vision(){

}

int Task_Gate_Vision::execute(){

  pm_.setCamera(INPUT_CAM_FRONT);

  while(ros::ok){ // change so it's while keep running, some value that determines whether to keep running

  ros::spinOnce();
  if(pm_.getKill()){
    return kill;
  }
  if(getTimeout()){
    return timeout;
  }


  switch(action){

    case 0: { //Go To Depth
      pm_.pidEnable(AXIS_SWAY, true);
      pm_.pidEnable(AXIS_YAW, true);
      pm_.pidEnable(AXIS_HEAVE, true);
      pm_.pidEnable(AXIS_SURGE, false);
      depthTimer.start();

      if(depthTimer.getTime() <= 8){
        pm_.setpoint_set(AXIS_YAW, INPUT_CAM_FRONT, 320);
        pm_.setpoint_set(AXIS_SWAY, INPUT_CAM_FRONT, 320);
        pm_.setpoint_set(AXIS_HEAVE, INPUT_DEPTH, -1.25);
        ROS_INFO("Going to depth for %f more seconds. At depth of %f", (8.0 - depthTimer.getTime()), pm_.getDepth());
      }

      else{
        ROS_INFO("Moving on, at depth of %f", pm_.getDepth());
        action = 1;
        break;
      }

    }

    case 1:{//start going forward, use vision for 10 sec
      pm_.pidEnable(AXIS_SWAY, true);
      pm_.pidEnable(AXIS_YAW, true);
      pm_.pidEnable(AXIS_HEAVE, true);
      pm_.pidEnable(AXIS_SURGE, false);

      forwardTimer.start();
      if(forwardTimer.getTime() <= 10){
        ROS_INFO("Moving forward with vision for %f more seconds", (10 - forwardTimer.getTime()) );
        pm_.setpoint_set(AXIS_YAW, INPUT_CAM_FRONT, 320);
        pm_.setpoint_set(AXIS_SWAY, INPUT_CAM_FRONT, 320);
        pm_.setpoint_set(AXIS_HEAVE, INPUT_DEPTH, -1.25);
        pm_.controlEffort_set(AXIS_SURGE, 25);//manually set to drive forward at speed 25
      }
      else{
        ROS_INFO("Done using vision, dead reackoning rest of way");
        action = 2;
        break;
      }
    }

    case 2:{ //dead reckon rest of way, looking for bottom
      if(counter < 1){ //zero the yaw, but only once.
        pm_.zero(IMU_YAW);
      }

      pm_.setCamera(INPUT_CAM_BTM);
      pm_.pidEnable(AXIS_SWAY, false);
      pm_.pidEnable(AXIS_YAW, true);
      pm_.pidEnable(AXIS_HEAVE, true);
      pm_.pidEnable(AXIS_SURGE, false);
      pm_.setpoint_set(AXIS_HEAVE, INPUT_DEPTH, -1.25);

      if(!objectFound){//if ya don't see anything on the floor, just go forward
        ROS_INFO("No path marker found");
        pm_.setpoint_set(AXIS_YAW, INPUT_IMU_POS, 0);
        pm_.controlEffort_set(AXIS_SURGE, 25);
      }
      else{
        ROS_INFO("Bottom path marker found. I'm chasing it. Vroom.");
        pm_.pidEnable(AXIS_SURGE, true);
        pm_.setpoint_set(AXIS_YAW, INPUT_IMU_POS, 0);
        pm_.setpoint_set(AXIS_SURGE, INPUT_CAM_BTM, 240);
        if(fabs(plantState_surge - setpoint_surge) <= 20){
          finalTimer.start();
          if(finalTimer.getTime() >= 3){
            ROS_INFO("All done. Hopefully I'm in the right place. Sorry if I'm not, I understand that could be highly frustrating. Congrats on the good work if I am. Really well done, champ.");//What's a robot without an attitude?
            action = 3;
            break;
          }

        }
      }

      counter++; //please don't zero the yaw again, thanks.
    }

    case 3:
      return succeeded;


  }//switch
  }//while ros::ok
}//execute
