#include <auv_mission_control/StateMachine.h>

Timer mainTimer;
bool timerStarted = false;
double timeSinceStart;

int main(int argc, char **argv){

bool killSwitch = 0;
bool startSwitch = 0;
int currentState = 0; //init
int initCount = 0;
int gateCount = 0;
double timeSinceStart;
ros::init(argc, argv, "state_machine");
ros::NodeHandle nh;


PidManager pm(&nh);
Camera cam;
ROS_ERROR("here");
  while(ros::ok){ //careful

  if(timerStarted)
    timeSinceStart = mainTimer.getTime();

    ros::spinOnce();
    switch(currentState){
      case 0: {//init
        if(initCount < 1){
          ROS_INFO("EXECUTING INIT STATE");
          initCount++;
        }
        //calibrate sensors
        if(killSwitch){
          currentState = 9;
        }
        else if(pm.getStart() == true){
          currentState = 1;
          ROS_INFO("start");
          mainTimer.start();
          timerStarted = true;
        }
        else
          currentState = 0;
        break;
      }


<<<<<<< HEAD:src/StateMachine.cpp
      case 1: { //gate
=======
      }
      case 2: { //gate

        TaskGate gate(&pm, &cam);
        int outcome = gate.execute();
>>>>>>> aecae700d55f4c671ffd932ac9a8c9c3b7a29d3d:src/StateMachine.cpp
        ROS_INFO("EXECUTING GATE TASK");
        TaskGate gate(&pm, &cam);
        int outcome = gate.execute();
        ROS_INFO("outcome %d", outcome);

        if (outcome == succeeded)
          currentState = 3;
        else if (outcome == timeout || getTimeout())
          currentState = 8;
        else if (outcome == kill || pm.getKill()){
          currentState = 9;
          ROS_INFO("kill");
        }
        else
          currentState = 9;
        break;

      }

      case 9:{ //kill
        //stop thrusters (in resource file)
        break;
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
