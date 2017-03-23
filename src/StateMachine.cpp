#include <auv_mission_control/StateMachine.hpp>
#include <auv_mission_control/template_states.hpp>

int main(int argc, char *argv[]) {
  ros::init(argc, argv, "state_machine");
  if (ros::console::set_logger_level(ROSCONSOLE_DEFAULT_NAME,
                                     ros::console::levels::Debug)) {
    ros::console::notifyLoggerLevelsChanged();
  }
  auto ph = std::make_shared<PointerHandler>();

  ph->sm_ = std::make_shared<StateMachine>(ph);
  ph->mission_ = std::make_shared<Mission>(ph);
  ph->pm_ = std::make_shared<PidManager>();
  ph->cam_ = std::make_shared<Camera>();
  ph->vision_ = std::make_shared<Vision>(ph);
  ph->test_ = std::make_shared<TaskTest>(ph);
  ph->example_ = std::make_shared<TaskExample>(ph);
  ph->gate_ = std::make_shared<TaskGate>(ph);
  ph->buoy_ = std::make_shared<TaskBuoy>(ph);
  ph->marker_ = std::make_shared<TaskMarker>(ph);

  ph->sm_->queueEnable();
  ph->sm_->queueState<StateMachine::Init>();

  ph->sm_->execute();
}

int StateMachine::execute() {
  AUV_INFO("Execute");

  ros::Rate state_rate(20);

  AUV_INFO("Waiting for IMU data...");

  while (!ph().pm_->isImuCalled() && ros::ok()) {
    ros::spinOnce();
    ph().pm_->updatePlantState(AXIS::YAW);
    ph().pm_->setZero(AXIS::YAW);
  }

  ph().pm_->startEnsuringDepth();
  ph().pm_->startEnsuringYaw();

  while (ros::ok()) {
    ros::spinOnce();

    state_rate.sleep();

    ph().pm_->ensureDepth();

    ph().pm_->ensureYaw();

    if (checkEventQueue()) {
      sm_->run();
    }
  }
}

void StateMachine::Top::init(StateMachine *self) {
  box().self_ = std::shared_ptr<StateMachine>(self);
}

void StateMachine::Init::run() {
  // setState<Timer::Timer>(3.0, Buoy::alias);
  // AUV_DEBUG("StateMachine::Init::run: ID %d", Macho::State<Top>()._KeyData)
  // Macho::IEvent<StateMachine::Top> *event =
  //     Macho::Event(&StateMachine::Top::whatever);
  // setState<Timer::Timer>
  // setState<TimerOld<Init>>(3.0);
  // setState<MoveOld<Init>>({INPUT::IMU_POS}, {10.0}, 3.0);

  // setState<MoveOld<Test>>(std::initializer_list<INPUT>{INPUT::IMU_POS},
  //  std::initializer_list<double>{10.0}, 3.0);
  // setState<MoveOld<Test>>(AxisVec{AXIS::YAW, AXIS::HEAVE, AXIS::ROLL},
  //                      ValuesVec{10.0, 5.0, 45.0}, 3.0);
  self().queueEnable();

  AUV_INFO("Starting movement...");

  // Top::box().self_->queueState<TimerOld<Nowhere>>(10.0);
  //
  // Top::box().self_->queueState<MoveOld<Nowhere>>(
  //     {AXIS::YAW, AXIS::HEAVE, AXIS::ROLL}, {10.0, 5.0, 45.0}, 3.0);

  // Top::box().self_->queueState<MoveOld<Test>>({AXIS::YAW, AXIS::SURGE},
  //                                          {6.0, 10.0}, 400.0);

  self().queueState<Move<Test>>({AXIS::SURGE}, {26.0}, 10.0);

  // std::vector<INPUT> in1 = {INPUT::IMU_POS};
  // std::vector<double> in2 = {10.0};
  // foo<char, int, float> f1;
  // foo<char, int> f2;
  // bar(f1, f2, 9);

  // setState<MoveOld<Init>>(INPUTS{INPUT::IMU_POS}, 3.0);
}

void StateMachine::Test::run() { ph().test_->execute(); }

void StateMachine::Example::run() { ph().example_->execute(); }

void StateMachine::Gate::run() { ph().gate_->execute(); }

void StateMachine::Buoy::run() { ph().buoy_->execute(); }

void StateMachine::Marker::run() { ph().marker_->execute(); }

void StateMachine::Kill::run() {
  // AUV_ERROR("Kill::run");
  // setState<Init>();
}
