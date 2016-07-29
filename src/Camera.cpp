#include <auv_mission_control/Camera.h>


Camera::Camera(){
  frontCap.open(0);
  if (!frontCap.isOpened()){
    ROS_ERROR("Unable to open Front Camera");
  }

  bottomCap.open(1);
  if (!bottomCap.isOpened()){
    ROS_ERROR("Unable to open bottom Camera");
  }
  frontCap.set(CV_CAP_PROP_FRAME_WIDTH, 640);
  frontCap.set(CV_CAP_PROP_FRAME_HEIGHT, 480);
  bottomCap.set(CV_CAP_PROP_FRAME_WIDTH, 640);
  bottomCap.set(CV_CAP_PROP_FRAME_HEIGHT, 480);
//
}

Camera::~Camera(){

}

void Camera::updateFront(){

  if (!frontCap.read(lastFrontImage)){
    ROS_ERROR("Failed to read from front camera");
  }
}

void Camera::updateBottom(){
  if (!bottomCap.read(lastBottomImage)){
    ROS_ERROR("Failed to read from bottom Camera");
  }
}

cv::Mat Camera::getFront(){
  return lastFrontImage;
}
cv::Mat Camera::getBottom(){
  return lastBottomImage;
}
