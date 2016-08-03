#ifndef CAMERA_MANAGER_H
#define CAMERA_MANAGER_H

#include <thread.h>

#include <opencv2/highgui/highgui.hpp>
#include <ros/ros.h>



#include <auv_mission_control/Task.h>


class Camera : public Task {
private:
        cv::VideoCapture capFront;
        cv::VideoCapture capBottom;

        cv::VideoWriter writerFront;
        cv::VideoWriter writerBottom;

        cv::Mat lastFrontImage;
        cv::Mat lastBottomImage;

        int imgWidth = 640;
        int imgHeight = 480;

        int camFPS = 24;

        bool isRecording = false;

        int codeFourcc = CV_FOURCC('H','2','6','4');

        std::string filenameVideoFront;
        std::string filenameVideoBottom;

        ros::Rate fpsRate = ros::Rate(camFPS);
        std::thread threadRecord;

        std::string Camera::getTag(){
                return std::string("[Task Buoy]");
        }

public:
        Camera();
        ~Camera();
        cv::Mat getFront();
        cv::Mat getBottom();

        void startRecording();

        void updateFrames(void *);

};

#endif
