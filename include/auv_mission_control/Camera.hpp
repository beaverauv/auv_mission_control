#ifndef CAMERA_MANAGER_H
#define CAMERA_MANAGER_H

#include <ctime>
#include <iostream>
#include <sstream>

#include <opencv2/highgui/highgui.hpp>
#include <ros/ros.h>

#include <auv_mission_control/task/Task.hpp>

class Camera : public TaskBase {
private:
  cv::VideoCapture capFront;
  cv::VideoCapture capBottom;

  bool isFrontOpened = false;
  bool isBottomOpened = false;

  cv::VideoWriter writerFront;
  cv::VideoWriter writerBottom;

  bool isFrontRecording = false;
  bool isBottomRecording = false;

  cv::Mat lastFrontImage;
  cv::Mat lastBottomImage;

  int imgWidth = 640;
  int imgHeight = 480;

  int camFPS = 24;

  int codeFourcc = CV_FOURCC('H', '2', '6', '4');

  char filenameVideoFront[1024];
  char filenameVideoBottom[1024];

  ros::Rate fpsRate = ros::Rate(camFPS);

  std::string getTag() { return std::string("[Camera]"); }

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
