// #include <auv_mission_control/StateMachine.hpp>
#include <auv_mission_control/TaskTest.hpp>
#include <auv_mission_control/template_states.hpp>

TaskTest::TaskTest(std::shared_ptr<PointerHandler> ph)
    : ph_(ph), state_test_(Macho::State<Top>(this, ph_)) {}

int TaskTest::execute() {
  if (checkEventQueue()) {
    state_test_->run();
  }
}

void TaskTest::Top::init(TaskTest *self, std::shared_ptr<PointerHandler> ph) {
  box().self_ = std::shared_ptr<TaskTest>(self);
  box().ph_ = ph;
}

void TaskTest::Init::run() {
  // setState<Timer::TimerOld<Whatever>>(3.0, Macho::Event(&Top::here));
  // setState<Timer::TimerOld<Init> >(3.0, Top::box().self_);
  // setState<Timer::TimerOld<Init> >(3.0, StateMachine::Test::alias());

  // setState<Timer::TimerOld<Whatever>>(3.0, Macho::State<Whatever>(),
  //                                  Macho::Event(&TaskTest::Whatever::here));
  Top::box().self_->queueEnable();

  Top::box().self_->queueState<Move<Nowhere>>(
      {AXIS::YAW, AXIS::HEAVE, AXIS::ROLL}, {6.0, 5.0, 45.0}, 3.0);

  // Top::box().self_->queueState<MoveOld<Nowhere>>(
  //     {AXIS::YAW, AXIS::HEAVE, AXIS::ROLL}, {14.0, 6.0, 46.0}, 3.0,
  //     StateMachine::Marker::alias());
  //
  Top::box().self_->queueState<Move<Nowhere>>(
      {AXIS::YAW, AXIS::HEAVE, AXIS::ROLL}, {14.0, 6.0, 46.0}, 3.0);

  Top::box().self_->queueState<Timer<Whatever>>(1.0);
}

void TaskTest::Whatever::run() {
  setState<Timer<Init>>(0.0, StateMachine::Marker::alias());
}
