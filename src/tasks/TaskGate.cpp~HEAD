#include <auv_mission_control/TaskGate.h>


TaskGate::TaskGate(){
}


TaskGate::TaskGate(PidManager* pm, Camera* cam) : pm_(*pm), cam_(*cam){
  ROS_INFO("TASK GATE INITIALIZED");
}

TaskGate::~TaskGate(){

}

int TaskGate::execute(){

  //pm_.setPidEnabled("ALL", true);//turns on all 6 pid controllers

  while(ros::ok){ // change so it's while keep running, some value that determines whether to keep running
  ros::spinOnce();
  ROS_INFO("Hi there, welcome to the gate task");

  killSwitch = pm_.getKill();
  if(killSwitch == true){ //checks for kill switch, enters kill state if activated
    ROS_INFO("kill");
    return kill;
  }
  else{
    ROS_INFO("kill appears to be false");
  }

//  if(getTimeout()){ //checks 15 min timer, if activated signals to enter resurface state
//    return timeout;
//  }

  ROS_INFO("MOM GET THE CAMERA. I MADE IT TO HERE!!!");
    switch(action){
      case 0: { //first step, go to depth
        ROS_INFO("Vroom Vroom going do depth");
        if(depthCounter < 1 && startTimer == true){
          pm_.setZero(AXIS_YAW);
          goToDepth_time.start();
          depthCounter ++;
        }

        pm_.setSetpoint(AXIS_YAW, INPUT_IMU_POS, 0);
        pm_.setSetpoint(AXIS_HEAVE, INPUT_DEPTH, -1.25);

        if(fabs(pm_.getDepth() - -1.25) <= .05)
          startTimer = true;
        else
          startTimer = false;

        if(startTimer){
          ROS_INFO("Going to depth for T-%f more seconds", 20-goToDepth_time.getTime());
        }

        ROS_INFO("At depth of %f", pm_.getDepth());
        if(goToDepth_time.getTime() >= 3){
          action = 1;
          ROS_INFO("Done going to depth. At depth %f", pm_.getDepth());
          break;
        }
      }


      case 1: {

          ROS_INFO("Vroom Vroom going forwards");
          //if (forwardCounter < 1)
            driveForwards_time.start();
          forwardCounter++;
          while(driveForwards_time.getTime() < 20 && !killSwitch){
            killSwitch = pm_.getKill();
            //ROS_INFO("kill %d", pm_.getKill());
            pm_.setControlEffort(AXIS_SURGE, 30);
          //ROS_INFO("Driving forwards for T-%f seconds", 5-driveForwards_time.getTime());
          }
          ROS_INFO("exited while loop");
          if(killSwitch){
            return kill;
            ROS_INFO("Kill switch detected");
          }
          else{
            pm_.setControlEffort(AXIS_SURGE, 0);//STAHP IT!
            action = 2;
            ROS_INFO("I THINK that I'm through the gate.");
            return succeeded;
            break;
          }
        }

      case 2:{ //return succeeded, and please proceed to the nearest task as quickly and calmly as possible, keeping in mind that it may be behind you
          return succeeded;
          break;
      }

    }//switch
  }//while ros::ok
}//execute
