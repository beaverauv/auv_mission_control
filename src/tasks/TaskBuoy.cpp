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
        cv::namedWindow("ControlHSV", CV_WINDOW_AUTOSIZE);
        // cv::namedWindow("ControlRGB", CV_WINDOW_AUTOSIZE);
        cv::namedWindow("ControlLAB", CV_WINDOW_AUTOSIZE);
        cv::namedWindow("HSV", CV_WINDOW_AUTOSIZE);
        cv::namedWindow("bitwiseFinal", CV_WINDOW_AUTOSIZE);
        cv::namedWindow("hsv+lab", CV_WINDOW_AUTOSIZE);
        cv::namedWindow("lab", CV_WINDOW_AUTOSIZE);

        cv::createTrackbar("minH", "ControlHSV", &minH, 255);
        cv::createTrackbar("maxH", "ControlHSV", &maxH, 255);
        cv::createTrackbar("minS", "ControlHSV", &minS, 255);
        cv::createTrackbar("maxS", "ControlHSV", &maxS, 255);
        cv::createTrackbar("minV", "ControlHSV", &minV, 255);
        cv::createTrackbar("maxV", "ControlHSV", &maxV, 255);

        // cv::createTrackbar("minR", "ControlRGB", &minR, 255);
        // cv::createTrackbar("maxR", "ControlRGB", &maxR, 255);
        // cv::createTrackbar("minG", "ControlRGB", &minG, 255);
        // cv::createTrackbar("maxG", "ControlRGB", &maxG, 255);
        // cv::createTrackbar("minB", "ControlRGB", &minB, 255);
        // cv::createTrackbar("maxB", "ControlRGB", &maxB, 255);

        cv::createTrackbar("minL", "ControlLAB", &minL, 255);
        cv::createTrackbar("maxL", "ControlLAB", &maxL, 255);
        cv::createTrackbar("minA", "ControlLAB", &minA, 255);
        cv::createTrackbar("maxA", "ControlLAB", &maxA, 255);
        cv::createTrackbar("minlaB", "ControlLAB", &minlaB, 255);
        cv::createTrackbar("maxlaB", "ControlLAB", &maxlaB, 255);
        //      cv::createTrackbar("ColorSControlHSV2pace", "Control", &ColorSpace, 2);

        // ROS_ERROR("2");

        while (ros::ok) {

                // ros::spinOnce();

                pm_.setSetpoint(AXIS_YAW, INPUT_IMU_POS, 0);

                cam_.update();

                cv::Mat original = cam_.getFront();
                cv::Mat imgHSV;
                cv::Mat hsvThresh;
                // cv::Mat rgbThresh;
                cv::Mat hsv+lab;
                cv::Mat bitwiseFinal;
                cv::Mat Lab;
                cv::Mat labThresh;

                cv::cvtColor(original,imgHSV,CV_BGR2HSV);
                cv::cvtColor(original,Lab, CV_BGR2Lab);

                cv::inRange(imgHSV, cv::Scalar(minH, minS, minV), cv::Scalar(maxH, maxS, maxV), hsvThresh);
                cv::erode(hsvThresh, hsvThresh, cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(5, 5)));
                cv::dilate(hsvThresh, hsvThresh, cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(10, 10)));

                // cv::inRange(original, cv::Scalar(minR, minG, minB), cv::Scalar(maxR, maxG, maxB), rgbThresh);
                // cv::erode(rgbThresh, rgbThresh, cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(5, 5)));
                // cv::dilate(rgbThresh, rgbThresh, cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(10, 10)));

                cv::inRange(Lab, cv::Scalar(minL, minA, minlaB), cv::Scalar(maxL, maxA, maxlaB), labThresh);
                cv::erode(labThresh, labThresh, cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(5, 5)));
                cv::dilate(labThresh, labThresh, cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(10, 10)));


                cv::bitwise_and(hsvThresh, labThresh, hsv+lab);
                // cv::bitwise_and(hsv+lab, rgbThresh, bitwiseFinal);
                // cv::addWeighted(hsvThresh, 1.0, rgbThresh, 1.0, 0.0, bitwise);

                cv::Moments hsv1Moments = cv::moments(hsvThresh);
                cv::Moments hsv2Moments = cv::moments(labThresh);
                cv::Moments oMoments = cv::moments(hsv+lab);

                double posX = oMoments.m10 / oMoments.m00;
                double posY = oMoments.m01 / oMoments.m00;
                double dArea = oMoments.m00;
                double hsv1Area = hsv1Moments.m00;
                double hsv2Area = hsv2Moments.m00;
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
                ROS_INFO("Located at: %f %f ", posX, posY);
                ROS_INFO("Area Image 1: %f Area Image 2: %f Area Image Bitwise: %f", hsv1Area, hsv2Area, dArea);

                cv::imshow("original", original);

                cv::imshow("ControlHSV", hsvThresh);
                // cv::imshow("ControlRGB", rgbThresh);
                cv::imshow("ControlLAB", labThresh);
                cv::imshow("hsv+lab", hsv+lab);
                // cv::imshow("bitwiseFinal", bitwiseFinal);
                cv::imshow("HSV", imgHSV);
                // cv::imshow("Bitwise", bitwise);
                cv::imshow("lab", Lab);

                if (cv::waitKey(30) == 27) //wait for 'esc' key press for 30ms. If 'esc' key is pressed, break loop
                {
                        break;
                }

                if (objectFound = 1) { //is there an object that meets the minimum size?

                        if (fabs(plantState_sway - setpoint_sway) > 40) {//SOME REASONABLE DEADBAND
                                pm_.setControlEffort(AXIS_SURGE, 0); //makes it so the robot doesn't try to move forward if the sway and heave are outside of a DEADBAND

                        }

                        else{
                                pm_.setControlEffort(AXIS_SURGE, surgeSpeed); // random number for now, will need to be replaced by testing. Will probably be a percent, but idk
                        }
                }

                else {

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

                }

                ros::spinOnce();

        } //ros::ok

} // end of function
