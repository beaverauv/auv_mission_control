#include <auv_mission_control/TaskGate.h>

double thisDepth = -0.6; // make -2
double markerErrorY;
double markerErrorX;
double offsetAngle;

TaskGate::TaskGate(){
}


TaskGate::TaskGate(PidManager* pm, Camera* cam, TaskVision* vision) : pm_(*pm), cam_(*cam), vision_(*vision){
  ROS_INFO("TASK GATE INIT");
}

TaskGate::~TaskGate(){

}

int TaskGate::execute(){
   ROS_INFO("TASK GATE EXECUTE");
  ros::Rate gateRate(20.);

  //pm_.pidEnable("ALL", true);//turns on all 6 pid controllers

  while(ros::ok){ // change so it's while keep running, some value that determines whether to keep running
  killSwitch = pm_.getKill();
  vision_.findMarker();
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
        pm_.setZero(AXIS_YAW);
        pm_.setZero(AXIS_YAW);
        pm_.setZero(AXIS_YAW);
        pm_.setZero(AXIS_YAW);
        pm_.setZero(AXIS_YAW);
        pm_.setZero(AXIS_YAW);
        pm_.setZero(AXIS_YAW);
        pm_.setZero(AXIS_YAW);


        killSwitch = pm_.getKill();
        if(killSwitch){
          ROS_ERROR("Kill switch detected");
          return kill;
          break;
        }

        ROS_INFO("Vroom Vroom going do depth");

        pm_.setSetpoint(AXIS_YAW, INPUT_IMU_POS, 0);
        pm_.setSetpoint(AXIS_HEAVE, INPUT_DEPTH, thisDepth);
	pm_.setPlantState(AXIS_HEAVE, pm_.getDepth());
	
	
        while(ros::ok){
          pm_.setPlantState(AXIS_HEAVE, pm_.getYaw());

          currentDepth = pm_.getDepth();
          double error = fabs(currentDepth - thisDepth);
          if(rosInfoCounter%20000000 == 0)
//            ROS_INFO("YAW = %f", pm_.getYaw());
          pm_.setSetpoint(AXIS_HEAVE, INPUT_DEPTH, thisDepth);
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
        pm_.setSetpoint(AXIS_HEAVE, INPUT_DEPTH, thisDepth);
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
          pm_.setSetpoint(AXIS_HEAVE, INPUT_DEPTH, thisDepth);
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
          pm_.setSetpoint(AXIS_HEAVE, INPUT_DEPTH, thisDepth);
          pm_.setPlantState(AXIS_YAW, pm_.getYaw());
          pm_.setSetpoint(AXIS_YAW, INPUT_IMU_POS, 0);

          killSwitch = pm_.getKill();
          if(killSwitch){
            ROS_ERROR("Kill switch detected");
            return kill;
            break;
          }
          pm_.setSetpoint(AXIS_HEAVE, INPUT_DEPTH, thisDepth);
          ros::spinOnce();
          gateRate.sleep();
        }

        ROS_INFO("Done going to depth. At depth %f", pm_.getDepth());
        action = 1;
        break;



      }


      case 1: {
        ROS_INFO("YAAARRRGGGG AVAST YE I BE MOVIN FORWARDS");
        if (forwardCounter < 1)
          driveForwards_time.start();
        forwardCounter++;
        pm_.setSetpoint(AXIS_HEAVE, INPUT_DEPTH, thisDepth);
        pm_.setPlantState(AXIS_HEAVE, pm_.getDepth());
        pm_.setPlantState(AXIS_YAW, pm_.getYaw());
        pm_.setSetpoint(AXIS_YAW, INPUT_IMU_POS, 0);

        killSwitch = pm_.getKill();
        if(killSwitch){
          ROS_ERROR("Kill switch detected");
          return kill;
          break;
        }


          pm_.setPlantState(AXIS_HEAVE, pm_.getDepth());
          pm_.setSetpoint(AXIS_HEAVE, INPUT_DEPTH, thisDepth);
          pm_.setPlantState(AXIS_YAW, pm_.getYaw());
          pm_.setSetpoint(AXIS_YAW, INPUT_IMU_POS, 0);

          killSwitch = pm_.getKill();
          if(killSwitch){
            ROS_ERROR("Kill switch detected");
            return kill;
            break;
          }

//          vision_.findMarker();
  ROS_INFO("found marker");

          double markerArea = vision_.getMarkerArea();
  ROS_INFO("got area");
          
if(markerArea > 6000000){
            pm_.setPidEnabled(AXIS_SURGE, true);
            pm_.setPlantState(AXIS_SURGE, vision_.getMarkerCoordY());
            pm_.setSetpoint(AXIS_SURGE, INPUT_CAM_BTM, 240);
            markerErrorY = fabs(240 - vision_.getMarkerCoordY());
            if(markerErrorY < 10){
              ROS_INFO("YAAARRRGGGG I THINK ME SURGE BE OVER TH' MARKER");
              action = 2;
              break;
            }
          }
          else
            pm_.setControlEffort(AXIS_SURGE, 60);

          pm_.setSetpoint(AXIS_HEAVE, INPUT_DEPTH, thisDepth);
          pm_.setPlantState(AXIS_HEAVE, pm_.getDepth());
          pm_.setPlantState(AXIS_YAW, pm_.getYaw());
          pm_.setSetpoint(AXIS_YAW, INPUT_IMU_POS, 0);

          ros::spinOnce();
          gateRate.sleep();
          action = 1;
          break;//
      }//end case 1

      case 2:{ //return succeeded, and please proceed to the nearest task as quickly and calmly as possible, keeping in mind that it may be behind you
        if(reZeroCounter < 1)
          pm_.setZero(AXIS_YAW);
        pm_.setPlantState(AXIS_YAW, pm_.getYaw());
        pm_.setSetpoint(AXIS_YAW, INPUT_IMU_POS, 0);
        pm_.setSetpoint(AXIS_HEAVE, INPUT_DEPTH, -2.75);
        pm_.setPlantState(AXIS_HEAVE, pm_.getDepth());
        pm_.setPidEnabled(AXIS_SURGE, true);
        pm_.setPlantState(AXIS_SURGE, vision_.getMarkerCoordY());
        pm_.setSetpoint(AXIS_SURGE, INPUT_CAM_BTM, 240);
        pm_.setPidEnabled(AXIS_SWAY, true);
        pm_.setPlantState(AXIS_SWAY, vision_.getMarkerCoordX());
        pm_.setSetpoint(AXIS_SWAY, INPUT_CAM_BTM, 360);
        markerErrorX = fabs(360 - vision_.getMarkerCoordX());
        markerErrorY = fabs(240 - vision_.getMarkerCoordY());
        if(markerErrorX < 20 && markerErrorY < 20){
          if(markerCounter < 1)
            markerTimer.start();
          while(markerTimer.getTime() < 3)
            vision_.findMarker();
          offsetAngle = vision_.getMarkerAngle();
          ROS_INFO("YAAARRRGGGG I FOUND TH' ANGLE TO BE %f DEGREES.", offsetAngle);
          action = 3;
        }
        else
          action = 2;
        break;

      }

      case 3:{
        ROS_INFO("YAAARRRGGGG I BE ROTATING TO TH' MARKER");
        pm_.setSetpoint(AXIS_HEAVE, INPUT_DEPTH, -2.75);
        pm_.setPlantState(AXIS_HEAVE, pm_.getDepth());
        pm_.setPlantState(AXIS_SURGE, vision_.getMarkerCoordY());
        pm_.setSetpoint(AXIS_SURGE, INPUT_CAM_BTM, 240);
        pm_.setPlantState(AXIS_SWAY, vision_.getMarkerCoordX());
        pm_.setSetpoint(AXIS_SWAY, INPUT_CAM_BTM, 360);
        pm_.setSetpoint(AXIS_YAW, INPUT_CAM_BTM, offsetAngle);
        pm_.setPlantState(AXIS_YAW, vision_.getMarkerAngle());
        pm_.setSetpoint(AXIS_YAW, INPUT_IMU_POS, offsetAngle);
        pm_.setPlantState(AXIS_YAW, pm_.getYaw());
        if(fabs(pm_.getYaw() - offsetAngle) < 1){
          ROS_INFO("YAAARRRGGGG I MATCHED TH' YAW AND BE HEADING TO SCUTTLE TH' SHIP");
          pm_.setZero(AXIS_YAW);
          return succeeded;
        }
        else
          action = 3;
        break;

      }
     }
     ros::spinOnce();
     gateRate.sleep();
    }
}//execute
