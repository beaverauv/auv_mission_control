#include <auv_mission_control/TaskGate.h>

TaskGate::TaskGate(){
}


TaskGate::TaskGate(PidManager* pm, Camera* cam) : pm_(*pm), cam_(*cam){
  ROS_INFO("TASK GATE INIT");
}

TaskGate::~TaskGate(){

}

int TaskGate::execute(){

  ros::Rate gateRate(20.);

  //pm_.pidEnable("ALL", true);//turns on all 6 pid controllers

  while(ros::ok){ // change so it's while keep running, some value that determines whether to keep running
  killSwitch = pm_.getKill();
  if(killSwitch){
    ROS_ERROR("Kill switch detected");
    return kill;
    break;
  }
  //  if(getTimeout()){ //checks 15 min timer, if activated signals to enter resurface state
  //    return timeout;
  //  }

    switch(action){
      case 0: { //first step, go to depth
        ros::spinOnce();

        pm_.setPidEnabled(AXIS_SWAY, false);
        pm_.setPidEnabled(AXIS_YAW, true);
        pm_.setPidEnabled(AXIS_HEAVE, true);
        pm_.setPidEnabled(AXIS_SURGE, false);

        //pm_.setControlEffort(AXIS_SWAY, 0);
        //pm_.setControlEffort(AXIS_SURGE, 0);


        killSwitch = pm_.getKill();
        if(killSwitch){
          ROS_ERROR("Kill switch detected");
          return kill;
          break;
        }

        ROS_INFO("Vroom Vroom going do depth");

        pm_.setSetpoint(AXIS_YAW, INPUT_IMU_POS, 0);
        pm_.setSetpoint(AXIS_HEAVE, INPUT_DEPTH, -1);

        while(ros::ok){
          pm_.setPlantState(AXIS_HEAVE, pm_.getYaw());

          currentDepth = pm_.getDepth();
          double error = fabs(currentDepth - -1);
          if(rosInfoCounter%20000000 == 0)
//            ROS_INFO("YAW = %f", pm_.getYaw());
          pm_.setSetpoint(AXIS_HEAVE, INPUT_DEPTH, -1);
          pm_.setPlantState(AXIS_YAW, pm_.getYaw());
          pm_.setSetpoint(AXIS_YAW, INPUT_IMU_POS, 0);
          killSwitch = pm_.getKill();
          if(killSwitch){
            ROS_ERROR("Kill switch detected");
            return kill;
            break;
          }

          startTimer = false;
          rosInfoCounter++;
          if(error < .01)
            break;
          ros::spinOnce();
          gateRate.sleep();
        }

        ROS_INFO("Near depth setoint of %f; currently at %f. Starting depth timer.", -1.25, pm_.getDepth());
        pm_.setPlantState(AXIS_HEAVE, pm_.getDepth());
        pm_.setSetpoint(AXIS_HEAVE, INPUT_DEPTH, -1);
        pm_.setPlantState(AXIS_YAW, pm_.getYaw());
        pm_.setSetpoint(AXIS_YAW, INPUT_IMU_POS, 0);

        startTimer = true;
        killSwitch = pm_.getKill();
        if(killSwitch){
          ROS_ERROR("Kill switch detected");
          return kill;
          break;
        }

        if(depthCounter < 1 && startTimer == true){
          pm_.setPlantState(AXIS_HEAVE, pm_.getDepth());
          pm_.setSetpoint(AXIS_HEAVE, INPUT_DEPTH, -1);
          pm_.setPlantState(AXIS_YAW, pm_.getYaw());
          pm_.setSetpoint(AXIS_YAW, INPUT_IMU_POS, 0);

          ROS_INFO("pm_.getDepth %f", pm_.getDepth());
          goToDepth_time.start();
          depthCounter ++;
          ROS_INFO("Depth timer started");
          killSwitch = pm_.getKill();
          if(killSwitch){
            ROS_ERROR("Kill switch detected");
            return kill;
            break;
          }
        }


        while(ros::ok && goToDepth_time.getTime() < 5){//just chill
          pm_.setPlantState(AXIS_HEAVE, pm_.getDepth());
          pm_.setSetpoint(AXIS_HEAVE, INPUT_DEPTH, -1);
          pm_.setPlantState(AXIS_YAW, pm_.getYaw());
          pm_.setSetpoint(AXIS_YAW, INPUT_IMU_POS, 0);

          killSwitch = pm_.getKill();
          if(killSwitch){
            ROS_ERROR("Kill switch detected");
            return kill;
            break;
          }          pm_.setSetpoint(AXIS_HEAVE, INPUT_DEPTH, -1);
          ros::spinOnce();
          gateRate.sleep();
        }

        ROS_INFO("Done going to depth. At depth %f", pm_.getDepth());
        action = 1;
        break;



      }


      case 1: {
        ROS_INFO("Vroom Vroom going forwards");
        if (forwardCounter < 1)
          driveForwards_time.start();
        forwardCounter++;
        pm_.setSetpoint(AXIS_HEAVE, INPUT_DEPTH, -1);
        pm_.setPlantState(AXIS_HEAVE, pm_.getDepth());
        pm_.setPlantState(AXIS_YAW, pm_.getYaw());
        pm_.setSetpoint(AXIS_YAW, INPUT_IMU_POS, 0);

        killSwitch = pm_.getKill();
        if(killSwitch){
          ROS_ERROR("Kill switch detected");
          return kill;
          break;
        }


        while(driveForwards_time.getTime() < 50){
          pm_.setPlantState(AXIS_HEAVE, pm_.getDepth());
          pm_.setSetpoint(AXIS_HEAVE, INPUT_DEPTH, -1);
          pm_.setPlantState(AXIS_YAW, pm_.getYaw());
          pm_.setSetpoint(AXIS_YAW, INPUT_IMU_POS, 0);

          pm_.setControlEffort(AXIS_SURGE, 50);
          killSwitch = pm_.getKill();
          if(killSwitch){
            ROS_ERROR("Kill switch detected");
            return kill;
            break;
          }
          ros::spinOnce();
          gateRate.sleep();
        }

        pm_.setControlEffort(AXIS_SURGE, 0);

        killSwitch = pm_.getKill();
        if(killSwitch){
          ROS_ERROR("Kill switch detected");
          return kill;
          break;
        }
        pm_.setSetpoint(AXIS_HEAVE, INPUT_DEPTH, -1);

        pm_.setPlantState(AXIS_HEAVE, pm_.getDepth());
        pm_.setPlantState(AXIS_YAW, pm_.getYaw());
        pm_.setSetpoint(AXIS_YAW, INPUT_IMU_POS, 0);

        pm_.setControlEffort(AXIS_SURGE, 0);
        ROS_INFO("I THINK that I'm through the gate");
        action = 2;
        break;

      }

      case 2:{ //return succeeded, and please proceed to the nearest task as quickly and calmly as possible, keeping in mind that it may be behind you
	if(upCount < 1)
	  upTimer.start();
	
        while(upTimer.getTime() < 5){
	  ros::spinOnce;
          gateRate.sleep();
	  pm_.setPidEnabled(AXIS_HEAVE, 0);
	  pm_.setControlEffort(AXIS_HEAVE, 50);	  
}     
   pm_.setControlEffort(AXIS_HEAVE, 0);
   return succeeded;
        break;
      
     }
    };

    ros::spinOnce();
    gateRate.sleep();

  }//while ros::ok
}//execute

