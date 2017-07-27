#ifndef BACKPLANE_MANAGER_HPP_
#define BACKPLANE_MANAGER_HPP_

#include <auv_mission_control/Task.hpp>
#include <std_msgs/Bool.h>

class BackplaneManager : public Logger {
public:
  BackplaneManager();
  ~BackplaneManager();

  void setBackplaneState(int state);

  void setBuoyOrder(int order);

  bool getStartSwitchState();

  void callbackStartSwitch(const std_msgs::Bool::ConstPtr &start);

  bool start_switch_ = false;

  ros::Subscriber sub_start_switch_;

  ros::NodeHandle nh_;
};

#endif
