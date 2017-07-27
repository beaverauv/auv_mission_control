#include <auv_mission_control/BackplaneManager.hpp>

BackplaneManager::BackplaneManager() {
  sub_start_switch_ =
      nh_.subscribe("/backplane/startswitch", 1,
                    &BackplaneManager::callbackStartSwitch, this);
}

BackplaneManager::~BackplaneManager() {}

void BackplaneManager::setBackplaneState(int state) {
  // TODO implement this
}

void BackplaneManager::setBuoyOrder(int order) {
  // TODO implement this
}

bool BackplaneManager::getStartSwitchState() { return start_switch_; }

void BackplaneManager::callbackStartSwitch(
    const std_msgs::Bool::ConstPtr &start) {
  start_switch_ = start->data;
}
