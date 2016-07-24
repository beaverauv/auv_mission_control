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

        cv::namedWindow("Original", CV_WINDOW_AUTOSIZE);
        cv::namedWindow("controlGate", CV_WINDOW_AUTOSIZE);

        cv::createTrackbar("minR", "controlGate", &minR, 255);
        cv::createTrackbar("maxR", "controlGate", &maxR, 255);
        cv::createTrackbar("minG", "controlGate", &minG, 255);
        cv::createTrackbar("maxG", "controlGate", &maxG, 255);
        cv::createTrackbar("minB", "controlGate", &minB, 255);
        cv::createTrackbar("maxB", "controlGate", &maxB, 255);

        while(ros::ok) { // change so it's while keep running, some value that determines whether to keep running

                pm_.setSetpoint(AXIS_YAW, INPUT_IMU_POS, 0);

                cam_.update();

                cv::Mat original = cam_.getFront();
                cv::Mat imgHSV;
                cv::Mat imgThresh;

                // cv::cvtColor(original,imgHSV,CV_BGR2HSV);

                cv::inRange(original, cv::Scalar(minR, minG, minB), cv::Scalar(maxR, maxG, maxB), imgThresh);
                cv::erode(imgThresh, imgThresh, cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(5, 5)));
                cv::dilate(imgThresh, imgThresh, cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(10, 10)));

                // ROS_ERROR("4");

                cv::Moments oMoments = cv::moments(imgThresh);

                double posX = oMoments.m10 / oMoments.m00;
                double posY = oMoments.m01 / oMoments.m00;
                double dArea = oMoments.m00;
                double posXcorrected = 640 - (oMoments.m10 / oMoments.m00);
                double posYcorrected = 480 - (oMoments.m01 / oMoments.m00);

                //ROS_ERROR("%d", ColorSpace);

                cv::circle(original, cv::Point(posX, posY), 6, cv::Scalar(255, 255, 255), -1);
                cv::circle(original, cv::Point(posX, posY), 7, cv::Scalar(0, 0, 0), 2);

                if (dArea > minObjectArea) {
                        objectFound = 1;
                }
                else {
                        objectFound = 0;
                }

                // ROS_ERROR(" Object found? %d Coords(corrected): %f %f ", objectFound, posXcorrected, posYcorrected);
                ROS_INFO("\033[2J\033[1;1H");
                ROS_INFO("Located at: %f %f", posX, posY);

                cv::imshow("original", original);
                cv::imshow("Control", imgThresh);

                if (cv::waitKey(30) == 27) //wait for 'esc' key press for 30ms. If 'esc' key is pressed, break loop
                {
                        break;
                }

                if(pm_.getKill()) {
                        return kill;
                }
                if(getTimeout()) {
                        return timeout;
                }


                switch(action) {

                case 0: { //Go To Depth
                        pm_.setPidEnabled(AXIS_SWAY, true);
                        pm_.setPidEnabled(AXIS_YAW, true);
                        pm_.setPidEnabled(AXIS_HEAVE, true);
                        pm_.setPidEnabled(AXIS_SURGE, false);
                        depthTimer.start();
                        if(depthCounter < 0) {
                                depthTimer.start();
                                depthCounter++;
                        }
                        if(depthTimer.getTime() <= 8) {
                                pm_.setSetpoint(AXIS_YAW, INPUT_CAM_FRONT, 320);
                                pm_.setSetpoint(AXIS_SWAY, INPUT_CAM_FRONT, 320);
                                pm_.setSetpoint(AXIS_HEAVE, INPUT_DEPTH, -1.25);
                                ROS_INFO("Going to depth for %f more seconds. At depth of %f", (8.0 - depthTimer.getTime()), pm_.getDepth());
                        }

                        else{
                                ROS_INFO("Moving on, at depth of %f", pm_.getDepth());
                                action = 1;
                                break;
                        }

                }

                case 1: {//start going forward, use vision for 10 sec
                        camInUse = INPUT_CAM_FRONT;
                        pm_.setPidEnabled(AXIS_SWAY, true);
                        pm_.setPidEnabled(AXIS_YAW, true);
                        pm_.setPidEnabled(AXIS_HEAVE, true);
                        pm_.setPidEnabled(AXIS_SURGE, false);

                        if(goCounter < 1) {
                                forwardTimer.start();
                                goCounter++;
                        }
                        if(forwardTimer.getTime() <= 10) {
                                ROS_INFO("Moving forward with vision for %f more seconds", (10 - forwardTimer.getTime()) );
                                pm_.setSetpoint(AXIS_YAW, INPUT_CAM_FRONT, 320);
                                pm_.setSetpoint(AXIS_SWAY, INPUT_CAM_FRONT, 320);
                                pm_.setSetpoint(AXIS_HEAVE, INPUT_DEPTH, -1.25);
                                pm_.setControlEffort(AXIS_SURGE, 25);//manually set to drive forward at speed 25
                        }
                        else{
                                ROS_INFO("Done using vision, dead reackoning rest of way");
                                action = 2;
                                break;
                        }
                }

                case 2: { //dead reckon rest of way, looking for bottom
                        if(counter < 1) { //setZero the yaw, but only once.
                                pm_.setZero(IMU_YAW);
                        }

                        camInUse = INPUT_CAM_BTM;
                        pm_.setPidEnabled(AXIS_SWAY, false);
                        pm_.setPidEnabled(AXIS_YAW, true);
                        pm_.setPidEnabled(AXIS_HEAVE, true);
                        pm_.setPidEnabled(AXIS_SURGE, false);
                        pm_.setSetpoint(AXIS_HEAVE, INPUT_DEPTH, -1.25);

                        if(!objectFound) {//if ya don't see anything on the floor, just go forward
                                ROS_INFO("No path marker found");
                                pm_.setSetpoint(AXIS_YAW, INPUT_IMU_POS, 0);
                                pm_.setControlEffort(AXIS_SURGE, 25);
                        }
                        else{
                                ROS_INFO("Bottom path marker found. I'm chasing it. Vroom.");
                                pm_.setPidEnabled(AXIS_SURGE, true);
                                pm_.setSetpoint(AXIS_YAW, INPUT_IMU_POS, 0);
                                pm_.setSetpoint(AXIS_SURGE, INPUT_CAM_BTM, 240);
                                if(fabs(posY - setpoint_surge) <= 20) {
                                        if(finalCounter < 1) {
                                                finalTimer.start();
                                                finalCounter += 102;
                                        }
                                        if(finalTimer.getTime() >= 3) {
                                                ROS_INFO("All done. Hopefully I'm in the right place. Sorry if I'm not, I understand that could be highly frustrating. Congrats on the good work if I am. Really well done, champ.");//What's a robot without an attitude?
                                                action = 3;
                                                break;
                                        }

                                }
                        }

                        counter++; //please don't setZero the yaw again, thanks.
                }

                case 3:
                        return succeeded;

                default:
                        break;

                }//switch
        }//while ros::ok
}//execute
