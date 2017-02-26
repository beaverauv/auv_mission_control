#include <auv_mission_control/StateMachine.hpp>
#include <auv_mission_control/template_states.hpp>

int main(int argc, char *argv[]) {
  ros::init(argc, argv, "state_machine");
  if (ros::console::set_logger_level(ROSCONSOLE_DEFAULT_NAME,
                                     ros::console::levels::Debug)) {
    ros::console::notifyLoggerLevelsChanged();
  }
  auto statemachine = std::make_shared<StateMachine>();
  statemachine->queueEnable();
  statemachine->queueState<StateMachine::Init>();

  statemachine->execute();
}

StateMachine::StateMachine() {
  AUV_INFO("Init");

  state_->setPointer(std::shared_ptr<StateMachine>(this));
}

StateMachine::~StateMachine() {}

int StateMachine::execute() {
  AUV_INFO("Execute");

  ros::Rate state_rate(20);

  state_->box().pm_->startEnsuringDepth();
  state_->box().pm_->startEnsuringYaw();

  while (!state_->box().pm_->isImuCalled() && ros::ok()) {
    ros::spinOnce();
    state_->box().pm_->updatePlantState(AXIS::YAW);
    state_->box().pm_->setZero(AXIS::YAW);
  }

  while (ros::ok()) {
    ros::spinOnce();

    state_rate.sleep();

    state_->box().pm_->ensureDepth();

    state_->box().pm_->ensureYaw();

    if (checkEventQueue()) {
      state_->run();
    }
  }
}

void StateMachine::Top::setPointer(std::shared_ptr<StateMachine> statemachine) {
  box().self_ = statemachine;
  box().statemachine_ = statemachine;
}

void StateMachine::Init::run() {
  AUV_INFO("%x", Top::box().self_.get());
  AUV_DEBUG("Init::run");
  AUV_DEBUG("Waiting for 3 seconds");
  // setState<Timer::Timer>(3.0, Buoy::alias);
  // AUV_DEBUG("StateMachine::Init::run: ID %d", Macho::State<Top>()._KeyData)
  // Macho::IEvent<StateMachine::Top> *event =
  //     Macho::Event(&StateMachine::Top::whatever);
  // setState<Timer::Timer>
  // setState<Timer<Init>>(3.0);
  // setState<Move<Init>>({INPUT::IMU_POS}, {10.0}, 3.0);

  // setState<Move<Test>>(std::initializer_list<INPUT>{INPUT::IMU_POS},
  //  std::initializer_list<double>{10.0}, 3.0);
  // setState<Move<Test>>(AxisVec{AXIS::YAW, AXIS::HEAVE, AXIS::ROLL},
  //                      ValuesVec{10.0, 5.0, 45.0}, 3.0);
  Top::box().self_->queueEnable();

  // Top::box().self_->queueState<Timer<Nowhere>>(10.0);
  //
  // Top::box().self_->queueState<Move<Nowhere>>(
  //     {AXIS::YAW, AXIS::HEAVE, AXIS::ROLL}, {10.0, 5.0, 45.0}, 3.0);

  // Top::box().self_->queueState<Move<Test>>({AXIS::YAW, AXIS::SURGE},
  //                                          {6.0, 10.0}, 400.0);

  Top::box().self_->queueState<Move<Test>>({AXIS::SURGE}, {10.0}, 400.0);

  // std::vector<INPUT> in1 = {INPUT::IMU_POS};
  // std::vector<double> in2 = {10.0};
  // foo<char, int, float> f1;
  // foo<char, int> f2;
  // bar(f1, f2, 9);

  // setState<Move<Init>>(INPUTS{INPUT::IMU_POS}, 3.0);
}

void StateMachine::Test::run() { Top::box().test_->execute(); }

void StateMachine::Gate::run() { Top::box().gate_->execute(); }

void StateMachine::Buoy::run() { Top::box().buoy_->execute(); }

void StateMachine::Marker::run() { Top::box().marker_->execute(); }

void StateMachine::Kill::run() {
  // AUV_ERROR("Kill::run");
  // setState<Init>();
}
