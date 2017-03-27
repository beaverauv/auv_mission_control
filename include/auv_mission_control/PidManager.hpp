#ifndef PidManager_H
#define PidManager_H

#include <sstream>
#include <string>

#include <dynamic_reconfigure/Config.h>
#include <dynamic_reconfigure/DoubleParameter.h>
#include <dynamic_reconfigure/Reconfigure.h>
#include <ros/ros.h>
#include <sensor_msgs/Imu.h>
#include <std_msgs/Bool.h>
#include <std_msgs/Float64.h>
#include <std_msgs/Int32.h>
#include <std_msgs/String.h>

#include <auv_mission_control/Axis.hpp>
#include <auv_mission_control/task/Task.hpp>

// Axis definitions
#define AXIS_SURGE 0
#define AXIS_SWAY 1
#define AXIS_HEAVE 2
#define AXIS_ROLL 3
#define AXIS_PITCH 4
#define AXIS_YAW 5

// INput type definitions

#define INPUT_CAM_FRONT 0
#define INPUT_CAM_BTM 1
#define INPUT_IMU_POS 2
#define INPUT_IMU_ACCEL 3
#define INPUT_DEPTH 4
// #define IMU_YAW 5

class PidManager : public TaskBase {
public:
  PidManager();
  ~PidManager();

  std::string getTag() { return std::string("[PidManager]"); }

  ros::NodeHandle nh_;

  Axis *getAxis(AXIS axis);

  std::string getAxisName(AXIS axis);

  void setControlEffort(AXIS axis, double speed);

  void setSetpoint(AXIS axis, INPUT input, double setpoint_value);

  void setPlantState(AXIS axis, double plant_value);

  void setZero(AXIS axis);

  void setEnabled(AXIS axis, bool enabled);

  void updatePlantState(AXIS axis);

  double getPlantState(AXIS axis);

  double getLimitedPlantState(AXIS axis);

  double getSetpoint(AXIS axis);

  double getYaw();

  double getDepth();

  bool isStarted();

  bool isKilled();

  bool isImuCalled();

  void ensureDepth();

  void ensureYaw();

  void startEnsuringDepth();

  void startEnsuringYaw();

  void stopEnsuringDepth();

  void stopEnsuringYaw();

  // bool getTimeout();

  void callbackImu(const sensor_msgs::Imu::ConstPtr &imu_msg);

  void callbackDepth(const std_msgs::Float64::ConstPtr &msg_depth);

  void callbackStartSwitch(const std_msgs::Bool::ConstPtr &msg_start_switch);

  void callbackKillSwitch(const std_msgs::Bool::ConstPtr &msg_kill_switch);

private:
  ros::Subscriber sub_imu_;
  ros::Subscriber sub_depth_;
  ros::Subscriber sub_start_switch_;
  ros::Subscriber sub_kill_switch_;

  double position_heave_, velocity_surge_, velocity_sway_;
  double position_roll_, position_pitch_, position_yaw_;

  bool should_ensure_depth_;
  bool should_ensure_yaw_;

  bool is_sub_imu_called_ = false;
  bool is_sub_depth_called_ = false;

  bool start_switch_ = false;
  bool kill_switch_ = false;

  Axis surge_, sway_, heave_;
  Axis roll_, pitch_, yaw_;
};

#endif
