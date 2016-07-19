#include "ros/ros.h"
#include "std_msgs/String.h"
#include "std_msgs/Float64.h"
#include <sstream>
#include "math.h"

#include <auv_mission_control/state_machine.h>

Timer mainTimer;
bool timerStarted = false;
double timeSinceStart;

int main(int argc, char **argv){

bool killSwitch = 0;
bool startSwitch;
int currentState = 0; //init
double timeSinceStart;
ros::init(argc, argv, "state_machine");
ros::NodeHandle nh;


Pid_Manager pm(&nh);
Camera cam;
ROS_ERROR("here");
  while(ros::ok){ //careful

  if(timerStarted)
    timeSinceStart = mainTimer.getTime();

    ros::spinOnce();
    switch(currentState){
      case 0: {//init
        ROS_INFO("EXECUTING INIT STATE");
        //calibrate sensors
        if(killSwitch){
          currentState = 9;
        }
        if(startSwitch){
          currentState = 1;
          mainTimer.start();
          timerStarted = true;
        }
        break;
      }

      case 1:{ //go to depth

      }
      case 2: { //gate

        Task_Gate gate(&pm, &cam);
        int outcome = gate.execute();
        ROS_INFO("EXECUTING GATE TASK");
        if (outcome == succeeded)
          currentState = 3;
        else if (outcome == timeout || getTimeout())
          currentState = 8;
        else if (outcome == kill)
          currentState = 9;
        else
          currentState = 9;
        break;

      }

      case 9:{ //kill
        //stop thrusters (in resource file)
      }

      default:
        break;//NO

    }

  }

}


bool getTimeout(){
  if (timeSinceStart <= 15 * 60)
    return true;
  else
    return false;
}
