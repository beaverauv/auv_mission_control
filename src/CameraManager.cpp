#include <auv_mission_control/CameraManager.h>


CameraManager::CameraManager(){
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

}

CameraManager::~CameraManager(){

}

void CameraManager::updateCameras(){
  if (!frontCap.read(lastFrontImage)){
    ROS_ERROR("Failed to read from front camera");
  }
  if (!bottomCap.read(lastBottomImage)){
    ROS_ERROR("Failed to read from bottom Camera");
  }
}


cv::Mat CameraManager::getFrontCamera(){
  return lastFrontImage;
}
cv::Mat CameraManager
::getBottomCamera(){
  return lastBottomImage;
}
