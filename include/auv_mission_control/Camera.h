#ifndef CAMERA_MANAGER_H
#define CAMERA_MANAGER_H

#include <ctime>

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

        std::string getTag(){
                return std::string("[Camera]");
        }

public:
        Camera();
        ~Camera();
        cv::Mat getFront();
        cv::Mat getBottom();

        void startRecording();
        void stopRecording();

        void updateFrames();

};

#endif
