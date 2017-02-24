#include <auv_mission_control/PidManager.hpp>

PidManager::PidManager() {
  sub_imu_ = nh_.subscribe("/imu/imu", 1, &PidManager::callbackImu, this);
  sub_depth_ = nh_.subscribe("/depth", 1, &PidManager::callbackDepth, this);
  sub_start_switch_ =
      nh_.subscribe("/start", 1, &PidManager::callbackStartSwitch, this);
  sub_kill_switch_ =
      nh_.subscribe("/kill", 1, &PidManager::callbackKillSwitch, this);

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
  // ROS_INFO("setpoint called AXIS: %d, INPUT: %d, setpoint_value: %f",
  // (int)axis,
  //          (int)input, setpoint_value);
  getAxis(axis)->setSetpoint(input, setpoint_value);
}

void PidManager::setPlantState(AXIS axis, double plant_value) {
  getAxis(axis)->setPlantState(plant_value);
}

void PidManager::setZero(AXIS axis) { getAxis(axis)->setZero(); }

void PidManager::setEnabled(AXIS axis, bool enabled) {
  getAxis(axis)->setEnabled(enabled);
}

void PidManager::updatePlantState(AXIS axis) {
  // ROS_INFO("update plant state called AXIS: %d", (int)axis);

  switch (axis) {
  case AXIS::SURGE:
    setPlantState(axis, velocity_surge_);
    break;
  case AXIS::SWAY:
    setPlantState(axis, velocity_sway_);
    break;
  case AXIS::HEAVE:
    setPlantState(axis, getDepth());
    break;
  case AXIS::ROLL:
    setPlantState(axis, position_roll_);
    break;
  case AXIS::PITCH:
    setPlantState(axis, position_pitch_);
    break;
  case AXIS::YAW:
    setPlantState(axis, getYaw());
    break;
  }
}

double PidManager::getPlantState(AXIS axis) {
  return getAxis(axis)->getPlantState();
}

double PidManager::getLimitedPlantState(AXIS axis) {
  return getAxis(axis)->getLimitedPlantState();
}

double PidManager::getSetpoint(AXIS axis) {
  return getAxis(axis)->getSetpoint();
}

double PidManager::getYaw() { return yaw_.getLimitedPlantState(); }

double PidManager::getDepth() {
  return is_sub_depth_called_ ? position_heave_ : 0;
}

bool PidManager::getStart() { return start_switch_; }

bool PidManager::getKill() { return kill_switch_; }

void PidManager::ensureDepth() {
  if (should_ensure_depth_)
    updatePlantState(AXIS::HEAVE);
}

void PidManager::ensureYaw() {
  if (should_ensure_yaw_)
    updatePlantState(AXIS::YAW);
}

void PidManager::startEnsuringDepth() { should_ensure_depth_ = true; }

void PidManager::startEnsuringYaw() { should_ensure_yaw_ = true; }

void PidManager::stopEnsuringDepth() { should_ensure_depth_ = false; }

void PidManager::stopEnsuringYaw() { should_ensure_yaw_ = false; }

// bool PidManager::getTimeout() { re}

void PidManager::callbackImu(const sensor_msgs::Imu::ConstPtr &msg_imu) {
  yaw_.plant_state_current_ = msg_imu->orientation.z;
  velocity_surge_ = msg_imu->linear_acceleration.x;
  velocity_sway_ = msg_imu->linear_acceleration.y;

  position_roll_ = msg_imu->orientation.x;
  position_pitch_ = msg_imu->orientation.y;

  is_sub_imu_called_ = true;
}

void PidManager::callbackDepth(const std_msgs::Float64::ConstPtr &msg_depth) {
  position_heave_ = msg_depth->data;
  is_sub_depth_called_ = true;
}

void PidManager::callbackStartSwitch(const std_msgs::Bool::ConstPtr &start) {
  start_switch_ = start->data;
}

void PidManager::callbackKillSwitch(const std_msgs::Bool::ConstPtr &kill) {
  kill_switch_ = kill->data;
}
