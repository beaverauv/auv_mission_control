#include <auv_mission_control/PidManager.h>

double depth_;
bool bKillSwitchState_;
double plantSurge_;
double plantSway_;
double plantHeave_;
double plantRoll_;
double plantPitch_;
double plantYaw_;
sensor_msgs::Imu imu_;
bool subImuHasBeenCalled;
double yawInitValue;
/*
void visionPlant_callback(const auv_mission_control::axes::ConstPtr& vision){
  plant_surge_vision = vision->surge;
  plant_sway_vision = vision->sway;
  plant_heave_vision = vision->heave;
  plant_yaw_vision = vision->yaw;
}
*/

void PidManager::depthCallBack(const std_msgs::Float64::ConstPtr& depth_msg){
  depth_ = depth_msg->data;
  subDepthHasBeenCalled = true;
  //ROS_INFO("depth_ %f", depth_);
}


void PidManager::startCallBack(const std_msgs::Bool::ConstPtr& start_msg){
  bStartSwitchState_ = start_msg->data;
  //bStartSwitchState_ = false;
  subStartHasBeenCalled = true;

}

void PidManager::killCallBack(const std_msgs::Bool::ConstPtr& kill_msg){
  bKillSwitchState_ = kill_msg->data;
//  ROS_INFO("kill_msg->data = %d", kill_msg->data);
  subKillHasBeenCalled = true;
}

void PidManager::imuCallBack(const sensor_msgs::Imu::ConstPtr& imu_msg){
//  imu_ = *imu_msg;
  //ROS_INFO("boop");
  plantYaw_ = imu_msg->orientation.z;
  subImuHasBeenCalled = true;

}



PidManager::PidManager(){
}


PidManager::PidManager(ros::NodeHandle* nh) : nh_(*nh){

  //setZero all sensors

//   ros::Subscriber vision_plant_sub = nh_.subscribe("/pid_plantState_vision", 100, visionPlant_callback);
// //  ros::Subscriber imu_plant_sub = nh_.subscribe("pid_plantState_IMU", 100, IMU_callback);

//
//
  // ros::Subscriber depth_plant_sub = nh_.subscribe("/pid_plantState_depth", 100, depth_plant_callBack);

  bool bStartSwitchState_ = false;
  bool bKillSwitchState_ = false;
  bool bTimoutSwitchState_ = false;


  subDepth = nh_.subscribe("/depth", 20, &PidManager::depthCallBack, this);
  subStart = nh_.subscribe("/start", 1, &PidManager::startCallBack, this);
  subImu = nh_.subscribe("/imu/imu", 1, &PidManager::imuCallBack, this);
  subKill = nh_.subscribe("/kill", 1, &PidManager::killCallBack, this);

  //setpoint publishers
   pubSetpointSurge = nh_.advertise<std_msgs::Float64>("setpoint_surge", 10);
   pubSetpointSway = nh_.advertise<std_msgs::Float64>("setpoint_sway", 10);
   pubSetpointHeave = nh_.advertise<std_msgs::Float64>("setpoint_heave", 10);
   pubSetpointRoll = nh_.advertise<std_msgs::Float64>("setpoint_roll", 10);
   pubSetpointPitch = nh_.advertise<std_msgs::Float64>("setpoint_pitch", 10);
   pubSetpointYaw = nh_.advertise<std_msgs::Float64>("setpoint_yaw", 10);



  //plant state publishers
   pubStateSurge = nh_.advertise<std_msgs::Float64>("state_surge", 10);
   pubStateSway = nh_.advertise<std_msgs::Float64>("state_sway", 10);
   pubStateHeave = nh_.advertise<std_msgs::Float64>("state_heave", 10);
   pubStateRoll = nh_.advertise<std_msgs::Float64>("state_roll", 10);
   pubStatePitch = nh_.advertise<std_msgs::Float64>("state_pitch", 10);
   pubStateYaw = nh_.advertise<std_msgs::Float64>("state_yaw", 10);

   pubEffortSurge = nh_.advertise<std_msgs::Float64>("controlEffort_surge", 10);
   pubEffortSway = nh_.advertise<std_msgs::Float64>("controlEffort_sway", 10);
   pubEffortHeave = nh_.advertise<std_msgs::Float64>("controlEffort_heave", 10);
   pubEffortYaw = nh_.advertise<std_msgs::Float64>("controlEffort_yaw", 10);

   pubEnableSurge = nh_.advertise<std_msgs::Bool>("setPidEnabled_surge", 10);
   pubEnableSway = nh_.advertise<std_msgs::Bool>("setPidEnabled_sway", 10);
   pubEnableHeave = nh_.advertise<std_msgs::Bool>("setPidEnabled_heave", 10);
   pubEnableYaw = nh_.advertise<std_msgs::Bool>("setPidEnabled_yaw", 10);

  this->setSetpoint(AXIS_SURGE, INPUT_IMU_POS, 0.0);
  this->setSetpoint(AXIS_SWAY, INPUT_IMU_POS, 0.0);
  this->setSetpoint(AXIS_HEAVE, INPUT_DEPTH, 0.0);

  this->setSetpoint(AXIS_YAW, INPUT_IMU_POS, 0.0);
}

