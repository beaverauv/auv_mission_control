#include <mission_control/pid_parameters.h>
#include <mission_control/pid_manager.h>
#include "ros/ros.h"
#include "std_msgs/String.h"
#include "std_msgs/Float64.h"
#include "std_msgs/Bool.h"
#include <sstream>
#include <auv_motor_control/pid_enable.h>
#include <auv_mission_control/pid_plantState.h>
#include <auv_mission_control/pid_setpoint.h>
#include <dynamic_reconfigure/DoubleParameter.h>
#include <dynamic_reconfigure/Reconfigure.h>
#include <dynamic_reconfigure/Config.h>
#include <string>

double plant_surge;
double plant_sway;
double plant_heave;
double plant_roll;
double plant_pitch;
double plant_yaw;

double plant_surge_vision;
double plant_sway_vision;
double plant_heave_vision;
double plant_yaw_vision;

double plant_surge_imu_pos;
double plant_sway_imu_pos;
double plant_roll_imu_pos;
double plant_pitch_imu_pos;
double plant_yaw_imu_pos;

double plant_surge_imu_vel;
double plant_sway_imu_vel;
double plant_yaw_imu_vel;

double plant_heave_depth;

Pid_Parameters parameters_surge;
Pid_Parameters parameters_sway;
Pid_Parameters parameters_heave;
Pid_Parameters parameters_roll;
Pid_Parameters parameters_pitch;
Pid_Parameters parameters_yaw;

auv_motor_control::pid_enable enable_pid;


void visionPlant_callback(const auv_mission_control::pid_plantState::ConstPtr& vision){
  plant_surge_vision = vision->surge;
  plant_sway_vision = vision->sway;
  plant_heave_vision = vision->heave;
  plant_yaw_vision = vision->yaw;
}

void depth_callBack(const std_msgs::Float64::ConstPtr& depth){
  plant_heave_depth = depth->data;
}



int main(int argc, char **argv){
  ros::init(argc, argv, "PID_Manager");
  ros::NodeHandle n;
  ros::Subscriber vision_plant_sub = n.subscribe("pid_plantState_vision", 100, visionPlant_callback);
//  ros::Subscriber imu_plant_sub = n.subscribe("pid_plantState_IMU", 100, IMU_callback);


  ros::Subscriber depth_plant_sub = n.subscribe("pid_plantState_depth", 100, depth_callBack);
  //setpoint publishers
  ros::Publisher state_surge_pub = n.advertise<std_msgs::Float64>("state_surge", 10);
  ros::Publisher state_sway_pub = n.advertise<std_msgs::Float64>("state_sway", 10);
  ros::Publisher state_heave_pub = n.advertise<std_msgs::Float64>("state_heave", 10);
  ros::Publisher state_roll_pub = n.advertise<std_msgs::Float64>("state_roll", 10);
  ros::Publisher state_pitch_pub = n.advertise<std_msgs::Float64>("state_pitch", 10);
  ros::Publisher state_yaw_pub = n.advertise<std_msgs::Float64>("state_yaw", 10);
  //plant state publishers
  ros::Publisher setpoint_surge_pub = n.advertise<std_msgs::Float64>("setpoint_surge", 10);
  ros::Publisher setpoint_sway_pub = n.advertise<std_msgs::Float64>("setpoint_sway", 10);
  ros::Publisher setpoint_heave_pub = n.advertise<std_msgs::Float64>("setpoint_heave", 10);
  ros::Publisher setpoint_roll_pub = n.advertise<std_msgs::Float64>("setpoint_roll", 10);
  ros::Publisher setpoint_pitch_pub = n.advertise<std_msgs::Float64>("setpoint_pitch", 10);
  ros::Publisher setpoint_yaw_pub = n.advertise<std_msgs::Float64>("setpoint_yaw", 10);

}

Pid_Manager::Pid_Manager(){
  pm.pidEnable("ALL", true); //sets all inputs to defaults
}


