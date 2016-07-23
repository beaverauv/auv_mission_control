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
        cv::namedWindow("original", CV_WINDOW_AUTOSIZE); //create a window with the name "OpenCV_Window"
        cv::namedWindow("Control", CV_WINDOW_AUTOSIZE);

        cv::createTrackbar("minR", "Control", &minR, 255); //Hue (0 - 179)
        cv::createTrackbar("maxR", "Control", &maxR, 255);
        cv::createTrackbar("minG", "Control", &minG, 255); //Hue (0 - 179)
        cv::createTrackbar("maxG", "Control", &maxG, 255);
        cv::createTrackbar("minB", "Control", &minB, 255); //Hue (0 - 179)
        cv::createTrackbar("maxB", "Control", &maxB, 255);
        cv::createTrackbar("ColorSpace", "Control", &ColorSpace, 2);

        while (ros::ok) {

                ros::spinOnce();

                pm_.setSetpoint(AXIS_YAW, INPUT_IMU_POS, 0);

                cam_.update();
                cv::Mat original = cam_.getFront();

                cv::Mat imgHSV = original;
                cv::Mat imgThresh;

                cv::inRange(original, cv::Scalar(minR, minG, minB), cv::Scalar(maxR, maxG, maxB), imgThresh);
                cv::erode(imgThresh, imgThresh, cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(5, 5)));
                cv::dilate(imgThresh, imgThresh, cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(10, 10)));

                cv::Moments oMoments = cv::moments(imgThresh);

                int dM01 = oMoments.m01;
                int dM10 = oMoments.m10;
                double dArea = oMoments.m00;
                int posX = dM10 / dArea;
                int posY = dM01 / dArea;
                double posXdouble = (double)posX;
                double posYdouble = (double)posY;
                double posXcorrected = 640 - posXdouble;
                double posYcorrected = 480 - posYdouble;

                //ROS_ERROR("%d", ColorSpace);

                cv::imshow("original", original);
                cv::imshow("Control", imgThresh);

                if ( ColorSpace == 0 ) {
                        // minR = 63;
                        // maxR = 115;
                        // minG = 0;
                        // maxG = 150;
                        // minB = 237;
                        // maxB = 255;
                        cv::setTrackbarPos("minR", "Control", 63);
                        cv::setTrackbarPos("maxR", "Control", 115);
                        cv::setTrackbarPos("minG", "Control", 0);
                        cv::setTrackbarPos("maxG", "Control", 150);
                        cv::setTrackbarPos("minB", "Control", 237);
                        cv::setTrackbarPos("maxB", "Control", 255);
                }
                else if ( ColorSpace == 1 ) {
                        // minR = 61;
                        // maxR = 101;
                        // minG = 105;
                        // maxG = 158;
                        // minB = 48;
                        // maxB = 100;
                        cv::setTrackbarPos("minR", "Control", 61);
                        cv::setTrackbarPos("maxR", "Control", 101);
                        cv::setTrackbarPos("minG", "Control", 105);
                        cv::setTrackbarPos("maxG", "Control", 158);
                        cv::setTrackbarPos("minB", "Control", 48);
                        cv::setTrackbarPos("maxB", "Control", 100);
                }

                else if ( ColorSpace == 2 ) {
                        // minR = 73;
                        // maxR = 137;
                        // minG = 84;
                        // maxG = 125;
                        // minB = 199;
                        // maxB = 226;
                        cv::setTrackbarPos("minR", "Control", 73);
                        cv::setTrackbarPos("maxR", "Control", 137);
                        cv::setTrackbarPos("minG", "Control", 84);
                        cv::setTrackbarPos("maxG", "Control", 125);
                        cv::setTrackbarPos("minB", "Control", 199);
                        cv::setTrackbarPos("maxB", "Control", 226);
                }

                cv::circle(original, cv::Point(posX, posY), 7, cv::Scalar(255, 255, 255), -1);
                cv::circle(original, cv::Point(posX, posY), 8, cv::Scalar(0, 0, 0), 2);

                if (dArea > minObjectArea) {
                        objectFound = 1;

                }

                else{
                        objectFound = 0;

                }


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

        } //ros::ok

}
