#include <auv_mission_control/Axis.hpp>

Axis::Axis() {}

Axis::Axis(std::string axis_name) : axis_name_(axis_name) {
  pub_setpoint_ = nh_.advertise<std_msgs::Float64>(axis_name_ + "/setpoint", 1);
  pub_plant_state_ =
      nh_.advertise<std_msgs::Float64>(axis_name_ + "/plant_state", 1);
  pub_effort_ =
      nh_.advertise<std_msgs::Float64>(axis_name_ + "/control_effort", 1);
  pub_enabled_ = nh_.advertise<std_msgs::Bool>(axis_name_ + "/enable", 1);
}
Axis::~Axis() {}

PidParam *Axis::getPid(INPUT input) {
  switch (input) {
  case INPUT::CAM_FRONT:
    return &pid_cam_front_;
  case INPUT::CAM_BOTTOM:
    return &pid_cam_bottom_;
  case INPUT::IMU_POS:
    return &pid_imu_pos_;
  case INPUT::IMU_VEL:
    return &pid_imu_vel_;
  case INPUT::DEPTH:
    return &pid_depth_;
  }
}

void Axis::printName() { ROS_INFO("%s", axis_name_.c_str()); }

void Axis::printPids() {
  ROS_INFO("pid_cam_front_");
  pid_cam_front_.print();

  ROS_INFO("pid_cam_bottom_");

  pid_cam_bottom_.print();
  ROS_INFO("pid_imu_pos_");

  pid_imu_pos_.print();
  ROS_INFO("pid_imu_vel_");

  pid_imu_vel_.print();
  ROS_INFO("pid_depth_");

  pid_depth_.print();
}

void Axis::loadParams() {
  std::vector<double> values;
  nh_.getParam(axis_name_ + "/input_cam_front", values);
  loadParam(INPUT::CAM_FRONT, values);

  nh_.getParam(axis_name_ + "/input_cam_bottom", values);
  loadParam(INPUT::CAM_BOTTOM, values);

  nh_.getParam(axis_name_ + "/input_imu_pos", values);
  loadParam(INPUT::IMU_POS, values);

  nh_.getParam(axis_name_ + "/input_imu_vel", values);
  loadParam(INPUT::IMU_VEL, values);

  nh_.getParam(axis_name_ + "/input_depth", values);
  loadParam(INPUT::DEPTH, values);
}

void Axis::loadParam(INPUT input, std::vector<double> values) {
  PidParam *pid = getPid(input);
  pid->kP = values.at(0);
  pid->kD = values.at(1);
  pid->kI = values.at(2);
  pid->kP_scale = values.at(3);
  pid->kD_scale = values.at(4);
  pid->kI_scale = values.at(5);
}

void Axis::updateParams(PidParam pid) {
  dynamic_reconfigure::ReconfigureRequest srv_req;
  dynamic_reconfigure::ReconfigureResponse srv_resp;
  dynamic_reconfigure::DoubleParameter double_param;
  dynamic_reconfigure::Config conf;

  double_param.name = "Kp";
  double_param.value = pid.kP;
  conf.doubles.push_back(double_param);

  double_param.name = "Ki";
  double_param.value = pid.kI;
  conf.doubles.push_back(double_param);

  double_param.name = "Kd";
  double_param.value = pid.kD;
  conf.doubles.push_back(double_param);
  srv_req.config = conf;

  double_param.name = "Kp_scale";
  double_param.value = pid.kP_scale;
  conf.doubles.push_back(double_param);

  double_param.name = "Kd_scale";
  double_param.value = pid.kD_scale;
  conf.doubles.push_back(double_param);

  double_param.name = "Ki_scale";
  double_param.value = pid.kI_scale;
  conf.doubles.push_back(double_param);

  ros::service::call("/" + axis_name_ + "/set_parameters", srv_req, srv_resp);
}

void Axis::setControlEffort(double speed) {
  msg_effort_.data = speed;
  pub_effort_.publish(msg_effort_);
}

void Axis::setSetpoint(INPUT input, double setpoint) {
  setpoint_current_ = setpoint;
  updateParams(*getPid(input));
  msg_setpoint_.data = setpoint;
  pub_setpoint_.publish(msg_setpoint_);
}

void Axis::setPlantState(double plant_value) {
  msg_plant_state_.data = plant_value;
  // plant_state_current_ = plant_value;
  pub_plant_state_.publish(msg_plant_state_);
}

void Axis::setPlantStateVal(double plant_value) {
  plant_state_current_ = plant_value;
}

void Axis::setEnabled(bool enabled) {
  msg_enabled_.data = enabled;
  pub_enabled_.publish(msg_enabled_);
}

void Axis::setZero() { plant_state_zero_ = plant_state_current_; }

void Axis::setLimit(double limit) { plant_state_limit_ = limit; }

double Axis::getPlantState() { return plant_state_current_; }

double Axis::getLimitedPlantState() {
  double delta = plant_state_zero_ - plant_state_current_;

  if (delta > plant_state_limit_)
    return delta - plant_state_limit_;

  if (delta < -plant_state_limit_)
    return delta + plant_state_limit_;

  return delta;
}

double Axis::getSetpoint() { return setpoint_current_; }
