#include <auv_mission_control/TemplateStates.hpp>
#include <auv_mission_control/mission/Controller.hpp>

namespace Mission {

int Controller::execute() {
  // pm()->ensureDepth();
  // pm()->ensureYaw();

  if (checkEventQueue()) {
    sm_->run();
  }
}

void Controller::joystickDataCallback(const sensor_msgs::Joy::ConstPtr &joy) {
  if (!does_joy_data_exist_)
    does_joy_data_exist_ = true;

  joystick_data_ = joy;
  // stick_left_[0] = joy->axes[0];
  // stick_left_[1] = joy->axes[1];
  //
  // stick_right_[0] = joy->axes[3];
  // stick_right_[1] = joy->axes[4];
  //
  // trigger_[0] = joy->axes[2];
  // trigger_[1] = joy->axes[5];
  //
  // pad_[0] = joy->axes[6];
  // pad_[1] = joy->axes[7];
}

LogitechF310Mapping_t Controller::getLogitechF310Mapping() {
  LogitechF310Mapping_t mapped_data;

  if (!does_joy_data_exist_)
    return mapped_data;

  mapped_data.stick_left_ = {
      {joystick_data_->axes[0], joystick_data_->axes[1]}};
  mapped_data.stick_right_ = {
      {joystick_data_->axes[3], joystick_data_->axes[4]}};
  mapped_data.triggers_ = {{joystick_data_->axes[2], joystick_data_->axes[5]}};
  mapped_data.dpad_ = {{joystick_data_->axes[6], joystick_data_->axes[7]}};

  mapped_data.stick_buttons_ = {
      {joystick_data_->buttons[9], joystick_data_->buttons[10]}};
  mapped_data.bumpers_ = {
      {joystick_data_->buttons[4], joystick_data_->buttons[5]}};

  mapped_data.button_a_ = joystick_data_->buttons[0];
  mapped_data.button_b_ = joystick_data_->buttons[1];
  mapped_data.button_x_ = joystick_data_->buttons[2];
  mapped_data.button_y_ = joystick_data_->buttons[3];

  mapped_data.button_back_ = joystick_data_->buttons[6];
  mapped_data.button_start_ = joystick_data_->buttons[7];
  mapped_data.button_power_ = joystick_data_->buttons[8];
}

void Controller::Init::run() {
  // setState<Timer::Timer>(3.0, Buoy::alias);
  // AUV_DEBUG("Test::Init::run: ID %d", Macho::State<Top>()._KeyData)
  // Macho::IEvent<Test::Top> *event =
  //     Macho::Event(&Test::Top::whatever);
  // setState<Timer::Timer>
  // setState<TimerOld<Init>>(3.0);
  // setState<MoveOld<Init>>({INPUT::IMU_POS}, {10.0}, 3.0);

  // setState<MoveOld<Test>>(std::initializer_list<INPUT>{INPUT::IMU_POS},
  //  std::initializer_list<double>{10.0}, 3.0);
  // setState<MoveOld<Test>>(AxisVec{AXIS::YAW, AXIS::HEAVE, AXIS::ROLL},
  //                      ValuesVec{10.0, 5.0, 45.0}, 3.0);
  // AUV_INFO("%s", ph().pm_->getAxisName(0).c_str());
  self()->queueEnable();

  AUV_INFO("Current mission is %s", ph()->mission_str().c_str());

  AUV_INFO("Starting movement...");

  // Top::box().self_->queueState<TimerOld<Nowhere>>(10.0);
  //
  // Top::box().self_->queueState<MoveOld<Nowhere>>(
  //     {AXIS::YAW, AXIS::HEAVE, AXIS::ROLL}, {10.0, 5.0, 45.0}, 3.0);

  // Top::box().self_->queueState<MoveOld<Test>>({AXIS::YAW, AXIS::SURGE},
  //                                          {6.0, 10.0}, 400.0);

  // QUEUE_ACTION(Move, {AXIS::SURGE}, {26.0}, 10.0)
  // // std::vector<INPUT> in1 = {INPUT::IMU_POS};
  // // std::vector<double> in2 = {10.0};
  // // foo<char, int, float> f1;
  // // foo<char, int> f2;
  // // bar(f1, f2, 9);
  //
  QUEUE_STATE(Default);
  //
  // setState<MoveOld<Init>>(INPUTS{INPUT::IMU_POS}, 3.0);
}

void Controller::Default::run() {
  LogitechF310Mapping_t mapped_data = self()->getLogitechF310Mapping();
  AUV_INFO("Stick left: %f, %f", mapped_data.stick_left_[0],
           mapped_data.stick_left_[1]);
  // rotation
  pm()->setControlEffort(AXIS::YAW,
                         (mapped_data.triggers_[0] + mapped_data.triggers_[1]) *
                             self()->scale_factor_);

  // left and right
  pm()->setControlEffort(AXIS::SWAY,
                         mapped_data.stick_left_[0] * self()->scale_factor_);
  // forward and backward
  pm()->setControlEffort(AXIS::SURGE,
                         mapped_data.stick_left_[1] * self()->scale_factor_);

  pm()->setControlEffort(AXIS::ROLL,
                         mapped_data.stick_right_[0] * self()->scale_factor_);

  pm()->setControlEffort(AXIS::PITCH,
                         mapped_data.stick_right_[1] * self()->scale_factor_);

  pm()->setControlEffort(AXIS::HEAVE, (mapped_data.stick_buttons_[1] -
                                       mapped_data.stick_buttons_[0]) *
                                          self()->scale_factor_);
}

void Controller::Tested::run() {}

void Controller::Example::run() {}

void Controller::Gate::run() {}

void Controller::Buoy::run() {}

void Controller::Marker::run() {}

void Controller::Kill::run() {
  // AUV_ERROR("Kill::run");
  // setState<Init>();
}
}
