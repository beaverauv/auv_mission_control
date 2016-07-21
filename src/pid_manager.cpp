#include <auv_mission_control/pid_manager.h>

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

void Pid_Manager::yaw_callBack(const std_msgs::Float64::ConstPtr& yaw_msg){
  currentYaw = yaw_msg->data;
}

void Pid_Manager::start_callBack(const std_msgs::Bool::ConstPtr& start_msg){
  startSwitch = start_msg->data;
}

void Pid_Manager::kill_callBack(const std_msgs::Bool::ConstPtr& kill_msg){
  killSwitch = kill_msg->data;
}

void Pid_Manager::imu_callBack(const sensor_msgs::Imu::ConstPtr& imu_msg){
  imu_ = *imu_msg;
}



Pid_Manager::Pid_Manager(){
}


Pid_Manager::Pid_Manager(ros::NodeHandle* nh) : nh_(*nh){

  //zero all sensors

//   ros::Subscriber vision_plant_sub = nh_.subscribe("/pid_plantState_vision", 100, visionPlant_callback);
// //  ros::Subscriber imu_plant_sub = nh_.subscribe("pid_plantState_IMU", 100, IMU_callback);

//
//
  // ros::Subscriber depth_plant_sub = nh_.subscribe("/pid_plantState_depth", 100, depth_plant_callBack);

  ros::Subscriber depth_sub = nh_.subscribe("/depth", 100, &Pid_Manager::depth_callBack, this);

  ros::Subscriber imu_sub = nh_.subscribe("/imu/imu", 100, &Pid_Manager::imu_callBack, this);

  ros::Subscriber yaw_sub = nh_.subscribe("plantState-yaw", 10, &Pid_Manager::yaw_callBack, this);
  //setpoint publishers
   setpoint_surge_pub = nh_.advertise<std_msgs::Float64>("setpoint_surge", 10);
   setpoint_sway_pub = nh_.advertise<std_msgs::Float64>("setpoint_sway", 10);
   setpoint_heave_pub = nh_.advertise<std_msgs::Float64>("setpoint_heave", 10);
   setpoint_roll_pub = nh_.advertise<std_msgs::Float64>("setpoint_roll", 10);
   setpoint_pitch_pub = nh_.advertise<std_msgs::Float64>("setpoint_pitch", 10);
   setpoint_yaw_pub = nh_.advertise<std_msgs::Float64>("setpoint_yaw", 10);

  //plant state publishers
   state_surge_pub = nh_.advertise<std_msgs::Float64>("state_surge", 10);
   state_sway_pub = nh_.advertise<std_msgs::Float64>("state_sway", 10);
   state_heave_pub = nh_.advertise<std_msgs::Float64>("state_heave", 10);
   state_roll_pub = nh_.advertise<std_msgs::Float64>("state_roll", 10);
   state_pitch_pub = nh_.advertise<std_msgs::Float64>("state_pitch", 10);
   state_yaw_pub = nh_.advertise<std_msgs::Float64>("state_yaw", 10);

   control_effort_pub = nh_.advertise<std_msgs::Float64>("controlEffort_surge", 10);

   surge_enable_pub = nh_.advertise<std_msgs::Bool>("pidEnable_surge", 10);
   sway_enable_pub = nh_.advertise<std_msgs::Bool>("pidEnable_sway", 10);
   heave_enable_pub = nh_.advertise<std_msgs::Bool>("pidEnable_heave", 10);
   yaw_enable_pub = nh_.advertise<std_msgs::Bool>("pidEnable_yaw", 10);

  this->setSetPoint(AXIS_SURGE, INPUT_IMU_POS, 0.0);
  this->setSetPoint(AXIS_SWAY, INPUT_IMU_POS, 0.0);
  this->setSetPoint(AXIS_HEAVE, INPUT_DEPTH, 0.0);

  this->setSetPoint(AXIS_YAW, INPUT_IMU_POS, 0.0);
}

Pid_Manager::~Pid_Manager(){

}

