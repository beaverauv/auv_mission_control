#include <auv_mission_control/TemplateStates.hpp>
#include <auv_mission_control/task/Gate.hpp>

namespace Task {

int Gate::execute() {
  pm()->ensureDepth(); //always make sure at depth, yaw, pitch, and roll
  pm()->ensureYaw();
  pm()->updatePlantState(AXIS::PITCH);
  pm()->updatePlantState(AXIS::ROLL);

  if (checkEventQueue()) {
    sm_->run();//run the thing
  }
}

void Gate::Init::run() { // initialization sequence
  pm()->setZero(AXIS::YAW);
  self()->sketchZero = pm()->getZero(AXIS::YAW);
  AUV_INFO("zero set to:%f, actual zero is %f, current value is %f", self()->sketchZero, pm()->getZero(AXIS::YAW), pm()->getYaw());
  AUV_INFO("INit");
  pm()->setEnabled(AXIS::SURGE, 0);//disable surge/sway pid loops
  pm()->setEnabled(AXIS::SWAY, 0);
  pm()->setEnabled(AXIS::HEAVE, 1);//enable all the others
  pm()->setEnabled(AXIS::YAW, 1);
  pm()->setEnabled(AXIS::ROLL, 1);
  pm()->setEnabled(AXIS::PITCH, 1);
  pm()->setOtherZero(AXIS::HEAVE); //zero the depth
  self()->queueEnable();//enable task queue
  QUEUE_STATE(GoToDepth);//switch to substate that takes sub to depth
}

void Gate::GoToDepth::run() { //go to depth
  AUV_INFO("depthing, depth is %f", pm()->getDepth());
  AUV_INFO("Zero is %f", pm()->getZero(AXIS::YAW));
  pm()->setSetpoint(AXIS::YAW, INPUT::IMU_POS, 0); // don't you turn
  pm()->updatePlantState(AXIS::YAW); // updates the controller state
  pm()->setSetpoint(AXIS::HEAVE, INPUT::DEPTH, -0.4); // go down to 1m
  pm()->updatePlantState(AXIS::HEAVE); // sets current controller state to depth
  pm()->setSetpoint(AXIS::ROLL, INPUT::IMU_POS, 0);
  pm()->setSetpoint(AXIS::PITCH, INPUT::IMU_POS, 0);
  pm()->updatePlantState(AXIS::PITCH);
  pm()->updatePlantState(AXIS::ROLL);
  self()->queueEnable();
  //QUEUE_ACTION(Timer, 10.0);
  //QUEUE_STATE(DriveForwards);
  if(pm()->isPidStable(AXIS::HEAVE, 0.3, 2)){
    AUV_INFO("Stable at depth of %f", pm()->getDepth());
    self()->queueEnable();
    QUEUE_STATE(DriveForwards);
  }
}

void Gate::DriveForwards::run() {
  if(self()->firstRun){
    timerStart = ros::Time::now().toSec();
  }
  AUV_INFO("here");
  AUV_INFO("Zero is %f", pm()->getZero(AXIS::YAW));
  pm()->setSetpoint(AXIS::YAW, INPUT::IMU_POS, 0); // don't you turn
  pm()->updatePlantState(AXIS::YAW);
  pm()->updatePlantState(AXIS::ROLL);
  pm()->updatePlantState(AXIS::PITCH);
  pm()->setControlEffort(AXIS::SURGE, 120);
  if(ros::Time::now().toSec() - timerStart >= 28){
    self()->queueEnable();
    QUEUE_STATE(SwitchTask);
  }

}

void Gate::SwitchTask::run() {
  AUV_INFO("DONE DRIVING");
  pm()->setControlEffort(AXIS::SURGE, 0);
  pm()->setEnabled(AXIS::YAW, 0);
  pm()->setEnabled(AXIS::ROLL, 0);
  pm()->setEnabled(AXIS::PITCH, 0);
  pm()->setControlEffort(AXIS::YAW, 0);
  pm()->setControlEffort(AXIS::PITCH, 0);
  pm()->setControlEffort(AXIS::ROLL, 0);
  pm()->updatePlantState(AXIS::HEAVE);
  self()->queueEnable();
  AUV_INFO("Switching to Buoy task");
 // ph()->queueTask("Buoy");
}
}
