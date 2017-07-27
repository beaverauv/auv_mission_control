#include <auv_mission_control/TemplateStates.hpp>
#include <auv_mission_control/task/Gate.hpp>

namespace Task {

int Gate::execute() {

  pm()->ensureDepth();
  pm()->ensureYaw();
  pm()->updatePlantState(AXIS::PITCH);
  pm()->updatePlantState(AXIS::ROLL);

  if (checkEventQueue()) {
    sm_->run();
  }
}

void Gate::Init::run() {
  pm()->setEnabled(AXIS::SURGE, 0);
  pm()->setEnabled(AXIS::SWAY, 0);
  pm()->setEnabled(AXIS::HEAVE, 1);
  pm()->setEnabled(AXIS::YAW, 1);
  pm()->setEnabled(AXIS::ROLL, 1);
  pm()->setEnabled(AXIS::PITCH, 1);
  for (int i = 0; i < 10; i++) {
    pm()->setZero(AXIS::YAW);
    AUV_INFO("Zerod yaw at %f", pm()->getYaw());
  }
  self()->queueEnable();
  QUEUE_STATE(GoToDepth);
}

void Gate::GoToDepth::run() {
  AUV_INFO("depthing");
  pm()->setSetpoint(AXIS::YAW, INPUT::IMU_POS, 0); // don't you turn
  pm()->updatePlantState(AXIS::YAW); // updates the controller state
  pm()->setSetpoint(AXIS::HEAVE, INPUT::DEPTH, -0.7); // go down to 1m
  pm()->updatePlantState(AXIS::HEAVE); // sets current controller state to depth
  pm()->setSetpoint(AXIS::ROLL, INPUT::IMU_POS, 0);
  pm()->setSetpoint(AXIS::PITCH, INPUT::IMU_POS, 0);
  pm()->updatePlantState(AXIS::PITCH);
  pm()->updatePlantState(AXIS::ROLL);
  self()->queueEnable();
  QUEUE_ACTION(Timer, 10.0);
  QUEUE_STATE(DriveForwards);
}

void Gate::DriveForwards::run() {
  AUV_INFO("DRIVING");
  pm()->setSetpoint(AXIS::YAW, INPUT::IMU_POS, 0); // don't you turn
  pm()->updatePlantState(AXIS::YAW);
  pm()->updatePlantState(AXIS::ROLL);
  pm()->updatePlantState(AXIS::PITCH);
  pm()->setControlEffort(AXIS::SURGE, 80);
  self()->queueEnable();
  QUEUE_ACTION(Timer, 100.0);
  QUEUE_STATE(SwitchTask);

}

void Gate::SwitchTask::run() {
  pm()->setControlEffort(AXIS::SURGE, 0);
  pm()->setEnabled(AXIS::YAW, 0);
  pm()->setEnabled(AXIS::ROLL, 0);
  pm()->setEnabled(AXIS::PITCH, 0);
  pm()->setEnabled(AXIS::HEAVE, 0);
  pm()->setControlEffort(AXIS::YAW, 0);
  pm()->setControlEffort(AXIS::PITCH, 0);
  pm()->setControlEffort(AXIS::HEAVE, 0);
  pm()->setControlEffort(AXIS::ROLL, 0);




  AUV_INFO("ima just sit here and do nothing forever");
}
}
