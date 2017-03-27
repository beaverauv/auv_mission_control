#include <auv_mission_control/task/TaskTest.hpp>
#include <auv_mission_control/template_states.hpp>

int TaskTest::execute() {
  if (checkEventQueue()) {
    sm_->run();
  }
}

void TaskTest::Init::run() {
  // setState<Timer::TimerOld<Whatever>>(3.0, Macho::Event(&Top::here));
  // setState<Timer::TimerOld<Init> >(3.0, Top::box().self_);
  // setState<Timer::TimerOld<Init> >(3.0, StateMachine::Test::alias());

  // setState<Timer::TimerOld<Whatever>>(3.0, Macho::State<Whatever>(),
  //                                  Macho::Event(&TaskTest::Whatever::here));

  self().queueEnable();

  self().queueState<Move<Nowhere>>({AXIS::YAW, AXIS::HEAVE, AXIS::ROLL},
                                   {6.0, 5.0, 45.0}, 3.0);

  // Top::box().self_->queueState<MoveOld<Nowhere>>(
  //     {AXIS::YAW, AXIS::HEAVE, AXIS::ROLL}, {14.0, 6.0, 46.0}, 3.0,
  //     StateMachine::Marker::alias());
  //
  self().queueState<Move<Nowhere>>({AXIS::YAW, AXIS::HEAVE, AXIS::ROLL},
                                   {14.0, 6.0, 46.0}, 3.0);

  self().queueState<Timer<Whatever>>(1.0);
}

void TaskTest::Whatever::run() {
  setState<Timer<Init>>(0.0, StateMachine::Example::alias());
}
