#ifndef CAMERA_MANAGER_H
#define CAMERA_MANAGER_H

#include <opencv2/highgui/highgui.hpp>
#include <ros/ros.h>

class CameraManager {
private:
  cv::VideoCapture frontCap;
  cv::VideoCapture bottomCap;


  cv::Mat lastFrontImage;
  cv::Mat lastBottomImage;

public:
  CameraManager();
  ~CameraManager();
  cv::Mat getFrontCamera();
  cv::Mat getBottomCamera();
  void updateCameras();

};

#endif
