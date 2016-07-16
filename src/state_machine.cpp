#include "ros/ros.h"
#include "std_msgs/String.h"
#include "std_msgs/Float64.h"
#include <sstream>
#include "math.h"
#include "auv_mission_control/pid_manager.h"
#include "auv_mission_control/task_gate.h"

int main(int argc, char **argv){

bool killSwitch = 0;
bool startSwitch;
int currentState = 0; //init

Pid_Manager pm;

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

      case 1: { //gate
        Task_Gate gate(pm);
        gate.excecute();

      }

      case 9:{ //kill
        //stop thrusters (in resource file)
      }

      default:
        break;//NO

    }

  }
}
