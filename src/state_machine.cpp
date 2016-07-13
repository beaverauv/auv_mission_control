#include "ros/ros.h"
#include "std_msgs/String.h"
#include "std_msgs/Float64.h"
#include <sstream>
#include "math.h"


int main(int argc, char **argv){

bool killSwitch = 0;
int currentState = 0; //init


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


      }

      case 9:{ //kill
        //stop thrusters (in resource file)
      }

      default:
        break;//NO

    }

  }