PidManager::~PidManager(){

}

void PidManager::setSetpoint(int axis, int input_type, double value){
  if (axis == AXIS_SURGE){
    std_msgs::Float64 msgSetpointSurge;

     if (input_type == INPUT_CAM_BTM){
      paramSurge.kP = 1;
      paramSurge.kD = 1;
      paramSurge.kI = 1;
      //same deal
    }
    else {
    //  cout << "The specified input_type does not exist";
      ROS_ERROR("The input_type does not exist for axis SURGE");
    }

    this->updateParams(AXIS_SURGE);

    msgSetpointSurge.data = value;
    pubSetpointSurge.publish(msgSetpointSurge);
  }

  else if (axis == AXIS_SWAY){

      std_msgs::Float64 msgSetpointSway;


      if (input_type == INPUT_CAM_FRONT){
        paramSway.kP = 0.028;
        paramSway.kD = 0.036;
        paramSway.kI = 0.032;
        //set tuning for front CAM_FRONT, set as plant state
      }
      else if (input_type == INPUT_CAM_BTM){
        paramSway.kP = 1;
        paramSway.kD = 1;
        paramSway.kI = 1;
        //same deal
      }
      else {
        //cout << "The specified input_type does not exist";
        ROS_ERROR("The input_type does not exist for axis SWAY");
      }

      this->updateParams(AXIS_SWAY);

      msgSetpointSway.data = value;
      pubSetpointSway.publish(msgSetpointSway);
  }

  else if (axis == AXIS_HEAVE){

      std_msgs::Float64 msgSetpointHeave;

      if (input_type == INPUT_DEPTH){
        paramHeave.kP = 0.75;//.;
        paramHeave.kD = 0;//.;
        paramHeave.kI = 0;//30.;
	paramHeave.Kp_scale = 100;
        //set tuning for surge axis on depth sensor
        //subscribe to this as plant state
        //publish setpoint
      }

      else if (input_type == INPUT_CAM_FRONT){
        paramHeave.kP = 0.035;
        paramHeave.kD = 0.03;
        paramHeave.kI = 0.04;
        //set tuning for front CAM_FRONT, set as plant state
      }

      else {
        // << "The specified input_type does not exist";
        ROS_ERROR("The input_type does not exist for axis HEAVE");
      }

      this->updateParams(AXIS_HEAVE);

      msgSetpointHeave.data = value;
      pubSetpointHeave.publish(msgSetpointHeave);
  }


  else if (axis == AXIS_YAW){
    std_msgs::Float64 msgSetpointYaw;

    if (input_type == INPUT_IMU_POS){
      paramYaw.kP = 0.2;
      paramYaw.kD = 0.1;
      paramYaw.kI = 0.1;
      //set tuning for surge axis on imu position
      //subscribe to this as plant state
      //publish setpoint
    }

    else if (input_type == INPUT_CAM_FRONT){
      paramYaw.kP = 0.038;
      paramYaw.kD = 0.01;
      paramYaw.kI = 0.028;
      //set tuning for front CAM_FRONT, set as plant state
    }
    else if (input_type == INPUT_CAM_BTM){
      paramYaw.kP = 0.5;
      paramYaw.kD = 1;
      paramYaw.kI = 1;
      //same deal
    }
    else{
    //  cout << "the specified input_type for axis 'YAW' does not exist";
      ROS_ERROR("The input_type does not exist for axis YAW");
    }

    this->updateParams(AXIS_YAW);

    msgSetpointYaw.data = value;
    pubSetpointYaw.publish(msgSetpointYaw);
  }

  else{
  //  cout << "the specified axis does not exist";
    ROS_ERROR("The axis %d %s", axis, "does not exist");
  }


}


void PidManager::setPlantState(int axis, double plantValue){

  std_msgs::Float64 msgPlantValue;
  msgPlantValue.data = plantValue;

  if(axis == AXIS_SURGE)
    pubStateSurge.publish(msgPlantValue);
  else if(axis == AXIS_SWAY)
    pubStateSway.publish(msgPlantValue);
  else if(axis == AXIS_HEAVE){
   ROS_INFO("boop");
   ROS_INFO("publishing heave: %f", plantValue);
   pubStateHeave.publish(msgPlantValue);
}
  else if(axis == AXIS_YAW){
    pubStateYaw.publish(msgPlantValue);
}
  else{
    ROS_ERROR("bad input type");

}
}
void PidManager::setZero(int sensor){
  yawInitValue = pm_.getYaw();
}

