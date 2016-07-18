#include "auv_mission_control/pid_manager.h"

/*
void visionPlant_callback(const auv_mission_control::axes::ConstPtr& vision){
  plant_surge_vision = vision->surge;
  plant_sway_vision = vision->sway;
  plant_heave_vision = vision->heave;
  plant_yaw_vision = vision->yaw;
}
*/

void Pid_Manager::depth_callBack(const std_msgs::Float64::ConstPtr& depth_msg){
  depth = depth_msg->data;
}

void Pid_Manager::start_callBack(const std_msgs::Bool::ConstPtr& start_msg){
  startSwitch = start_msg->data;
}

void Pid_Manager::kill_callBack(const std_msgs::Bool::ConstPtr& kill_msg){
  killSwitch = kill_msg->data;
}


Pid_Manager::Pid_Manager(){
}


Pid_Manager::Pid_Manager(ros::NodeHandle* nh) : nh_(*nh){

  //zero all sensors

//   ros::Subscriber vision_plant_sub = nh->subscribe("/pid_plantState_vision", 100, visionPlant_callback);
// //  ros::Subscriber imu_plant_sub = nh->subscribe("pid_plantState_IMU", 100, IMU_callback);

//
//
  // ros::Subscriber depth_plant_sub = nh->subscribe("/pid_plantState_depth", 100, depth_plant_callBack);

  ros::Subscriber depth_sub = nh->subscribe("/depth", 100, &Pid_Manager::depth_callBack, this);
  //setpoint publishers
   setpoint_surge_pub = nh->advertise<std_msgs::Float64>("setpoint_surge", 10);
   setpoint_sway_pub = nh->advertise<std_msgs::Float64>("setpoint_sway", 10);
   setpoint_heave_pub = nh->advertise<std_msgs::Float64>("setpoint_heave", 10);
   setpoint_roll_pub = nh->advertise<std_msgs::Float64>("setpoint_roll", 10);
   setpoint_pitch_pub = nh->advertise<std_msgs::Float64>("setpoint_pitch", 10);
   setpoint_yaw_pub = nh->advertise<std_msgs::Float64>("setpoint_yaw", 10);

  //plant state publishers
   state_surge_pub = nh->advertise<std_msgs::Float64>("state_surge", 10);
   state_sway_pub = nh->advertise<std_msgs::Float64>("state_sway", 10);
   state_heave_pub = nh->advertise<std_msgs::Float64>("state_heave", 10);
   state_roll_pub = nh->advertise<std_msgs::Float64>("state_roll", 10);
   state_pitch_pub = nh->advertise<std_msgs::Float64>("state_pitch", 10);
   state_yaw_pub = nh->advertise<std_msgs::Float64>("state_yaw", 10);

   control_effort_pub = nh->advertise<std_msgs::Float64>("controlEffort_surge", 10);

   surge_enable_pub = nh->advertise<std_msgs::Bool>("pidEnable_surge", 10);
   sway_enable_pub = nh->advertise<std_msgs::Bool>("pidEnable_sway", 10);
   heave_enable_pub = nh->advertise<std_msgs::Bool>("pidEnable_heave", 10);
   yaw_enable_pub = nh->advertise<std_msgs::Bool>("pidEnable_yaw", 10);

  this->setpoint_set(AXIS_SURGE, INPUT_IMU_POS, 0.0);
  this->setpoint_set(AXIS_SWAY, INPUT_IMU_POS, 0.0);
  this->setpoint_set(AXIS_HEAVE, INPUT_DEPTH, 0.0);

  this->setpoint_set(AXIS_YAW, INPUT_IMU_POS, 0.0);
}

Pid_Manager::~Pid_Manager(){

}

