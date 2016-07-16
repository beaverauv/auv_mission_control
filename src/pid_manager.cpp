#include <auv_mission_control/pid_manager.h>


Pid_Parameters parameters_surge;
Pid_Parameters parameters_sway;
Pid_Parameters parameters_heave;
Pid_Parameters parameters_roll;
Pid_Parameters parameters_pitch;
Pid_Parameters parameters_yaw;

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





Pid_Manager::Pid_Manager(ros::NodeHandle* nh){

  //zero all sensors

//   ros::Subscriber vision_plant_sub = nh.subscribe("/pid_plantState_vision", 100, visionPlant_callback);
// //  ros::Subscriber imu_plant_sub = nh.subscribe("pid_plantState_IMU", 100, IMU_callback);

this->nh = nh;
//
//
  // ros::Subscriber depth_plant_sub = nh.subscribe("/pid_plantState_depth", 100, depth_plant_callBack);

  ros::Subscriber depth_sub = nh.subscribe("/depth", 100, Pid_Manager::depth_callBack);
  //setpoint publishers
  ros::Publisher state_surge_pub = nh.advertise<std_msgs::Float64>("state_surge", 10);
  ros::Publisher state_sway_pub = nh.advertise<std_msgs::Float64>("state_sway", 10);
  ros::Publisher state_heave_pub = nh.advertise<std_msgs::Float64>("state_heave", 10);
  ros::Publisher state_roll_pub = nh.advertise<std_msgs::Float64>("state_roll", 10);
  ros::Publisher state_pitch_pub = nh.advertise<std_msgs::Float64>("state_pitch", 10);
  ros::Publisher state_yaw_pub = nh.advertise<std_msgs::Float64>("state_yaw", 10);
  //plant state publishers
  ros::Publisher setpoint_surge_pub = nh.advertise<std_msgs::Float64>("setpoint_surge", 10);
  ros::Publisher setpoint_sway_pub = nh.advertise<std_msgs::Float64>("setpoint_sway", 10);
  ros::Publisher setpoint_heave_pub = nh.advertise<std_msgs::Float64>("setpoint_heave", 10);
  ros::Publisher setpoint_roll_pub = nh.advertise<std_msgs::Float64>("setpoint_roll", 10);
  ros::Publisher setpoint_pitch_pub = nh.advertise<std_msgs::Float64>("setpoint_pitch", 10);
  ros::Publisher setpoint_yaw_pub = nh.advertise<std_msgs::Float64>("setpoint_yaw", 10);


  this->setpoint_set(AXIS_SURGE, INPUT_IMU_POS, 0.0);
  this->setpoint_set(AXIS_SWAY, INPUT_IMU_POS, 0.0);
  this->setpoint_set(AXIS_HEAVE, INPUT_DEPTH, 0.0);
  this->setpoint_set(AXIS_ROLL, INPUT_IMU_POS, 0.0);
  this->setpoint_set(AXIS_PITCH, INPUT_IMU_POS, 0.0);
  this->setpoint_set(AXIS_YAW, INPUT_IMU_POS, 0.0);
}

Pid_Manager::


void Pid_Manager::setpoint_set((int axis, int input_type, double value)){
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
      cout << "The specified input_type does not exist";
      ROS_ERROR("The input_type does not exist for axis SURGE");
    }

    setpoint_surge.data = value;
    surge_pub.publish(setpoint_surge);
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
        cout << "The specified input_type does not exist";
        ROS_ERROR("The input_type does not exist for axis SWAY");
      }

      setpoint_sway.data = value;
      sway_pub.publish(setpoint_sway);
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
        cout << "The specified input_type does not exist";
        ROS_ERROR("The input_type does not exist for axis HEAVE");
      }

      setpoint_heave.data = value;
      heave_pub.publish(setpoint_heave);
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
      cout << "The specified input_type for 'ROLL' does not exist";
      ROS_ERROR("The input_type does not exist for axis ROLL");
    }
      setpoint_roll.data = value;
      roll_pub.publish(setpoint_roll);
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
      cout << "The specified input_type for 'PITCH' does not exist";
      ROS_ERROR("The input_type does not exist for axis PITCH");
    }

    setpoint_pitch.data = value;
    pitch_pub.publish(setpoint_pitch);

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
      cout << "the specified input_type for axis 'YAW' does not exist";
      ROS_ERROR("The input_type does not exist for axis YAW");
    }

    setpoint_yaw.data = value;
    yaw_pub.publish(setpoint_yaw);
  }

  else{
    cout << "the specified axis does not exist";
    ROS_ERROR("The axis %s", axis.c_str(), "does not exist");
  }

}


void Pid_Manager::setPlantState(int axis, double plantValue, ){

}

double Pid_Manager::getDepth(){
  return depth;
}


void pid_defaults(){

}
