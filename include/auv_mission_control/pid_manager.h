#ifndef PID_MANAGER_H
#define PID_MANAGER_H
#include <string>
#include <ros/ros.h>
#include <std_msgs/String.h>
#include <std_msgs/Float64.h>
#include <std_msgs/Bool.h>
#include <sstream>
#include <auv_motor_control/pid_enable.h>
#include <auv_mission_control/axes.h>
#include <dynamic_reconfigure/DoubleParameter.h>
#include <dynamic_reconfigure/Reconfigure.h>
#include <dynamic_reconfigure/Config.h>
#include <auv_mission_control/single_use_timer.h>
#include <dynamic_reconfigure/DoubleParameter.h>
#include <dynamic_reconfigure/Reconfigure.h>
#include <dynamic_reconfigure/Config.h>
#include <sensor_msgs/Imu.h>



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
#define IMU_YAW 5

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

  ros::Publisher surgeEffort_pub;
  ros::Publisher swayEffort_pub;
  ros::Publisher heaveEffort_pub;
  ros::Publisher yawEffort_pub;

  ros::Publisher control_effort_pub;

  bool startSwitch;
  bool killSwitch;
  bool timeout;

  sensor_msgs::Imu imu_;



  pid_parameters parameters_surge;
  pid_parameters parameters_sway;
  pid_parameters parameters_heave;
  pid_parameters parameters_yaw;



public:

  Pid_Manager();
  Pid_Manager(ros::NodeHandle* nh);

  ~Pid_Manager();


  void pidInit_all();
  void pidEnable(int axis, bool enabled);
  void taskDelay(int seconds);
  void zero(int sensor);
  void updateParameters(int axis);

  void setControlEffort(int axis, int speed); //manually set controlEffort, must disable PID first
  void setSetPoint(int axis, int input_type, double value);
  void setPlantState(int axis, double plantValue);


  void getPlantState(int axis);
  double getDepth();
  bool getStart();
  bool getKill();
  bool getTimeout();

  void depth_callBack(const std_msgs::Float64::ConstPtr& depth_msg);
  void start_callBack(const std_msgs::Bool::ConstPtr& start_msg);
  void kill_callBack(const std_msgs::Bool::ConstPtr& kill_msg);
  void imu_callBack(const sensor_msgs::Imu::ConstPtr& imu_msg);


};


#endif
