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

struct pid_parameters {
  double kp;
  double kd;
  double ki;
};

class Pid_Manager{
private:
  double plant_surge;
  double plant_sway;
  double plant_heave;
  double plant_roll;
  double plant_pitch;
  double plant_yaw;
  double depth;

  ros::NodeHandle nh_;
  ros::Publisher setpoint_surge_pub;
  ros::Publisher setpoint_sway_pub;
  ros::Publisher setpoint_heave_pub;
  ros::Publisher setpoint_roll_pub;
  ros::Publisher setpoint_pitch_pub;
  ros::Publisher setpoint_yaw_pub;

  //plant state publishers
  ros::Publisher state_surge_pub;
  ros::Publisher state_sway_pub;
  ros::Publisher state_heave_pub;
  ros::Publisher state_roll_pub;
  ros::Publisher state_pitch_pub;
  ros::Publisher state_yaw_pub;

  //enable publishers
  ros::Publisher surge_enable_pub;
  ros::Publisher sway_enable_pub;
  ros::Publisher heave_enable_pub;
  ros::Publisher yaw_enable_pub;


  ros::Publisher control_effort_pub;
  
  double timeSinceStart;
  bool startSwitch;
  bool killSwitch;
  bool timeout;



public:

  Pid_Manager();
  Pid_Manager(ros::NodeHandle* nh);

  ~Pid_Manager();

  void setpoint_set(int axis, int input_type, double value);
  void plantState_get(int axis);
  void depth_callBack(const std_msgs::Float64::ConstPtr& depth_msg);
  void start_callBack(const std_msgs::Bool::ConstPtr& start_msg);
  void kill_callBack(const std_msgs::Bool::ConstPtr& kill_msg);
  void setPlantState(int axis, double plantValue);
  void setCamera(int camera); //choose which camera is in use
  double getDepth();
  void zero(int sensor);
  void pidInit_all();
  void pidEnable(int axis, bool enabled);
  bool getTimeout();
  bool getStart();
  bool getKill();
  void controlEffort_set(int speed); //manually set controlEffort (surge only), must disable PID first


};


#endif
