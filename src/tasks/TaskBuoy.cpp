#include <auv_mission_control/TaskBuoy.h>
// #include "auv_mission_control/TaskBuoy.h"
#include <opencv2/imgproc/imgproc.hpp>
#include <cmath>


TaskBuoy::TaskBuoy(){

// Task_Buoy::Task_Buoy(){
}


TaskBuoy::TaskBuoy(PidManager* pm, Camera* cam) : pm_(*pm), cam_(*cam){
        ROS_ERROR("TASK GATE INIT");
}

TaskBuoy::~TaskBuoy(){

}


// int TaskBuoy::execute(){
//pm_.setPidEnabled("ALL", true);//turns on all 6 pid controllers
int TaskBuoy::execute(){
        // pm_.pidEnable("ALL", true);//turns on all 6 pid controllers

        pm_.setZero(AXIS_YAW);
        pm_.setSetpoint(AXIS_YAW, INPUT_IMU_POS, 0);
        pm_.setSetpoint(AXIS_HEAVE, INPUT_DEPTH, -1.25);
        // pm_.taskDelay(5);
        cv::namedWindow("original", CV_WINDOW_AUTOSIZE); //create windows
        cv::namedWindow("Control", CV_WINDOW_AUTOSIZE);

        ROS_ERROR("1");

        cv::createTrackbar("minR", "Control", &minR, 255); //Hue (0 - 179)
        cv::createTrackbar("maxR", "Control", &maxR, 255);
        cv::createTrackbar("minG", "Control", &minG, 255); //Hue (0 - 179)
        cv::createTrackbar("maxG", "Control", &maxG, 255);
        cv::createTrackbar("minB", "Control", &minB, 255); //Hue (0 - 179)
        cv::createTrackbar("maxB", "Control", &maxB, 255);
        //      cv::createTrackbar("ColorSpace", "Control", &ColorSpace, 2);

        ROS_ERROR("2");

        while (ros::ok) {

                // ros::spinOnce();

                pm_.setSetpoint(AXIS_YAW, INPUT_IMU_POS, 0);

                cam_.update();

                cv::Mat original = cam_.getFront();
                cv::Mat imgHSV;
                cv::Mat imgThresh;

                ROS_ERROR("3");

                cv::cvtColor(original,imgHSV,CV_BGR2HSV);

                cv::inRange(original, cv::Scalar(minR, minG, minB), cv::Scalar(maxR, maxG, maxB), imgThresh);
                cv::erode(imgThresh, imgThresh, cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(5, 5)));
                cv::dilate(imgThresh, imgThresh, cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(10, 10)));

                ROS_ERROR("4");

                cv::Moments oMoments = cv::moments(imgThresh);

                double posX = oMoments.m10 / oMoments.m00;
                double posY = oMoments.m01 / oMoments.m00;
                double dArea = oMoments.m00;
                double posXcorrected = 640 - (oMoments.m10 / oMoments.m00);
                double posYcorrected = 480 - (oMoments.m01 / oMoments.m00);

                ROS_ERROR("5");

                //ROS_ERROR("%d", ColorSpace);

                cv::circle(original, cv::Point(posX, posY), 7, cv::Scalar(255, 255, 255), -1);
                cv::circle(original, cv::Point(posX, posY), 8, cv::Scalar(0, 0, 0), 2);

                if (dArea > minObjectArea) {
                  objectFound = 1;
                }
                else {
                  objectFound = 0;
                }

                ROS_ERROR("6");

                // if (dArea > 74419200) { //  Stop, move backwards and switch to the next buoy
                //
                //
                // }
                // else { // move forwards
                //
                //
                // }

                ROS_ERROR(" Object found? %d Coords(corrected): %f %f ", objectFound, posXcorrected, posYcorrected);

                cv::imshow("original", original);
                cv::imshow("Control", imgThresh);

                ROS_ERROR("7");

                if (cv::waitKey(30) == 27) //wait for 'esc' key press for 30ms. If 'esc' key is pressed, break loop
                {
                        break;
                }

                // if (objectFound = 1) { //is there an object that meets the minimum size?
                //
                //         if (fabs(plantState_sway - setpoint_sway) > 40) {//SOME REASONABLE DEADBAND
                //                 pm_.setControlEffort(AXIS_SURGE, 0); //makes it so the robot doesn't try to move forward if the sway and heave are outside of a DEADBAND
                //
                //         }
                //
                //         else{
                //                 pm_.setControlEffort(AXIS_SURGE, surgeSpeed); // random number for now, will need to be replaced by testing. Will probably be a percent, but idk
                //         }
                // }
                //
                // else {
                //
                //         if (true) {//check if there is a centroid tracked.... if there is, go to. It will be path marker
                //                 pm_.setPidEnabled(AXIS_SURGE, true);
                //                 pm_.setSetpoint(AXIS_SURGE, INPUT_CAM_BTM, 240); //set to go straight until centroid is in middle of bottom cam
                //                 if(fabs(plantState_surge - setpoint_surge) < 10) { //if it's pretty close to centered
                //                         sleep(5); //give it some time to finish up
                //                         return succeeded; //yay
                //                 }
                //         }
                //         else{
                //                 pm_.setControlEffort(AXIS_SURGE, 30);
                //         }
                //
                // }

                ROS_ERROR("8");

                ros::spinOnce();

        } //ros::ok

} // end of function
