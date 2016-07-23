#include <auv_mission_control/TaskGate.h>


TaskGate::TaskGate(){
}


TaskGate::TaskGate(PidManager* pm, Camera* cam) : pm_(*pm), cam_(*cam){
  ROS_ERROR("TASK GATE INIT");
}

TaskGate::~TaskGate(){

}

int TaskGate::execute(){

  //pm_.setPidEnabled("ALL", true);//turns on all 6 pid controllers

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
          pm_.setZero(AXIS_YAW);
          goToDepth_time.start();
        }

        depthCounter ++;
        pm_.setSetpoint(AXIS_YAW, INPUT_IMU_POS, 0);
        pm_.setSetpoint(AXIS_HEAVE, INPUT_DEPTH, -1.25);
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
      }

      case 2: {//return succeeded, kill task
        return succeeded;
        break;
      }

    }//else
  }//while ros::ok
}//execute
