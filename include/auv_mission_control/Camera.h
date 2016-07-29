#ifndef CAMERA_MANAGER_H
#define CAMERA_MANAGER_H

#include <opencv2/highgui/highgui.hpp>
#include <ros/ros.h>

class Camera {
private:
  cv::VideoCapture frontCap;
  cv::VideoCapture bottomCap;


  cv::Mat lastFrontImage;
  cv::Mat lastBottomImage;

public:
  Camera();
  ~Camera();
  cv::Mat getFront();
  cv::Mat getBottom();
  void updateFront();
  void updateBottom();

};

#endif