void Pid_Manager::setpoint_set((std::string axis, std::string input_type, double value)){
  if (axis == "SURGE"){
    std_msgs::Float64 setpoint_surge;
    if (input_type == "IMU_POS"){
      parameters_surge.kp = 1;
      parameters_surge.kd = 1;
      parameters_surge.ki = 1;
      //set tuning for surge axis on imu position
      setpoint_surge =
      //publish setpoint
    }
    else if (input_type == "IMU_VEL"){
      parameters_surge.kp = 1;
      parameters_surge.kd = 1;
      parameters_surge.ki = 1;
      //set tuning for surge axis to imu linear velocity
      //subscribe to this as plant state
    }
    else if (input_type == "CAM_FRONT"){
      parameters_surge.kp = 1;
      parameters_surge.kd = 1;
      parameters_surge.ki = 1;
      //set tuning for front CAM_FRONT, set as plant state
    }
    else if (input_type == "CAM_BTTM"){
      parameters_surge.kp = 1;
      parameters_surge.kd = 1;
      parameters_surge.ki = 1;
      //same deal
    }
    else {
      cout << "The specified input_type does not exist";
      ROS_ERROR("The input_type %s", input_type, "does not exist for axis 'SURGE'");
    }

    setpoint_surge.data = value;
    surge_pub.publish(setpoint_surge);
  }

  else if (axis == "SWAY"){

      std_msgs::Float64 setpoint_sway;

      if (input_type == "IMU_POS"){
        parameters_sway.kp = 1;
        parameters_sway.kd = 1;
        parameters_sway.ki = 1;
        //set tuning for surge axis on imu position
        //subscribe to this as plant state
        //publish setpoint
      }
      else if (input_type == "IMU_VEL"){
        parameters_sway.kp = 1;
        parameters_sway.kd = 1;
        parameters_sway.ki = 1;
        //set tuning for surge axis to imu linear velocity
        //subscribe to this as plant state
      }
      else if (input_type == "CAM_FRONT"){
        parameters_sway.kp = 1;
        parameters_sway.kd = 1;
        parameters_sway.ki = 1;
        //set tuning for front CAM_FRONT, set as plant state
      }
      else if (input_type == "CAM_BTTM"){
        parameters_sway.kp = 1;
        parameters_sway.kd = 1;
        parameters_sway.ki = 1;
        //same deal
      }
      else {
        cout << "The specified input_type does not exist";
        ROS_ERROR("The input_type %s", input_type, "does not exist for axis 'SWAY'");
      }

      setpoint_sway.data = value;
      sway_pub.publish(setpoint_sway);
  }

  else if (axis == "HEAVE"){

      std_msgs::Float64 setpoint_heave;

      if (input_type == "DEPTH_SENSOR"){
        parameters_heave.kp = 1;
        parameters_heave.kd = 1;
        parameters_heave.ki = 1;
        //set tuning for surge axis on depth sensor
        //subscribe to this as plant state
        //publish setpoint
      }

      else if (input_type == "CAM_FRONT"){
        parameters_heave.kp = 1;
        parameters_heave.kd = 1;
        parameters_heave.ki = 1;
        //set tuning for front CAM_FRONT, set as plant state
      }

      else {
        cout << "The specified input_type does not exist";
        ROS_ERROR("The input_type %s", input_type, "does not exist for axis 'HEAVE'");
      }

      setpoint_heave.data = value;
      heave_pub.publish(setpoint_heave);
  }

  else if (axis == "ROLL"){

    std_msgs::Float64 setpoint_roll;

    if (input_type == "IMU_POS"){
      parameters_roll.kp = 1;
      parameters_roll.kd = 1;
      parameters_roll.ki = 1;
      //you get it by now
    }

    else {
      cout << "The specified input_type for 'ROLL' does not exist";
      ROS_ERROR("The input_type %s", input_type, "does not exist for axis 'ROLL'");
    }
      setpoint_roll.data = value;
      roll_pub.publish(setpoint_roll);
  }

  else if (axis == "PITCH"){

    std_msgs::Float64 setpoint_pitch;

    if (input_type == "IMU_POS"){
      parameters_pitch.kp = 1;
      parameters_pitch.kd = 1;
      parameters_pitch.ki = 1;
      //you get it by now
    }

    else {
      cout << "The specified input_type for 'PITCH' does not exist";
      ROS_ERROR("The input_type %s", input_type, "does not exist for axis 'PITCH'");
    }

    setpoint_pitch.data = value;
    pitch_pub.publish(setpoint_pitch);

  }
  else if (axis == "YAW"){
    std_msgs::Float64 setpoint_yaw;

    if (input_type == "IMU_POS"){
      parameters_yaw.kp = 1;
      parameters_yaw.kd = 1;
      parameters_yaw.ki = 1;
      //set tuning for surge axis on imu position
      //subscribe to this as plant state
      //publish setpoint
    }
    else if (input_type == "IMU_MAG"){
      parameters_yaw.kp = 1;
      parameters_yaw.kd = 1;
      parameters_yaw.ki = 1;
      //set tuning for surge axis to imu magnetometer data
      //subscribe to this as plant state
    }
    else if (input_type == "CAM_FRONT"){
      parameters_yaw.kp = 1;
      parameters_yaw.kd = 1;
      parameters_yaw.ki = 1;
      //set tuning for front CAM_FRONT, set as plant state
    }
    else if (input_type == "CAM_BTTM"){
      parameters_yaw.kp = 1;
      parameters_yaw.kd = 1;
      parameters_yaw.ki = 1;
      //same deal
    }
    else{
      cout << "the specified input_type for axis 'YAW' does not exist";
      ROS_ERROR("The input_type %s", input_type, "does not exist for axis 'YAW'");
    }

    setpoint_yaw.data = value;
    yaw_pub.publish(setpoint_yaw);
  }

  else{
    cout << "the specified axis does not exist";
    ROS_ERROR("The axis %s", axis.c_str(), "does not exist");
  }

}


void pid_defaults(){

}


void
