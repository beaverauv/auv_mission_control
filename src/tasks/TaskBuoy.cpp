#include <auv_mission_control/TaskBuoy.h>

TaskBuoy::TaskBuoy(){
}


TaskBuoy::TaskBuoy(PidManager* pm, Camera* cam) : pm_(*pm), cam_(*cam){
        ROS_ERROR("TASK GATE INIT");
}

TaskBuoy::~TaskBuoy(){

}


int TaskBuoy::execute(){
        //pm_.setPidEnabled("ALL", true);//turns on all 6 pid controllers

        pm_.setZero(AXIS_YAW);
        pm_.setSetpoint(AXIS_YAW, INPUT_IMU_POS, 0);
        pm_.setSetpoint(AXIS_HEAVE, INPUT_DEPTH, -1.25);
        // pm_.taskDelay(5);

        while (ros::ok) {

                ros::spinOnce();

                pm_.setSetpoint(AXIS_YAW, INPUT_IMU_POS, 0);

                cam_.update();
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
                double dArea = oMoments.m00;
                double posX = oMoments.m10 / dArea;
                double posY = oMoments.m01 / dArea;
                double posXcorrected = 640 - posX;
                double posYcorrected = posY; //here for continuity

                cv::circle(original, cv::Point(posX, posY), 7, cv::Scalar(255, 255, 255), -1);
                cv::circle(original, cv::Point(posX, posY), 8, cv::Scalar(0, 0, 0), 2);
                switch(action){

                  if (dArea > minObjectArea) {
                    objectFound = 1;
                  }
                  else{
                    objectFound = 0;
                  }

                  case 0:{ //approach first buoy (red) YO MA OG GIMMME CENTROID OF RED
                    ColorSpace = 0;  //(red)
                    if(!objectFound || dArea > 74419200){
                      pm_.setControlEffort(AXIS_SURGE, 0);
                      redDepth = pm_.getDepth();
                      action = 1;
                      ROS_INFO("In place to ram first buoy. BOP IT!");
                      break;
                    }
                    else{
                      pm_.setSetpoint(AXIS_YAW, INPUT_CAM_FRONT, 360);
                      pm_.setSetpoint(AXIS_SWAY, INPUT_CAM_FRONT, 360);
                      pm_.setSetpoint(AXIS_HEAVE, INPUT_CAM_FRONT, 240);
                      pm_.setControlEffort(AXIS_SURGE, 15);
                      action = 0;
                      break;
                    }
                  }

                  case 1:{ //BOP IT! (ram it)
                    if(ramRedCounter < 1){
                      ramRed.start();
                      ramRedCounter++;
                      pm_.setZero(AXIS_YAW);
                    }

                    pm_.setSetpoint(AXIS_HEAVE, INPUT_DEPTH, redDepth);
                    pm_.setPidEnabled(AXIS_SWAY, 0);
                    pm_.setControlEffort(AXIS_SWAY, 0);
                    pm_.setSetpoint(AXIS_YAW, INPUT_IMU_POS, 0);

                    if(ramRed.getTime() <=2)
                      pm_.setControlEffort(AXIS_SURGE, 15);
                    else if(ramRed.getTime() > 2 && ramRed.getTime() <= 2.5)
                      pm_.setControlEffort(AXIS_SURGE, 0);
                    else if(ramRed.getTime() > 2.5 && ramRed.getTime() <= 6)
                      pm_.setControlEffort(AXIS_SURGE, -15);
                    else{
                      pm_.setControlEffort(AXIS_SURGE, 0);
                      action = 2;
		      return 0;
                      break;
                    }
                  }

                  case 2:{ //TWIST IT! (rotate to match green)

                  }

                  case 3:{ //approach second buoy (green)

                  }

                  case 4:{ //BOP-IT! (ram it)

                  }

                  case 5:{//PULL-IT! (back off)

                  }

                  case 6:{//TWIST IT! (go around)

                  }

                  case 7:{ //move on

                  }

                  case 9:{//SPIN IT! (look around if can't find object)

                  }

                }

/*
                if (dArea > 74419200) { //  Stop, move backwards and switch to the next buoy


                }
                else { // move forwards


                }

                ROS_ERROR(" Object found? %d Coords(corrected): %f %f ", objectFound, posXcorrected, posYcorrected);

                if (cv::waitKey(30) == 27) //wait for 'esc' key press for 30ms. If 'esc' key is pressed, break loop
                {
                        break;
                }

                if(!outOfSight) {


                        if (fabs(plantState_sway - setpoint_sway) > 40) {//SOME REASONABLE DEADBAND
                                pm_.setControlEffort(AXIS_SURGE, 0); //makes it so the robot doesn't try to move forward if the sway and heave are outside of a DEADBAND

                        }

                        else{
                                pm_.setControlEffort(AXIS_SURGE, surgeSpeed); //random number for now, will need to be replaced by testing. Will probably be a percent, but idk
                        }



                }

                else{ // if out of sight...


                        pm_.setPidEnabled(AXIS_SWAY, false);
                        pm_.setPidEnabled(AXIS_SURGE, false);//disable PID

                        if (true) {//check if there is a centroid tracked.... if there is, go to. It will be path marker
                                pm_.setPidEnabled(AXIS_SURGE, true);
                                pm_.setSetpoint(AXIS_SURGE, INPUT_CAM_BTM, 240); //set to go straight until centroid is in middle of bottom cam
                                if(fabs(plantState_surge - setpoint_surge) < 10) { //if it's pretty close to centered
                                        sleep(5); //give it some time to finish up
                                        return succeeded; //yay
                                }
                        }
                        else{
                                pm_.setControlEffort(AXIS_SURGE, 30);
                        }

                }//else
*/
  } //ros::ok

}
