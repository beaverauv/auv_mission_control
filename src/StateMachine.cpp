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

StateMachine::StateMachine() : pc_(std::make_shared<PointerController>()) {
  state_->box().self_ = std::shared_ptr<StateMachine>(this);

  pc_->sm_ = std::shared_ptr<StateMachine>(this);
  pc_->mission_ = std::make_shared<Mission>();
  pc_->pm_ = std::make_shared<PidManager>();
  pc_->cam_ = std::make_shared<Camera>();
  pc_->vision_ = std::make_shared<Vision>(pc_->cam_);
  pc_->test_ = std::make_shared<TaskTest>(pc_->pm_, pc_->vision_);
  pc_->gate_ = std::make_shared<TaskGate>(pc_->pm_, pc_->vision_);
  pc_->buoy_ = std::make_shared<TaskBuoy>(pc_->pm_, pc_->vision_);
  pc_->marker_ = std::make_shared<TaskMarker>(pc_->pm_, pc_->vision_);

  state_->box().pc_ = pc_;
}

StateMachine::~StateMachine() {}

int StateMachine::execute() {
  AUV_INFO("Execute");

  ros::Rate state_rate(20);

  AUV_INFO("Waiting for IMU data...");

  while (!pc_->pm_->isImuCalled() && ros::ok()) {
    ros::spinOnce();
    pc_->pm_->updatePlantState(AXIS::YAW);
    pc_->pm_->setZero(AXIS::YAW);
  }

  pc_->pm_->startEnsuringDepth();
  pc_->pm_->startEnsuringYaw();

  while (ros::ok()) {
    ros::spinOnce();

    state_rate.sleep();

    pc_->pm_->ensureDepth();

    pc_->pm_->ensureYaw();

    if (checkEventQueue()) {
      state_->run();
    }
  }
}

void StateMachine::Init::run() {
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

  Top::box().self_->queueState<MoveTest<Test>>({AXIS::SURGE}, {26.0}, 10.0);

  // std::vector<INPUT> in1 = {INPUT::IMU_POS};
  // std::vector<double> in2 = {10.0};
  // foo<char, int, float> f1;
  // foo<char, int> f2;
  // bar(f1, f2, 9);

  // setState<Move<Init>>(INPUTS{INPUT::IMU_POS}, 3.0);
}

void StateMachine::Test::run() { Top::box().pc_->test_->execute(); }

void StateMachine::Gate::run() { Top::box().pc_->gate_->execute(); }

void StateMachine::Buoy::run() { Top::box().pc_->buoy_->execute(); }

void StateMachine::Marker::run() { Top::box().pc_->marker_->execute(); }

void StateMachine::Kill::run() {
  // AUV_ERROR("Kill::run");
  // setState<Init>();
}
