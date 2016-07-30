#ifndef STATE_MACHINE_H
#define STATE_MACHINE_H

#include <ros/ros.h>
#include <std_msgs/String.h>
#include <std_msgs/Float64.h>
#include <sstream>
#include <math.h>

#include <auv_mission_control/PidManager.h>
#include <auv_mission_control/TaskGate.h>
#include <auv_mission_control/TaskBuoy.h>
#include <auv_mission_control/TaskVision.h>
#include <auv_mission_control/Camera.h>


#define succeeded 0
#define timeout 1
#define kill 2


bool getTimeout();



#endif
