#include <auv_mission_control/TemplateStates.hpp>
#include <auv_mission_control/task/Buoy.hpp>

namespace Task {

int Buoy::execute() {
  if (checkEventQueue()) {
    sm_->run();
  }
  pm()->updatePlantState(AXIS::PITCH);
  pm()->updatePlantState(AXIS::ROLL);

}

void Buoy::Init::run() {
  pm()->setEnabled(AXIS::SURGE, 0);//disable surge pid loop
  pm()->setEnabled(AXIS::SWAY, 0);
  pm()->setEnabled(AXIS::HEAVE, 1);//enable all the others
  pm()->setEnabled(AXIS::YAW, 1);
  pm()->setEnabled(AXIS::ROLL, 1);
  pm()->setEnabled(AXIS::PITCH, 1);
  pm()->setSetpoint(AXIS::YAW, INPUT::IMU_POS, 0);
  pm()->setSetpoint(AXIS::ROLL, INPUT::IMU_POS, 0);
  pm()->setSetpoint(AXIS::PITCH, INPUT::IMU_POS, 0);
  pm()->setSetpoint(AXIS::HEAVE, INPUT::DEPTH, -1.5);
  pm()->updatePlantState(AXIS::YAW);
  pm()->updatePlantState(AXIS::HEAVE);
  pm()->updatePlantState(AXIS::ROLL);
  pm()->updatePlantState(AXIS::PITCH);
  pm()->setControlEffort(AXIS::SURGE, 0);
  pm()->setControlEffort(AXIS::SWAY, 0);
  self()->queueEnable();
  QUEUE_STATE(Scan);
}

void Buoy::Scan::run() {
  pm()->setSetpoint(AXIS::YAW, INPUT::IMU_POS, 0);
  pm()->updatePlantState(AXIS::YAW);
  AUV_INFO("Scanning for Red Buoy");
  if(pm()->isDetected(OBJECT::BUOY_RED)){
    AUV_INFO("Detected Red Buoy. Using vision to move to depth.");
    self()->queueEnable();
    QUEUE_STATE(Heave);
  }
  else{
    if(self()->scanIterator < 3){
      self()->scanAngle = (self()->scanIterator + 1) * 10;
    }
    else if(self()->scanIterator >= 3 & self()->scanIterator <6){
      self()->scanAngle = (self()->scanIterator - 4) * 10;
    }
  }

  pm()->setSetpoint(AXIS::YAW, INPUT::IMU_POS, self()->scanAngle);
  pm()->updatePlantState(AXIS::YAW);
  if(pm()->isPidStable(AXIS::HEAVE, 0.1, 2)){
    AUV_INFO("Scanned to angle %f. Angle is %f", self()->scanAngle, pm()->getYaw());
    self()->scanIterator++;
  }
  pm()->updatePlantState(AXIS::HEAVE);
  pm()->updatePlantState(AXIS::ROLL);
  pm()->updatePlantState(AXIS::PITCH);
}

void Buoy::Heave::run() {
  self()->checkForBuoy();
  pm()->setSetpoint(AXIS::HEAVE, INPUT::CAM_FRONT, 240);
  if(pm()->isPidStable(AXIS::YAW, 20, 2)){
    self()->queueEnable();
    QUEUE_STATE(Sway);
  }
  pm()->setPlantState(AXIS::HEAVE, pm()->getCenterPointY(OBJECT::BUOY_RED));

}

void Buoy::Sway::run() {
  self()->checkForBuoy();
  pm()->setEnabled(AXIS::SWAY, 1);
  pm()->setSetpoint(AXIS::HEAVE, INPUT::CAM_FRONT, 240);
  pm()->setSetpoint(AXIS::SWAY, INPUT::CAM_FRONT, 320);
  if(pm()->isPidStable(AXIS::YAW, 20, 2)){
    self()->queueEnable();
    QUEUE_STATE(Sway);
  }
  pm()->setPlantState(AXIS::HEAVE, pm()->getCenterPointY(OBJECT::BUOY_RED));
  pm()->setPlantState(AXIS::SWAY, pm()->getCenterPointX(OBJECT::BUOY_RED));
}

void Buoy::YawDrive::run() {
  self()->checkForBuoy();
  pm()->setSetpoint(AXIS::HEAVE, INPUT::CAM_FRONT, 240);
  pm()->setSetpoint(AXIS::SWAY, INPUT::CAM_FRONT, 320);
  pm()->setSetpoint(AXIS::YAW, INPUT::CAM_FRONT, 320);
  if(pm()->isPidStable(AXIS::YAW, 20, 2)){
    self()->queueEnable();
    QUEUE_STATE(Sway);
  }
  pm()->setPlantState(AXIS::HEAVE, pm()->getCenterPointY(OBJECT::BUOY_RED));
  pm()->setPlantState(AXIS::SWAY, pm()->getCenterPointX(OBJECT::BUOY_RED));
  pm()->setPlantState(AXIS::YAW, pm()->getCenterPointX(OBJECT::BUOY_RED));
  pm()->setControlEffort(AXIS::SURGE, 40);
}

void Buoy::BackScan::run() {
  pm()->setEnabled(AXIS::HEAVE, 1);//enable all the others
  pm()->setEnabled(AXIS::YAW, 1);
  pm()->setEnabled(AXIS::ROLL, 1);
  pm()->setEnabled(AXIS::PITCH, 1);
  pm()->setEnabled(AXIS::SWAY, 0);
  self()->checkForBuoy();
  int holdDepth = pm()->getDepth();
  int holdYaw = pm()->getYaw();
  pm()->setControlEffort(AXIS::SWAY, 0);
  pm()->setSetpoint(AXIS::HEAVE, INPUT::DEPTH, holdDepth);
  pm()->setSetpoint(AXIS::YAW, INPUT::IMU_POS, holdYaw);
  pm()->setControlEffort(AXIS::SURGE, -15);
  if(pm()->isDetected(OBJECT::BUOY_RED)){
    self()->queueEnable();
    QUEUE_STATE(Heave);
  }
}

void Buoy::checkForBuoy(){
  if(!pm()->isDetected(OBJECT::BUOY_RED)){
    self()->queueEnable();
    QUEUE_STATE(BackScan);
  }
}
}
