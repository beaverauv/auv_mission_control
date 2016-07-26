#include <auv_mission_control/TaskMarker.h>
#include <opencv2/imgproc/imgproc.hpp>
#include <cmath>


TaskMarker::TaskMarker(){
}


TaskMarker::TaskMarker(PidManager* pm, Camera* cam) : pm_(*pm), cam_(*cam){
}

TaskMarker::~TaskMarker(){

}

int TaskMarker::execute(){

        pm_.setPidEnabled(AXIS_SURGE, false);
        pm_.setPidEnabled(AXIS_SWAY, true);
        pm_.setPidEnabled(AXIS_YAW, true);
        pm_.setPidEnabled(AXIS_HEAVE, true);

        pm_.setZero(AXIS_YAW);
        pm_.setSetpoint(AXIS_YAW, INPUT_IMU_POS, 0);


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

                        // fitEllipseMat = hlslab;
                        //
                        // std::vector < std::vector <cv::Point> > contours;
                        // std::vector <cv::Vec4i> hierarchy;
                        //
                        // cv::findContours( fitEllipseMat, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, cv::Point(0, 0) );
                        //
                        // //std::vector<cv::RotatedRect> minEllipse( contours.size() );
                        //
                        // cv::RotatedRect marker_angle;
                        // int numBigConours = 0;
                        // int largest_area = 0;
                        // int largest_contour_index = 0;
                        //
                        //
                        //
                        // for( int i = 0; i < contours.size(); i++ )
                        // {
                        //         double count = contours[i].size();
                        //
                        //         //ROS_INFO("%f",count);
                        //         if( count < 6 )
                        //                 continue;
                        //         numBigConours++;
                        //         double a = contourArea ( contours[i], false);
                        //         if ( a > largest_area )
                        //         {
                        //                 largest_area = a;
                        //                 largest_contour_index = i;
                        //         }
                        //         //cv::ellipse = cv::fitEllipse( cv::Mat(contours[]) );
                        //
                        //         // for( int j = 0; j < 4; j++ )
                        //         //         cv::line( original, rect_points[j], rect_points[(j+1)%4], cv::Scalar(0,0,255), 1, 8 );
                        // }
                        // ROS_INFO("\033[2J\033[1;1H");
                        // if (numBigConours >= 1){
                        //   marker_angle = cv::fitEllipse(contours[largest_contour_index]);
                        //
                        //   ROS_INFO("%d", largest_contour_index);
                        //
                        //   cv::Point2f rect_points[4];
                        //   //minEllipse[largest_contour_index].points( rect_points );
                        //   marker_angle.points( rect_points );
                        //
                        //   //  marker_angle = cv::fitEllipse( cv::Mat(contours[i]) );
                        //
                        //   double angle = marker_angle.angle - 90;
                        //   if (marker_angle.size.width < marker_angle.size.height)
                        //           angle = 90 + angle;
                        //
                        //   ROS_INFO("%f", angle);
                        //   //cv::drawContours( original, contours, largest_contour_index, cv::Scalar(0,255,0), 1, 8, std::vector<cv::Vec4i>(), 0, cv::Point() );
                        //   // ellipse
                        //   cv::ellipse( original, marker_angle, cv::Scalar(255,0,0), 2, 8 );
                        //
                        // } else {
                        //   ROS_INFO("No Contours greater than 6");
                        // }

                        //minEllipse[largest_contour_index] = cv::fitEllipse( cv::Mat(contours[largest_contour_index]) );






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

                switch(action) {
                case 0: {
                        int counter = 0;
                        if(counter < 1)
                                depthTimer.start();
                        counter++;
                        pm_.setSetpoint(AXIS_HEAVE, INPUT_DEPTH, -2.45);
                        if(depthTimer.getTime() >= 10) {
                                action = 1;
                                ROS_INFO("At depth, moving on");
                                break;
                        }
                }

                case 1: {
                        pm_.setSetpoint(AXIS_SWAY, INPUT_CAM_BTM, 320);
                        setpoint_sway = 320;
                        int counter = 0;
                        if (counter < 1)
                                swayTimer.start();
                        counter++;

                        if(swayTimer.getTime() >= 5) {
                                action = 2;
                                ROS_INFO("Sway in place, moving on to yaw");
                                break;
                        }

                }

                case 2: {
                        int counter = 0;
                        if (counter < 1)
                                yawTimer.start();
                        counter++;
                        if(yawTimer.getTime() <= 5) {
                                pm_.setSetpoint(AXIS_YAW, INPUT_CAM_BTM, 0);
                        }
                        else{
                                pm_.setZero(AXIS_YAW);
                                pm_.setSetpoint(AXIS_YAW, INPUT_IMU_POS, 0);
                                action = 3;
                                ROS_INFO("Yaw alighned, locking position and moving forwards");
                                break;
                        }
                }

                case 3: {
                        break;
                }
                }
        }
}
