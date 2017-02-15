#include <auv_mission_control/PidManager.hpp>

PidManager::PidManager() {
  surge_ = Axis("surge");
  surge_.loadParams();

  sway_ = Axis("sway");
  sway_.loadParams();

  heave_ = Axis("heave");
  heave_.loadParams();

  roll_ = Axis("roll");
  roll_.loadParams();

  pitch_ = Axis("pitch");
  pitch_.loadParams();

  yaw_ = Axis("yaw");
  yaw_.loadParams();
  yaw_.setLimit(180);
}

PidManager::~PidManager() {}

Axis *PidManager::getAxis(AXIS axis) {
  switch (axis) {
  case AXIS::SURGE:
    return &surge_;
  case AXIS::SWAY:
    return &sway_;
  case AXIS::HEAVE:
    return &heave_;
  case AXIS::ROLL:
    return &roll_;
  case AXIS::PITCH:
    return &pitch_;
  case AXIS::YAW:
    return &yaw_;
  }
}

// void PidManager::updateParams(AXIS axis) {
//    getAxis(axis)->updateParams();
// }

void PidManager::setControlEffort(AXIS axis, double speed) {
  getAxis(axis)->setControlEffort(speed);
}

void PidManager::setSetpoint(AXIS axis, INPUT input, double setpoint_value) {
  getAxis(axis)->setSetpoint(input, setpoint_value);
}

void PidManager::setPlantState(AXIS axis, double plant_value) {
  getAxis(axis)->setPlantState(plant_value);
}

void PidManager::setZero(AXIS axis) { getAxis(axis)->setZero(); }

void PidManager::setEnabled(AXIS axis, bool enabled) {
  getAxis(axis)->setEnabled(enabled);
}

double PidManager::getPlantState(AXIS axis) {
  return getAxis(axis)->getPlantState();
}

double PidManager::getLimitedPlantState(AXIS axis) {
  return getAxis(axis)->getLimitedPlantState();
}

double PidManager::getYaw() { return yaw_.getLimitedPlantState(); }

double PidManager::getDepth() { return is_sub_depth_called_ ? depth_ : 0; }

bool PidManager::getStart() { return start_switch_; }

bool PidManager::getKill() { return kill_switch_; }

// bool PidManager::getTimeout() { re}

void PidManager::callbackImu(const sensor_msgs::Imu::ConstPtr &msg_imu) {
  yaw_.plant_state_current_ = msg_imu->orientation.z;
  is_sub_imu_called_ = true;
}

void PidManager::callbackDepth(const std_msgs::Float64::ConstPtr &msg_depth) {
  depth_ = msg_depth->data;
  is_sub_depth_called_ = true;
}

void PidManager::callbackStartSwitch(const std_msgs::Bool::ConstPtr &start) {
  start_switch_ = start->data;
}

void PidManager::callbackKillSwitch(const std_msgs::Bool::ConstPtr &kill) {
  kill_switch_ = kill->data;
}
