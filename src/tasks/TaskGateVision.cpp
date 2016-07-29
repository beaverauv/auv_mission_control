#include <auv_mission_control/TaskGateVision.h>

TaskGateVision::TaskGateVision(){
}


TaskGateVision::TaskGateVision(PidManager* pm, Camera* cam) : pm_(*pm), cam_(*cam){
}

TaskGateVision::~TaskGateVision(){

}

int TaskGateVision::execute(){

  // pm_.setCamera(INPUT_CAM_FRONT);
  pm_.setZero(AXIS_YAW);
  pm_.setSetpoint(AXIS_YAW, INPUT_IMU_POS, 0);
  pm_.setSetpoint(AXIS_HEAVE, INPUT_DEPTH, -1.25);

  while(ros::ok){ // change so it's while keep running, some value that determines whether to keep running

  ros::spinOnce();

  pm_.setSetpoint(AXIS_YAW, INPUT_IMU_POS, 0);

  cam_.updateFront();
  cv::Mat original;
  cv::Mat imgThresh;

  original = cam_.getFront();
  if (currentColor == COLOR_RED){
    cv::inRange(original, redMin, redMax, imgThresh);
  } else if (currentColor = COLOR_GREEN) {
    cv::inRange(original, greenMin, greenMax, imgThresh);
  }

//  cv::Mat imgHSV = original;

  cv::dilate(imgThresh, imgThresh, cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(5,5)));
  cv::erode(imgThresh, imgThresh, cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(5,5)));

  cv::erode(imgThresh, imgThresh, cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(5,5)));
  cv::dilate(imgThresh, imgThresh, cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(5,5)));

  cv::dilate(imgThresh, imgThresh, cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(10,10)));

  cv::Moments oMoments = cv::moments(imgThresh);

  double posX = oMoments.m10 / oMoments.m00;
  double posY = oMoments.m01 / oMoments.m00;
  double posXcorrected = 640 - posX;
  double posYcorrected = posY; //here for continuity

  // minR = 0;
  // maxR = 0;
  // minG = 0;
  // maxG = 0;
  // minB = 0;
  // maxB = 0;


  killSwitch = pm_.getKill();
  if(killSwitch){
    ROS_ERROR("Kill switch detected");
    return kill;
    break;
  }

  // if(getTimeout()){
  //   return timeout;
  // }


  switch(action){

    case 0: { //Go To Depth
      ros::spinOnce();

      pm_.setPidEnabled(AXIS_SWAY, false);
      pm_.setPidEnabled(AXIS_YAW, true);
      pm_.setPidEnabled(AXIS_HEAVE, true);
      pm_.setPidEnabled(AXIS_SURGE, false);

      pm_.setControlEffort(AXIS_SWAY, 0);
      pm_.setControlEffort(AXIS_SURGE, 0);

      killSwitch = pm_.getKill();
      if(killSwitch){
        ROS_ERROR("Kill switch detected");
        return kill;
        break;
      }

      ROS_INFO("Vroom Vroom going do depth");

      pm_.setSetpoint(AXIS_YAW, INPUT_IMU_POS, 0);
      pm_.setSetpoint(AXIS_HEAVE, INPUT_DEPTH, -1.25);

      while(ros::ok){
        currentDepth = pm_.getDepth();
        double error = fabs(currentDepth - -1.25);
      //  if(rosInfoCounter%20000000 == 0)

        killSwitch = pm_.getKill();
        if(killSwitch){
          ROS_ERROR("Kill switch detected");
          return kill;
          break;
        }

        startTimer = false;
        ros::spinOnce();
        rosInfoCounter++;
        if(error < .01)
          break;
      }

      ROS_INFO("Near depth setoint of %f; currently at %f. Starting depth timer.", -1.25, pm_.getDepth());

      startTimer = true;
      killSwitch = pm_.getKill();
      if(killSwitch){
        ROS_ERROR("Kill switch detected");
        return kill;
        break;
      }

      if(depthCounter < 1 && startTimer == true){
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
        killSwitch = pm_.getKill();
        if(killSwitch){
          ROS_ERROR("Kill switch detected");
          return kill;
          break;
        }
        pm_.setSetpoint(AXIS_HEAVE, INPUT_DEPTH, -1.25);
        ros::spinOnce();
      }

      ROS_INFO("Done going to depth. At depth %f", pm_.getDepth());
      action = 1;
      break;

    }

    case 1:{//start going forward towards gate using vision, check if jumped too forward

      killSwitch = pm_.getKill();
      if(killSwitch){
        ROS_ERROR("Kill switch detected");
        return kill;
        break;
      }

      camInUse = INPUT_CAM_FRONT;
      pm_.setPidEnabled(AXIS_SWAY, true);
      pm_.setPidEnabled(AXIS_YAW, true);
      pm_.setPidEnabled(AXIS_HEAVE, true);
      pm_.setPidEnabled(AXIS_SURGE, false);

      pm_.setControlEffort(AXIS_SURGE, 0);
      if(goCounter < 1){
        forwardTimer.start() ;
        goCounter++;
        prevPosX = posXcorrected;
        prevPosY = posYcorrected;
        ROS_INFO("Using vision to navigate to gate");
      }


      while(ros::ok && forwardTimer.getTime() < 30){

        ros::spinOnce();

        killSwitch = pm_.getKill();
        if(killSwitch){
          ROS_ERROR("Kill switch detected");
          return kill;
          break;
        }

        if(fabs(prevPosX - posXcorrected) > maxJump || fabs(prevPosY - posYcorrected) > maxJump){ //checks if vision jumped too far
          ROS_WARN("Jump in position of centroid of gate exceeds max jump. Switching to dead reckoning");
          action = 2;
          break;
        }

        pm_.setSetpoint(AXIS_YAW, INPUT_CAM_FRONT, 320);
        pm_.setSetpoint(AXIS_SWAY, INPUT_CAM_FRONT, 320);
        pm_.setSetpoint(AXIS_HEAVE, INPUT_DEPTH, -1.25);
        pm_.setControlEffort(AXIS_SURGE, 25);//manually set to drive forward at speed 25
      }
      ROS_WARN("Max time for vision tracing forwards exceeded. Timeout called. Resurfacing :(");
      action = 2;
      return timeout;
      break;
    }

    case 2:{ //dead reckon rest of way, looking for bottom
      if(counter < 1){ //setZero the yaw, but only once.
        pm_.setZero(IMU_YAW);
        counter++; //please don't setZero the yaw again, thanks.
        dedReckonTimer.start();
      }

      camInUse = INPUT_CAM_BTM;
      pm_.setPidEnabled(AXIS_SWAY, false);
      pm_.setPidEnabled(AXIS_YAW, true);
      pm_.setPidEnabled(AXIS_HEAVE, true);
      pm_.setPidEnabled(AXIS_SURGE, false);
      pm_.setSetpoint(AXIS_HEAVE, INPUT_DEPTH, -1.25);

      while(ros::ok && dedReckonTimer.getTime() < 30){
        ros::spinOnce();
        if(objectFound){//if found, move on
          ROS_INFO("Path marker located. Navigating forwards to it.");
          action = 3;
          break;
        }
        pm_.setSetpoint(AXIS_YAW, INPUT_IMU_POS, 0); //otherwise ded reckon forwards
        pm_.setControlEffort(AXIS_SURGE, 35);
      }
      ROS_WARN("Max time for ded reckoning forwards exceeded. Timeout called. Resurfacing" );
      action = 3;
      return timeout;
      break;
    }

    case 3:{
        ros::spinOnce();
        ROS_INFO("Going towards path marker.");
        pm_.setPidEnabled(AXIS_SURGE, true);

        while(true){
          ros::spinOnce();

          if(finalTimer.getTime() >= 10){
            return timeout;
          }

        pm_.setSetpoint(AXIS_YAW, INPUT_IMU_POS, 0);
        pm_.setSetpoint(AXIS_SURGE, INPUT_CAM_BTM, 240);

        if(fabs(240 - posYcorrected) <= 10){
          if(finalCounter < 1){
            finalTimer.start();
            finalCounter ++;
          }

        if(finalTimer.getTime() >= 3){
          ROS_INFO("All done. Hopefully I'm in the right place. Sorry if I'm not, I understand that could be highly frustrating. Congrats on the good work if I am. Really well done, champ.");//What's a robot without an attitude?
          action = 3;
          break;
        }
      };
    }

      return succeeded;

    }

    default:
      break;

  }//switch
  }//while ros::ok
}//execute
