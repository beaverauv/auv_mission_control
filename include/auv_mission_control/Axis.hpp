#ifndef AXIS_H_
#define AXIS_H_

#include <sstream>
#include <string>
#include <vector>

#include <dynamic_reconfigure/Config.h>
#include <dynamic_reconfigure/DoubleParameter.h>
#include <dynamic_reconfigure/Reconfigure.h>
#include <ros/ros.h>
#include <sensor_msgs/Imu.h>
#include <std_msgs/Bool.h>
#include <std_msgs/Float64.h>
#include <std_msgs/Int32.h>
#include <std_msgs/String.h>

#include <auv_mission_control/Task.hpp>

class PidParam {
public:
  double kP;
  double kD;
  double kI;
  double kP_scale;
  double kD_scale;
  double kI_scale;
  void print() {
    ROS_INFO("kP: %f", kP);
    ROS_INFO("kD: %f", kD);
    ROS_INFO("kI: %f", kI);
    ROS_INFO("kP_scale: %f", kP_scale);
    ROS_INFO("kD_scale: %f", kD_scale);
    ROS_INFO("kI_scale: %f", kI_scale);
  }
};

class Axis {
public:
  Axis();
  Axis(std::string axis_name);
  ~Axis();

  PidParam *getPid(INPUT input);

  void printName();

  void printPids();

  void Init();

  void loadParam(INPUT input, std::vector<double> values);

  void loadParams();

  void updateParams(PidParam pid);

  void setControlEffort(double speed);

  void setSetpoint(INPUT input, double setpoint);

  void setPlantState(double plant_value);

  void setPlantStateVal(double plant_value);

  void setEnabled(bool enabled);

  void setZero();

  void setZeroTo(double zero_value);

  void setLimit(double limit);

  void setPidFirstRun(bool pid_first_run);

  void resetPidFirstRun();

  double getPlantState();

  double getLimitedPlantState();

  double getZero();

  double getSetpoint();

  bool isPidStable(int deadband, int wait_time);

  double plant_state_zero_ = 0;
  double plant_state_current_ = 0;
  double plant_state_limit_ = 0;

  double setpoint_current_ = 0;

  bool pid_first_run_ = true;
  double pid_start_time_ = 0;

private:
  std::string axis_name_;
  ros::NodeHandle nh_;

  ros::Publisher pub_setpoint_;
  ros::Publisher pub_plant_state_;
  ros::Publisher pub_effort_;
  ros::Publisher pub_enabled_;

  std_msgs::Float64 msg_setpoint_;
  std_msgs::Float64 msg_plant_state_;
  std_msgs::Float64 msg_effort_;

  std_msgs::Bool msg_enabled_;

  INPUT last_setpoint_input_;

  PidParam pid_cam_front_, pid_cam_bottom_, pid_imu_pos_, pid_imu_vel_,
      pid_depth_;
};

#endif