void Pid_Manager::setSetPoint(int axis, int input_type, double value){
  if (axis == AXIS_SURGE){
    std_msgs::Float64 setpoint_surge;

     if (input_type == INPUT_CAM_BTM){
      parameters_surge.kp = 1;
      parameters_surge.kd = 1;
      parameters_surge.ki = 1;
      //same deal
    }
    else {
    //  cout << "The specified input_type does not exist";
      ROS_ERROR("The input_type does not exist for axis SURGE");
    }

    this->updateParameters(AXIS_SURGE);

    setpoint_surge.data = value;
    setpoint_surge_pub.publish(setpoint_surge);
  }

  else if (axis == AXIS_SWAY){

      std_msgs::Float64 setpoint_sway;


      if (input_type == INPUT_CAM_FRONT){
        parameters_sway.kp = 0.028;
        parameters_sway.kd = 0.036;
        parameters_sway.ki = 0.032;
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

      this->updateParameters(AXIS_SWAY);

      setpoint_sway.data = value;
      setpoint_sway_pub.publish(setpoint_sway);
  }

  else if (axis == AXIS_HEAVE){

      std_msgs::Float64 setpoint_heave;

      if (input_type == INPUT_DEPTH){
        parameters_heave.kp = 0.4;
        parameters_heave.kd = 0.3;
        parameters_heave.ki = 0.3;
        //set tuning for surge axis on depth sensor
        //subscribe to this as plant state
        //publish setpoint
      }

      else if (input_type == INPUT_CAM_FRONT){
        parameters_heave.kp = 0.035;
        parameters_heave.kd = 0.03;
        parameters_heave.ki = 0.04;
        //set tuning for front CAM_FRONT, set as plant state
      }

      else {
        // << "The specified input_type does not exist";
        ROS_ERROR("The input_type does not exist for axis HEAVE");
      }

      this->updateParameters(AXIS_HEAVE);

      setpoint_heave.data = value;
      setpoint_heave_pub.publish(setpoint_heave);
  }


  else if (axis == AXIS_YAW){
    std_msgs::Float64 setpoint_yaw;

    if (input_type == INPUT_IMU_POS){
      double publishData;

      parameters_yaw.kp = 0.2;
      parameters_yaw.kd = 0.1;
      parameters_yaw.ki = 0.1;
      publishData = value + yawZeroValue; //compensates for 0 values

      if(publishData > 180){ //changes so always correctly between 180 and -180
        double difference = 180 - publishData;
        publishData = -180 - difference;
      }

      if(publishData < -180){//same
        double difference = 180 + publishData;
        publishData = 180 + difference;
      }
      setpoint_yaw.data = publishData;
    }



    else if (input_type == INPUT_CAM_FRONT){
      parameters_yaw.kp = 0.038;
      parameters_yaw.kd = 0.01;
      parameters_yaw.ki = 0.028;
      setpoint_yaw.data = value;

      //set tuning for front CAM_FRONT, set as plant state
    }
    else if (input_type == INPUT_CAM_BTM){
      parameters_yaw.kp = 0.5;
      parameters_yaw.kd = 1;
      parameters_yaw.ki = 1;
      setpoint_yaw.data = value;

      //same deal
    }
    else{
    //  cout << "the specified input_type for axis 'YAW' does not exist";
      ROS_ERROR("The input_type does not exist for axis YAW");
    }

    this->updateParameters(AXIS_YAW);

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

void Pid_Manager::setControlEffort(int axis, int speed){
  std_msgs::Float64 effortMsg;
  effortMsg.data = speed;

  if (axis == AXIS_SURGE){
    surgeEffort_pub.publish(effortMsg);
  }

  else if (axis == AXIS_SWAY){
    swayEffort_pub.publish(effortMsg);
  }

  else if (axis == AXIS_HEAVE){
    heaveEffort_pub.publish(effortMsg);
  }

  else if (axis == AXIS_YAW){
    yawEffort_pub.publish(effortMsg);
  }
}

void Pid_Manager::taskDelay(int seconds){
  ros::Duration(seconds).sleep();
}


void Pid_Manager::updateParameters(int axis){
  dynamic_reconfigure::ReconfigureRequest srv_req;
  dynamic_reconfigure::ReconfigureResponse srv_resp;
  dynamic_reconfigure::DoubleParameter double_param;
  dynamic_reconfigure::Config conf;

  if (axis == AXIS_SURGE){
     double_param.name = "Kp";
     double_param.value = parameters_surge.kp;
     conf.doubles.push_back(double_param);

     double_param.name = "Ki";
     double_param.value = parameters_surge.ki;
     conf.doubles.push_back(double_param);

     double_param.name = "Kd";
     double_param.value = parameters_surge.kd;
     conf.doubles.push_back(double_param);
     srv_req.config = conf;

     ros::service::call("/surge_pid/set_parameters", srv_req, srv_resp);

  }

  else if (axis == AXIS_SWAY){
    double_param.name = "Kp";
    double_param.value = parameters_sway.kp;
    conf.doubles.push_back(double_param);

    double_param.name = "Ki";
    double_param.value = parameters_sway.ki;
    conf.doubles.push_back(double_param);

    double_param.name = "Kd";
    double_param.value = parameters_sway.kd;
    conf.doubles.push_back(double_param);

    srv_req.config = conf;

    ros::service::call("/sway_pid/set_parameters", srv_req, srv_resp);

  }

  else if (axis == AXIS_HEAVE){
    double_param.name = "Kp";
    double_param.value = parameters_heave.kp;
    conf.doubles.push_back(double_param);

    double_param.name = "Ki";
    double_param.value = parameters_heave.ki;
    conf.doubles.push_back(double_param);

    double_param.name = "Kd";
    double_param.value = parameters_heave.kd;
    conf.doubles.push_back(double_param);

    srv_req.config = conf;

    ros::service::call("/heave_pid/set_parameters", srv_req, srv_resp);

  }

  else if (axis == AXIS_YAW){
    double_param.name = "Kp";
    double_param.value = parameters_yaw.kp;
    conf.doubles.push_back(double_param);

    double_param.name = "Ki";
    double_param.value = parameters_yaw.ki;
    conf.doubles.push_back(double_param);

    double_param.name = "Kd";
    double_param.value = parameters_yaw.kd;
    conf.doubles.push_back(double_param);

    srv_req.config = conf;

    ros::service::call("/yaw_pid/set_parameters", srv_req, srv_resp);

  }
}//end update parameters


void Pid_Manager::zero(int axis){
  if (axis == AXIS_YAW){
    yawZeroValue = currentYaw;
  }

  else{
    ROS_ERROR("The specified axis does not exist");
  }
}