void Pid_Manager::setpoint_set(int axis, int input_type, double value){
  if (axis == AXIS_SURGE){
    std_msgs::Float64 setpoint_surge;
    if (input_type == INPUT_IMU_POS){
      parameters_surge.kp = 100;
      parameters_surge.kd = 0;
      parameters_surge.ki = 0;
      //set tuning for surge axis on imu position
      //publish setpoint
    }
    else if (input_type == INPUT_IMU_VEL){
      parameters_surge.kp = 5;
      parameters_surge.kd = 0;
      parameters_surge.ki = 0;
      //set tuning for surge axis to imu linear velocity
      //subscribe to this as plant state
    }
    else if (input_type == INPUT_CAM_FRONT){
      parameters_surge.kp = 1;
      parameters_surge.kd = 1;
      parameters_surge.ki = 1;
      //set tuning for front CAM_FRONT, set as plant state
    }
    else if (input_type == INPUT_CAM_BTM){
      parameters_surge.kp = 1;
      parameters_surge.kd = 1;
      parameters_surge.ki = 1;
      //same deal
    }
    else {
    //  cout << "The specified input_type does not exist";
      ROS_ERROR("The input_type does not exist for axis SURGE");
    }

    setpoint_surge.data = value;
    setpoint_surge_pub.publish(setpoint_surge);
  }

  else if (axis == AXIS_SWAY){

      std_msgs::Float64 setpoint_sway;

      if (input_type == INPUT_IMU_POS){
        parameters_sway.kp = 1;
        parameters_sway.kd = 1;
        parameters_sway.ki = 1;
        //set tuning for surge axis on imu position
        //subscribe to this as plant state
        //publish setpoint
      }
      else if (input_type == INPUT_IMU_VEL){
        parameters_sway.kp = 1;
        parameters_sway.kd = 1;
        parameters_sway.ki = 1;
        //set tuning for surge axis to imu linear velocity
        //subscribe to this as plant state
      }
      else if (input_type == INPUT_CAM_FRONT){
        parameters_sway.kp = 1;
        parameters_sway.kd = 1;
        parameters_sway.ki = 1;
        //set tuning for front CAM_FRONT, set as plant state
      }
      else if (input_type == INPUT_CAM_BTM){
        parameters_sway.kp = 1;
        parameters_sway.kd = 1;
        parameters_sway.ki = 1;
        //same deal
      }
      else {
        //cout << "The specified input_type does not exist";
        ROS_ERROR("The input_type does not exist for axis SWAY");
      }

      setpoint_sway.data = value;
      setpoint_sway_pub.publish(setpoint_sway);
  }

  else if (axis == AXIS_HEAVE){

      std_msgs::Float64 setpoint_heave;

      if (input_type == INPUT_DEPTH){
        parameters_heave.kp = 1;
        parameters_heave.kd = 1;
        parameters_heave.ki = 1;
        //set tuning for surge axis on depth sensor
        //subscribe to this as plant state
        //publish setpoint
      }

      else if (input_type == INPUT_CAM_FRONT){
        parameters_heave.kp = 1;
        parameters_heave.kd = 1;
        parameters_heave.ki = 1;
        //set tuning for front CAM_FRONT, set as plant state
      }

      else {
        // << "The specified input_type does not exist";
        ROS_ERROR("The input_type does not exist for axis HEAVE");
      }

      setpoint_heave.data = value;
      setpoint_heave_pub.publish(setpoint_heave);
  }

  else if (axis == AXIS_ROLL){

    std_msgs::Float64 setpoint_roll;

    if (input_type == INPUT_IMU_POS){
      parameters_roll.kp = 1;
      parameters_roll.kd = 1;
      parameters_roll.ki = 1;
      //you get it by now
    }

    else {
      //cout << "The specified input_type for 'ROLL' does not exist";
      ROS_ERROR("The input_type does not exist for axis ROLL");
    }
      setpoint_roll.data = value;
      setpoint_roll_pub.publish(setpoint_roll);
  }

  else if (axis == AXIS_PITCH){

    std_msgs::Float64 setpoint_pitch;

    if (input_type == INPUT_IMU_POS){
      parameters_pitch.kp = 1;
      parameters_pitch.kd = 1;
      parameters_pitch.ki = 1;
      //you get it by now
    }

    else {
    //  cout << "The specified input_type for 'PITCH' does not exist";
      ROS_ERROR("The input_type does not exist for axis PITCH");
    }

    setpoint_pitch.data = value;
    setpoint_pitch_pub.publish(setpoint_pitch);

  }

  else if (axis == AXIS_YAW){
    std_msgs::Float64 setpoint_yaw;

    if (input_type == INPUT_IMU_POS){
      parameters_yaw.kp = 1;
      parameters_yaw.kd = 1;
      parameters_yaw.ki = 1;
      //set tuning for surge axis on imu position
      //subscribe to this as plant state
      //publish setpoint
    }

    else if (input_type == INPUT_CAM_FRONT){
      parameters_yaw.kp = 1;
      parameters_yaw.kd = 1;
      parameters_yaw.ki = 1;
      //set tuning for front CAM_FRONT, set as plant state
    }
    else if (input_type == INPUT_CAM_BTM){
      parameters_yaw.kp = 1;
      parameters_yaw.kd = 1;
      parameters_yaw.ki = 1;
      //same deal
    }
    else{
    //  cout << "the specified input_type for axis 'YAW' does not exist";
      ROS_ERROR("The input_type does not exist for axis YAW");
    }

    setpoint_yaw.data = value;
    setpoint_yaw_pub.publish(setpoint_yaw);
  }

  else{
  //  cout << "the specified axis does not exist";
    ROS_ERROR("The axis %d %s", axis, "does not exist");
  }


}


void Pid_Manager::setPlantState(int axis, double plantValue){

  std_msgs::Float64 msg;
  msg.data = plantValue;

  if(axis == AXIS_SURGE)
    state_surge_pub.publish(msg);
  else if(axis == AXIS_SWAY)
    state_sway_pub.publish(msg);
  else if(axis == AXIS_HEAVE)
    state_heave_pub.publish(msg);
  else if(axis == AXIS_YAW)
    state_yaw_pub.publish(msg);
}

void Pid_Manager::zero(int sensor){

}

bool Pid_Manager::getKill(){
  return killSwitch;
}

bool Pid_Manager::getStart(){
  return startSwitch;
}

bool Pid_Manager::getTimeout(){

}

double Pid_Manager::getDepth(){
  return depth;
}

void Pid_Manager::controlEffort_set(int speed){
  std_msgs::Float64 effortMsg;
  effortMsg.data = speed;
  control_effort_pub.publish(effortMsg);
}

void Pid_Manager::setCamera(int camera){ //OLIVER FIX THIS
  if (camera == INPUT_CAM_FRONT)
    bool x = true;//do something;
  else
    bool x = false;
    //do something else;
}

void Pid_Manager::pidEnable(int axis, bool enabled){
  std_msgs::Bool enablePid;
  enablePid.data = enabled;

  if (axis == AXIS_SURGE){
    surge_enable_pub.publish(enablePid);
  }

  else if (axis == AXIS_SWAY){
    sway_enable_pub.publish(enablePid);
  }

  else if (axis == AXIS_HEAVE){
    heave_enable_pub.publish(enablePid);
  }

  else if (axis == AXIS_YAW){
    yaw_enable_pub.publish(enablePid);
  }

}

void Pid_Manager::taskDelay(int seconds){
  ros::Duration(seconds).sleep();
}
