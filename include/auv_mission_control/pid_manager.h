#ifndef PID_MANAGER_H
#define PID_MANAGER_H
#include <string>
#include "ros/ros.h"
#include "std_msgs/String.h"
#include "std_msgs/Float64.h"
#include "std_msgs/Bool.h"
#include <sstream>
#include <auv_motor_control/pid_enable.h>
#include <auv_mission_control/axes.h>
#include <dynamic_reconfigure/DoubleParameter.h>
#include <dynamic_reconfigure/Reconfigure.h>
#include <dynamic_reconfigure/Config.h>
#include <auv_mission_control/pid_parameters.h>

//Axis definitions
#define AXIS_SURGE 0
#define AXIS_SWAY 1
#define AXIS_HEAVE 2
#define AXIS_ROLL 3
#define AXIS_PITCH 4
#define AXIS_YAW 5

//INput type definitions

#define INPUT_CAM_FRONT 0
#define INPUT_CAM_BTM 1
#define INPUT_IMU_POS 2
#define INPUT_IMU_VEL 3
#define INPUT_DEPTH 4





class Pid_Manager{
private:
  double plant_surge;
  double plant_sway;
  double plant_heave;
  double plant_roll;
  double plant_pitch;
  double plant_yaw;
  double depth;
  ros::NodeHandle* nh;



public:
  void setpoint_set(int axis, int input_type, double value);
  void plantState_get(int axis);
  double getDepth();
  void zero(int sensor);
  void pidInit_all();
  void pidEnable(int axis, bool enabled);
  void onlyPID_set(bool state);
  ~Pid_Manager();
  Pid_Manager(ros::NodeHandle nh);

};


#endif