bool PidManager::getKill(){
  if(!subKillHasBeenCalled){
    return false;
  }
  else{
    return bKillSwitchState_;
  }

}

bool PidManager::getStart(){
  if (!subStartHasBeenCalled){
    //ROS_INFO("fsdfsd");
    return 0;
  }
  else
    return bStartSwitchState_;
}

bool PidManager::getTimeout(){
  return bTimoutSwitchState_;
}

double PidManager::getDepth(){
  if(!subDepthHasBeenCalled)
    return 0;
  else
    return depth_;
}

double PidManager::getYaw(){
  double yaw;
  //OS_INFO("here");
  if(!subImuHasBeenCalled){
	return 0;
}
  else{
    yaw = plantYaw_ - yawInitValue;

//-190. +180 = -10.          -170 + - 20 = -190. -190 - 180 = 10. 180 - 10 = 170;
  if(yaw < -180){
    double difference = yaw + 180;
    return 180 - difference;
  }
  else if (yaw > 180){ //160 + 30 = 190. 190 - 180 = 10. -180 + 10;
    double differnce = yaw - 180;
    return -180 + difference;

  }

}
}


void PidManager::setPidEnabled(int axis, bool enabled){
  std_msgs::Bool enablePid;
  enablePid.data = enabled;

  if (axis == AXIS_SURGE){
    pubEnableSurge.publish(enablePid);
  }

  else if (axis == AXIS_SWAY){
    pubEnableSway.publish(enablePid);
  }

  else if (axis == AXIS_HEAVE){
    pubEnableHeave.publish(enablePid);
  }

  else if (axis == AXIS_YAW){
    pubEnableYaw.publish(enablePid);
  }

}

void PidManager::setControlEffort(int axis, int speed){
  std_msgs::Float64 effortMsg;
  effortMsg.data = speed;

  if (axis == AXIS_SURGE){
    pubEffortSurge.publish(effortMsg);
  }

  else if (axis == AXIS_SWAY){
    pubEffortSway.publish(effortMsg);
  }

  else if (axis == AXIS_HEAVE){
    pubEffortHeave.publish(effortMsg);
  }

  else if (axis == AXIS_YAW){
    pubEffortYaw.publish(effortMsg);
  }
}

void PidManager::taskDelay(int seconds){
  ros::Duration(seconds).sleep();
}


void PidManager::updateParams(int axis){
  dynamic_reconfigure::ReconfigureRequest srv_req;
  dynamic_reconfigure::ReconfigureResponse srv_resp;
  dynamic_reconfigure::DoubleParameter double_param;
  dynamic_reconfigure::Config conf;

  if (axis == AXIS_SURGE){
     double_param.name = "Kp";
     double_param.value = paramSurge.kP;
     conf.doubles.push_back(double_param);

     double_param.name = "Ki";
     double_param.value = paramSurge.kI;
     conf.doubles.push_back(double_param);

     double_param.name = "Kd";
     double_param.value = paramSurge.kD;
     conf.doubles.push_back(double_param);
     srv_req.config = conf;

     ros::service::call("/surge_pid/set_parameters", srv_req, srv_resp);

  }

  else if (axis == AXIS_SWAY){
    double_param.name = "Kp";
    double_param.value = paramSway.kP;
    conf.doubles.push_back(double_param);

    double_param.name = "Ki";
    double_param.value = paramSway.kI;
    conf.doubles.push_back(double_param);

    double_param.name = "Kd";
    double_param.value = paramSway.kD;
    conf.doubles.push_back(double_param);

    srv_req.config = conf;

    ros::service::call("/sway_pid/set_parameters", srv_req, srv_resp);

  }

  else if (axis == AXIS_HEAVE){
    double_param.name = "Kp";
    double_param.value = paramHeave.kP;
    conf.doubles.push_back(double_param);

    double_param.name = "Ki";
    double_param.value = paramHeave.kI;
    conf.doubles.push_back(double_param);

    double_param.name = "Kd";
    double_param.value = paramHeave.kD;
    conf.doubles.push_back(double_param);

    double_param.name="Kp_scale";
    double_param.value=paramHeave.Kp_scale;
    conf.doubles.push_back(double_param);

    srv_req.config = conf;

    ros::service::call("/heave_pid/set_parameters", srv_req, srv_resp);

  }

  else if (axis == AXIS_YAW){
    double_param.name = "Kp";
    double_param.value = paramYaw.kP;
    conf.doubles.push_back(double_param);

    double_param.name = "Ki";
    double_param.value = paramYaw.kI;
    conf.doubles.push_back(double_param);

    double_param.name = "Kd";
    double_param.value = paramYaw.kD;
    conf.doubles.push_back(double_param);

    srv_req.config = conf;

    ros::service::call("/yaw_pid/set_parameters", srv_req, srv_resp);

  }
}
