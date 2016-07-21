#include "auv_mission_control/task_gate_vision.h"
#include <opencv2/imgproc/imgproc.hpp>
#include <cmath>

Task_Gate_Vision::Task_Gate_Vision(){
}


Task_Gate_Vision::Task_Gate_Vision(Pid_Manager* pm, Camera* cam) : pm_(*pm), cam_(*cam){
}

Task_Gate_Vision::~Task_Gate_Vision(){

}

int Task_Gate_Vision::execute(){

  // pm_.setCamera(INPUT_CAM_FRONT);
  pm_.zero(AXIS_YAW);
  pm_.setSetPoint(AXIS_YAW, INPUT_IMU_POS, 0);
  pm_.setSetPoint(AXIS_HEAVE, INPUT_DEPTH, -1.25);

  cv::namedWindow("Original", CV_WINDOW_AUTOSIZE);
  cv::namedWindow("controlGate", CV_WINDOW_AUTOSIZE);

  cv::createTrackbar("minR", "controlGate", &minR, 255);
  cv::createTrackbar("maxR", "controlGate", &maxR, 255);
  cv::createTrackbar("minG", "controlGate", &minG, 255);
  cv::createTrackbar("maxG", "controlGate", &maxG, 255);
  cv::createTrackbar("minB", "controlGate", &minB, 255);
  cv::createTrackbar("maxB", "controlGate", &maxB, 255);


  while(ros::ok){ // change so it's while keep running, some value that determines whether to keep running

  ros::spinOnce();

  pm_.setSetPoint(AXIS_YAW, INPUT_IMU_POS, 0);

  cam_.update();

  cv::Mat original;

  if (camInUse == INPUT_CAM_FRONT){
    original = cam_.getFront();
  }
  else{
    original = cam_.getBottom();
  }
//  cv::Mat imgHSV = original;
  cv::Mat imgThresh;

  cv::inRange(original, cv::Scalar(minR, minG, minB), cv::Scalar(maxR, maxG, maxB), imgThresh);
  cv::erode(imgThresh, imgThresh, cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(5,5)));
  cv::dilate(imgThresh, imgThresh, cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(10,10)));

  cv::Moments oMoments = cv::moments(imgThresh);

  int dM01 = oMoments.m01;
  int dM10 = oMoments.m10;
  double dArea = oMoments.m00;
  int posX = dM10 / dArea;
  int posY = dM01 / dArea;
  double posXdouble = (double)posX;
  double posYdouble = (double)posY;
  double posXcorrected = 640 - posXdouble;

  // minR = 0;
  // maxR = 0;
  // minG = 0;
  // maxG = 0;
  // minB = 0;
  // maxB = 0;


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
      if(depthCounter < 0){
        depthTimer.start();
        depthCounter++;
      }
      if(depthTimer.getTime() <= 8){
        pm_.setSetPoint(AXIS_YAW, INPUT_CAM_FRONT, 320);
        pm_.setSetPoint(AXIS_SWAY, INPUT_CAM_FRONT, 320);
        pm_.setSetPoint(AXIS_HEAVE, INPUT_DEPTH, -1.25);
        ROS_INFO("Going to depth for %f more seconds. At depth of %f", (8.0 - depthTimer.getTime()), pm_.getDepth());
      }

      else{
        ROS_INFO("Moving on, at depth of %f", pm_.getDepth());
        action = 1;
        break;
      }

    }

    case 1:{//start going forward, use vision for 10 sec
      camInUse = INPUT_CAM_FRONT;
      pm_.pidEnable(AXIS_SWAY, true);
      pm_.pidEnable(AXIS_YAW, true);
      pm_.pidEnable(AXIS_HEAVE, true);
      pm_.pidEnable(AXIS_SURGE, false);

      if(goCounter < 1){
        forwardTimer.start() ;
        goCounter++;
      }
      if(forwardTimer.getTime() <= 10){
        ROS_INFO("Moving forward with vision for %f more seconds", (10 - forwardTimer.getTime()) );
        pm_.setSetPoint(AXIS_YAW, INPUT_CAM_FRONT, 320);
        pm_.setSetPoint(AXIS_SWAY, INPUT_CAM_FRONT, 320);
        pm_.setSetPoint(AXIS_HEAVE, INPUT_DEPTH, -1.25);
        pm_.setControlEffort(AXIS_SURGE, 25);//manually set to drive forward at speed 25
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

      camInUse = INPUT_CAM_BTM;
      pm_.pidEnable(AXIS_SWAY, false);
      pm_.pidEnable(AXIS_YAW, true);
      pm_.pidEnable(AXIS_HEAVE, true);
      pm_.pidEnable(AXIS_SURGE, false);
      pm_.setSetPoint(AXIS_HEAVE, INPUT_DEPTH, -1.25);

      if(!objectFound){//if ya don't see anything on the floor, just go forward
        ROS_INFO("No path marker found");
        pm_.setSetPoint(AXIS_YAW, INPUT_IMU_POS, 0);
        pm_.setControlEffort(AXIS_SURGE, 25);
      }
      else{
        ROS_INFO("Bottom path marker found. I'm chasing it. Vroom.");
        pm_.pidEnable(AXIS_SURGE, true);
        pm_.setSetPoint(AXIS_YAW, INPUT_IMU_POS, 0);
        pm_.setSetPoint(AXIS_SURGE, INPUT_CAM_BTM, 240);
        if(fabs(posYdouble - setpoint_surge) <= 20){
          if(finalCounter < 1){
          finalTimer.start();
          finalCounter += 102;
        }
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

    default:
      break;

  }//switch
  }//while ros::ok
}//execute
