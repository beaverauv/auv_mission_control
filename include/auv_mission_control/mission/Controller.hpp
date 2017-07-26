#ifndef MISSIONCONTROLLER_H_
#define MISSIONCONTROLLER_H_

#include <auv_mission_control/mission/Base.hpp>
#include <sensor_msgs/Joy.h>

typedef struct LogitechF310Mapping {
  std::array<float, 2> stick_left_ = {{0}};
  std::array<float, 2> stick_right_ = {{0}};
  std::array<float, 2> triggers_ = {{0}};
  std::array<float, 2> dpad_ = {{0}};

  std::array<bool, 2> stick_buttons_ = {{false}};
  std::array<bool, 2> bumpers_ = {{false}};

  bool button_a_ = false;
  bool button_b_ = false;
  bool button_x_ = false;
  bool button_y_ = false;

  bool button_back_ = false;
  bool button_start_ = false;
  bool button_power_ = false;
} LogitechF310Mapping_t;

namespace Mission {

class Controller : public Base {
public:
  Controller(std::shared_ptr<PointerHandler> ph)
      : INHERITED(ph), sm_(Macho::State<Top>(this)) {
    joystick_subscriber_ = nh_.subscribe<sensor_msgs::Joy>(
        "joy", 1, &Controller::joystickDataCallback, this);
  }

  ~Controller() {}

  ros::Subscriber joystick_subscriber_;

  sensor_msgs::Joy::ConstPtr joystick_data_;

  bool does_joy_data_exist_ = false;

  int scale_factor_ = 50;

  double depth_setpoint_ = -0.1;
  double yaw_setpoint_ = 0.0;

  LogitechF310Mapping_t last_joystick_data_;

  void joystickDataCallback(const sensor_msgs::Joy::ConstPtr &joy);

  LogitechF310Mapping_t getLogitechF310Mapping();

  AUV_LOG_TAG(Controller);

  AUV_CREATE_MISSION_TOP_STATE(Controller);

  AUV_CREATE_STATE(Init);

  AUV_CREATE_STATE(Kill);

  AUV_CREATE_EMPTY_STATE(Idle);

  AUV_CREATE_STATE(Default);

  AUV_CREATE_STATE(Tested);

  AUV_CREATE_STATE(Example);

  AUV_CREATE_STATE(Gate);

  AUV_CREATE_STATE(Buoy);

  AUV_CREATE_STATE(Marker);

  AUV_CREATE_MISSION_FUNCTIONS(Controller);
};
}

#endif
