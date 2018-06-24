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

ros::Rate stateRate(20);

PidManager pm(&nh);
Camera cam;
TaskVision vision(&cam);

while(ros::ok){ //careful
  if(timerStarted)
  timeSinceStart = mainTimer.getTime();


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
      else if(pm.getStart()){
        currentState = 1;
        ROS_INFO("STARTING");
        mainTimer.start();
        timerStarted = true;
        while(mainTimer.getTime() < 1){
          ros::spinOnce;
          stateRate.sleep();
        }
      }
      else
        currentState = 0;
      break;
    }


    case 1: { //gate
      ROS_INFO("EXECUTING GATE TASK");
      TaskGate gate(&pm, &cam, &vision);
      int outcome = gate.execute();
      // ROS_INFO("outcome %d", outcome);

      if (outcome == succeeded){
        currentState = 2;
        ROS_INFO("Gate task succeeded. Transitioning to Buoy task");
      }
      else if (outcome == timeout)// || getTimeout())
        currentState = 8;
      else if (outcome == kill){
        currentState = 9;
        ROS_INFO("kill");
      }
      else
        currentState = 9;
      break;
    }
    case 2: { //buoy

      ROS_INFO("EXECUTING BUOY TASK");
      TaskBuoy buoy(&pm, &vision);
      //int outcome = 10;
      int outcome =  buoy.execute();
      //ROS_INFO("outcome %d", outcome);
      if (outcome == succeeded){
        currentState = 10;
        ROS_INFO("Buoy task succeeded. Resetting to init state");
      }
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

    case 10:{//prep to go back to init

      if(initCount < 1){
        ROS_INFO("All QUEUED TASKS COMPLETED. RETURN START SWITCH TO RETURN TO INIT STATE.");
        initCount++;
      }
      //calibrate sensors
      if(killSwitch){
        currentState = 9;
      }
      else if(!pm.getStart()){
        currentState = 0;
        initCount = 0;
        ROS_INFO("RETURNING TO INIT STATE");
      }
      else
        currentState = 10;
      break;
    }

    default:
      break;//NO

  }

  ros::spinOnce();
  stateRate.sleep();
  }

}


bool getTimeout(){
if (timeSinceStart <= 15 * 60)
  return true;
else
  return false;
}
