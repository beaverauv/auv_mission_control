#include "ros/ros.h"
#include "std_msgs/String.h"
#include "std_msgs/Float64.h"
#include <sstream>
#include "math.h"
#include "auv_mission_control/pid_manager.h"
#include "auv_mission_control/task_gate.h"
#include <auv_mission_control/state_machine.h>


int main(int argc, char **argv){

bool killSwitch = 0;
bool startSwitch;
int currentState = 0; //init
ros::init(argc, argv, "state_machine");
ros::NodeHandle nh;

Pid_Manager pm(&nh);

  while(ros::ok){ //careful

    switch(currentState){
      case 0: {//init
        //calibrate sensors
        if(killSwitch){
          currentState = 9;
        }
        if(startSwitch){
          currentState = 1;
        }
        break;
      }

      case 1:{ //go to depth

      }
      case 2: { //gate
        Task_Gate gate(&pm);
        int outcome = gate.execute();

        if (outcome == succeeded)
          currentState = 3;
        else if (outcome == timeout || getTimeout())
          currentState = 8;
        else if (outcome == kill)
          currentState = 9;
        else
          currentState = 9;

      }

      case 9:{ //kill
        //stop thrusters (in resource file)
      }

      default:
        break;//NO

    }

  }

}

void startTimer(){
  startTime = ros::Time::now();
}

ros::Duration timeSinceStart(){
  return ros::Time::now() - startTime;
}

bool getTimeout(){
  if (timeSinceStart <= 0)
    return true;
  else
    return false;
}
