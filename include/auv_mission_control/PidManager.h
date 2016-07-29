#ifndef PidManager_H
#define PidManager_H

#include <string>
#include <sstream>

#include <ros/ros.h>
#include <std_msgs/String.h>
#include <std_msgs/Float64.h>
#include <std_msgs/Int32.h>
#include <std_msgs/Bool.h>
#include <sensor_msgs/Imu.h>
#include <dynamic_reconfigure/DoubleParameter.h>
#include <dynamic_reconfigure/Reconfigure.h>
#include <dynamic_reconfigure/Config.h>

#include <auv_motor_control/pid_enable.h>
#include <auv_mission_control/axes.h>
#include <auv_mission_control/Timer.h>

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


class PidParam {
public:
  double kP;
  double kD;
  double kI;
  double Kp_scale;
  double Kd_scale;
  double Ki_scale;
};

class PidManager{

private:

  ros::NodeHandle nh_;

  ros::Subscriber subDepth;
  bool subDepthHasBeenCalled = false;
  ros::Subscriber subStart;
  bool subStartHasBeenCalled = false;
  ros::Subscriber subImu;
//  bool subImuHasBeenCalled = false;
  ros::Subscriber subKill;
  bool subKillHasBeenCalled = false;

  ros::Publisher pubSetpointSurge;
  ros::Publisher pubSetpointSway;
  ros::Publisher pubSetpointHeave;
  ros::Publisher pubSetpointRoll;
  ros::Publisher pubSetpointPitch;
  ros::Publisher pubSetpointYaw;

  //plant state publishers
  ros::Publisher pubStateSurge;
  ros::Publisher pubStateSway;
  ros::Publisher pubStateHeave;
  ros::Publisher pubStateRoll;
  ros::Publisher pubStatePitch;
  ros::Publisher pubStateYaw;

  //enable publishers
  ros::Publisher pubEnableSurge;
  ros::Publisher pubEnableSway;
  ros::Publisher pubEnableHeave;
  ros::Publisher pubEnableYaw;

  ros::Publisher pubEffortSurge;
  ros::Publisher pubEffortSway;
  ros::Publisher pubEffortHeave;
  ros::Publisher pubEffortYaw;

  ros::Publisher pubControlEffort;

  bool bStartSwitchState_ = false;
  bool bTimoutSwitchState_ = false;;

  int rosInfoCounter = 0;

  PidParam paramSurge;
  PidParam paramSway;
  PidParam paramHeave;
  PidParam paramYaw;

  int testCount = 0;


public:

  PidManager();
  PidManager(ros::NodeHandle* nh);

  ~PidManager();



  void taskDelay(int seconds);
  void updateParams(int axis);

  void setControlEffort(int axis, int speed); //manually set controlEffort, must disable PID first
  void setSetpoint(int axis, int input_type, double value);
  void setPlantState(int axis, double plantValue);
  void setZero(int sensor);
  void setPidEnabled(int axis, bool enabled);


  void getPlantState(int axis);
  double getDepth();
  bool getStart();
  bool getKill();
  bool getTimeout();

  void depthCallBack(const std_msgs::Float64::ConstPtr& depth_msg);
  void startCallBack(const std_msgs::Bool::ConstPtr& start_msg);
  void killCallBack(const std_msgs::Bool::ConstPtr& kill_msg);
  void imuCallBack(const sensor_msgs::Imu::ConstPtr& imu_msg);

  double getYaw();


};


#endif
