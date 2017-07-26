#include <auv_mission_control/TemplateStates.hpp>
#include <auv_mission_control/task/Gate.hpp>

namespace Task {

int Gate::execute() {
  if (checkEventQueue()) {
    sm_->run();
  }
}

void Gate::Init::run() {
  pm()->setEnabled(AXIS::SURGE, 0);
  pm()->setEnabled(AXIS::SWAY, 0);
  pm()->setEnabled(AXIS::HEAVE, 1);
  pm()->setEnabled(AXIS::YAW, 1);
}

void Gate::GoToDepth::run() {
  pm()->setSetpoint(AXIS::YAW, INPUT::IMU::POS, 0); //don't you turn
  pm()->updatePlantState(AXIS::YAW);//updates the controller state
  pm()->setSetpoint(AXIS::HEAVE, INPUT::DEPTH, 1);//go down to 1m
  pm()->updatePlantState(AXIS::HEAVE);//sets current controller state to depth
  if(pm()->isPidStable(AXIS::SURGE, 0.1, 2)){
    ROS_INFO("Stable at depth of %f", pm()->getDepth());
    self()->queueEnable();
    QUEUE_STATE(DriveForwards);
  }
}

void Gate::DriveForwards::run() {
  pm()->updatePlantState(AXIS::YAW);
  pm()->setControlEffort(AXIS::SURGE, 15);
  timer = ros::Time::now().toSec();
  if(timer >= 25){
    self()->queueEnable();
    QUEUE_STATE(SwitchTask)
  }
}

void Gate::SwitchTask::run() {
  ph()->queueTask("Kill");
}
}
