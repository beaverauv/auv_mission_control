#ifndef TASKVISION_H
#define TASKVISION_H

#define COLOR_RED 0
#define COLOR_GREEN 1

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include <ros/ros.h>

#include <auv_mission_control/Camera.h>




class TaskVision {
public:
  TaskVision();
  TaskVision(Camera* cam);
  ~TaskVision();


  void findBuoy(int color);
  double getBuoyArea();
  double getBuoyCoordX();
  double getBuoyCoordY();

  void findMarker();
  double getMarkerArea();
  double getMarkerCoordX();
  double getMarkerCoordY();
  double getMarkerAngle();

private:

  Camera cam_;
  cv::Scalar sRedMin = cv::Scalar(116, 0, 0);
  cv::Scalar sRedMax = cv::Scalar(255, 255, 255);

  cv::Scalar sGreenMin = cv::Scalar(0, 0, 0);
  cv::Scalar sGreenMax = cv::Scalar(255, 255, 255);

  cv::Scalar sOrangeMin = cv::Scalar(0,0,0);
  cv::Scalar sOrangeMax = cv::Scalar(76,85,200);

  cv::Mat imgOrigFront;
  cv::Mat imgOrigBottom;

  cv::Mat imgHlsFront;
  cv::Mat imgHlsBottom;

  cv::Mat imgThreshFront;
  cv::Mat imgThreshBottom;

  cv::Mat imgContoursBottom;

  cv::Moments momentsBuoy;
  cv::Moments momentsMarker;

  double buoyArea;
  double buoyCoordX;
  double buoyCoordY;

  double buoyCoordCorrectedX;
  double buoyCoordCorrectedY;

  double markerArea;
  double markerCoordX;
  double markerCoordY;

  double markerCoordCorrectedX;
  double markerCoordCorrectedY;


  std::vector <std::vector <cv::Point> > contours;
  std::vector <cv::Vec4i> hierarchy;

  cv::RotatedRect markerRect;

  int largestArea;
  int largestContourIndex;
  int numLargeContours;

  double currentAngle;
  double averagedAngle;

};






#endif
