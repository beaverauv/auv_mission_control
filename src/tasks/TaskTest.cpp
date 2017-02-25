// #include <auv_mission_control/StateMachine.hpp>
#include <auv_mission_control/TaskTest.hpp>
#include <auv_mission_control/template_states.hpp>

TaskTest::TaskTest() {}

TaskTest::TaskTest(std::shared_ptr<PidManager> pm,
                   std::shared_ptr<Vision> vision) {
  AUV_DEBUG("[Pointers] [PM]: %x", pm.get());
  AUV_DEBUG("[Pointers] [VISION]: %x", vision.get());

  state_test_->setPointer(pm);
  state_test_->setPointer(vision);
  state_test_->setPointer(std::shared_ptr<TaskTest>(this));
}

TaskTest::~TaskTest() {}

void TaskTest::prepare(std::shared_ptr<StateMachine> statemachine) {
  state_test_->setPointer(statemachine);
  state_test_->initialize();
}

int TaskTest::execute() {
  if (checkEventQueue()) {
    state_test_->run();
  }
}

void TaskTest::Init::run() {
  AUV_INFO("Init::run");
  AUV_DEBUG("Waiting for 3 seconds");
  // setState<Timer::Timer<Whatever>>(3.0, Macho::Event(&Top::here));
  // setState<Timer::Timer<Init> >(3.0, Top::box().self_);
  // setState<Timer::Timer<Init> >(3.0, StateMachine::Test::alias());

  // setState<Timer::Timer<Whatever>>(3.0, Macho::State<Whatever>(),
  //                                  Macho::Event(&TaskTest::Whatever::here));
  Top::box().self_->queueEnable();

  Top::box().self_->queueState<Move<Nowhere>>(
      {AXIS::YAW, AXIS::HEAVE, AXIS::ROLL}, {6.0, 5.0, 45.0}, 1.0);

  // Top::box().self_->queueState<Move<Nowhere>>(
  //     {AXIS::YAW, AXIS::HEAVE, AXIS::ROLL}, {14.0, 6.0, 46.0}, 3.0,
  //     StateMachine::Marker::alias());
  //
  Top::box().self_->queueState<Move<Nowhere>>(
      {AXIS::YAW, AXIS::HEAVE, AXIS::ROLL}, {14.0, 6.0, 46.0}, 1.0);

  Top::box().self_->queueState<Timer<Whatever>>(1.0);
}

void TaskTest::Whatever::run() {
  setState<Timer<Init>>(0.0, StateMachine::Marker::alias());
}
