#ifndef CAMERA_MANAGER_H
#define CAMERA_MANAGER_H

#include <opencv2/highgui/highgui.hpp>
#include <ros/ros.h>

#include <auv_mission_control/Task.h>


class Camera : public Task {
private:
        cv::VideoCapture frontCap;
        cv::VideoCapture bottomCap;


        cv::Mat lastFrontImage;
        cv::Mat lastBottomImage;

        std::string getTag();


public:
        Camera();
        ~Camera();
        cv::Mat getFront();
        cv::Mat getBottom();
        void updateFront();
        void updateBottom();

};

#endif
