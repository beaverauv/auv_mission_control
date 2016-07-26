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
        cv::namedWindow("ControlHLS", CV_WINDOW_AUTOSIZE);
        // cv::namedWindow("ControlRGB", CV_WINDOW_AUTOSIZE);
        cv::namedWindow("ControlLAB", CV_WINDOW_AUTOSIZE);
        cv::namedWindow("HLS", CV_WINDOW_AUTOSIZE);
        cv::namedWindow("hlslab", CV_WINDOW_AUTOSIZE);
        cv::namedWindow("lab", CV_WINDOW_AUTOSIZE);

        cv::createTrackbar("minH", "ControlHLS", &minH, 255);
        cv::createTrackbar("maxH", "ControlHLS", &maxH, 255);
        cv::createTrackbar("minS", "ControlHLS", &minS, 255);
        cv::createTrackbar("maxS", "ControlHLS", &maxS, 255);
        cv::createTrackbar("minV", "ControlHLS", &minV, 255);
        cv::createTrackbar("maxV", "ControlHLS", &maxV, 255);

        cv::createTrackbar("minL", "ControlLAB", &minL, 255);
        cv::createTrackbar("maxL", "ControlLAB", &maxL, 255);
        cv::createTrackbar("minA", "ControlLAB", &minA, 255);
        cv::createTrackbar("maxA", "ControlLAB", &maxA, 255);
        cv::createTrackbar("minlaB", "ControlLAB", &minlaB, 255);
        cv::createTrackbar("maxlaB", "ControlLAB", &maxlaB, 255);
        //      cv::createTrackbar("ColorSControlHLS2pace", "Control", &ColorSpace, 2);

        // ROS_ERROR("2");

        while (ros::ok) {

                // ros::spinOnce();

                pm_.setSetpoint(AXIS_YAW, INPUT_IMU_POS, 0);

                cam_.update();

                cv::Mat original = cam_.getFront();
                cv::Mat imgHLS;
                cv::Mat hlsThresh;
                // cv::Mat rgbThresh;
                cv::Mat hlslab;
                cv::Mat fitEllipseMat;
                cv::Mat Lab;
                cv::Mat labThresh;



                cv::cvtColor(original,imgHLS,CV_BGR2HLS);
                cv::cvtColor(original,Lab, CV_BGR2Lab);

                cv::inRange(imgHLS, cv::Scalar(minH, minS, minV), cv::Scalar(maxH, maxS, maxV), hlsThresh);
                cv::erode(hlsThresh, hlsThresh, cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(5, 5)));
                cv::dilate(hlsThresh, hlsThresh, cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(10, 10)));

                // cv::inRange(original, cv::Scalar(minR, minG, minB), cv::Scalar(maxR, maxG, maxB), rgbThresh);
                // cv::erode(rgbThresh, rgbThresh, cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(5, 5)));
                // cv::dilate(rgbThresh, rgbThresh, cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(10, 10)));

                cv::inRange(Lab, cv::Scalar(minL, minA, minlaB), cv::Scalar(maxL, maxA, maxlaB), labThresh);
                cv::erode(labThresh, labThresh, cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(5, 5)));
                cv::dilate(labThresh, labThresh, cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(10, 10)));


                cv::bitwise_and(hlsThresh, labThresh, hlslab);
                // cv::addWeighted(hlsThresh, 1.0, rgbThresh, 1.0, 0.0, bitwise);

                cv::Moments hls1Moments = cv::moments(hlsThresh);
                cv::Moments hls2Moments = cv::moments(labThresh);
                cv::Moments oMoments = cv::moments(hlslab);

                double posX = oMoments.m10 / oMoments.m00;
                double posY = oMoments.m01 / oMoments.m00;
                double dArea = oMoments.m00;
                double hls1Area = hls1Moments.m00;
                double hls2Area = hls2Moments.m00;
                double posXcorrected = 640 - (oMoments.m10 / oMoments.m00);
                double posYcorrected = 480 - (oMoments.m01 / oMoments.m00);

                fitEllipseMat = hlslab;

                std::vector < std::vector <cv::Point> > contours;
                std::vector <cv::Vec4i> hierarchy;

                cv::findContours( fitEllipseMat, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, cv::Point(0, 0) );

                std::vector<cv::RotatedRect> minEllipse( contours.size() );

                cv::RotatedRect marker_angle;



                for( int i = 0; i < contours.size(); i++ )
                {
                        double count = contours[i].size();

                        //ROS_INFO("%f",count);
                        if( count < 6 )
                                continue;

                        double a = contourArea ( contours[i], false);
                        if ( a > largest_area )
                        {
                                largest_area = a;
                                largest_contour_index = i;
                        }
                        minEllipse[i] = cv::fitEllipse( cv::Mat(contours[i]) );

                        ROS_INFO("%d", largest_contour_index);


                        //cv::ellipse = cv::fitEllipse( cv::Mat(contours[]) );
                        cv::Point2f rect_points[4]; minEllipse[i].points( rect_points );
                        marker_angle = cv::fitEllipse( cv::Mat(contours[i]) );

                        double angle = marker_angle.angle - 90;
                        if (marker_angle.size.width < marker_angle.size.height)
                                angle = 90 + angle;

                        ROS_INFO("\033[2J\033[1;1H");
                        ROS_INFO("%f", angle);
                        cv::drawContours( original, contours, largest_contour_index, cv::Scalar(0,255,0), 1, 8, std::vector<cv::Vec4i>(), 0, cv::Point() );
                        // ellipse
                        cv::ellipse( original, minEllipse[i], cv::Scalar(255,0,0), 2, 8 );
                        // for( int j = 0; j < 4; j++ )
                        //         cv::line( original, rect_points[j], rect_points[(j+1)%4], cv::Scalar(0,0,255), 1, 8 );
                }




                // ROS_INFO("\033[2J\033[1;1H");
                // ROS_INFO("Located at: %f %f ", posX, posY);
                // ROS_INFO("Area Image 1: %f Area Image 2: %f Area Image Bitwise: %f", hls1Area, hls2Area, dArea);


                cv::circle(original, cv::Point(posX, posY), 6, cv::Scalar(255, 255, 255), -1);
                cv::circle(original, cv::Point(posX, posY), 7, cv::Scalar(0, 0, 0), 2);

                if (dArea > minObjectArea) {
                        objectFound = 1;
                }
                else {
                        objectFound = 0;
                }

                cv::imshow("original", original);
                cv::imshow("ControlHLS", hlsThresh);
                cv::imshow("ControlLAB", labThresh);
                cv::imshow("hlslab", hlslab);
                cv::imshow("HLS", imgHLS);
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
