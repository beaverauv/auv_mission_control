#include <auv_mission_control/task/Test.hpp>
#include <auv_mission_control/template_states.hpp>

namespace Task {

int Test::execute() {
  if (checkEventQueue()) {
    sm_->run();
  }
}

void Test::Init::run() {

  // setState<Timer::TimerOld<Whatever>>(3.0, Macho::Event(&Top::here));
  // setState<Timer::TimerOld<Init> >(3.0, Top::box().self_);
  // setState<Timer::TimerOld<Init> >(3.0, StateMachine::Test::alias());

  // setState<Timer::TimerOld<Whatever>>(3.0, Macho::State<Whatever>(),
  //                                  Macho::Event(&Test::Whatever::here));

  self().queueEnable();

  // self().queueState<Move<Nowhere>>({AXIS::YAW, AXIS::HEAVE, AXIS::ROLL},
  //                                  {6.0, 5.0, 45.0}, 3.0);

  QUEUE_ACTION(Move, {AXIS::YAW, AXIS::HEAVE, AXIS::ROLL}, {6.0, 5.0, 45.0},
               3.0);

  // Top::box().self_->queueState<MoveOld<Nowhere>>(
  //     {AXIS::YAW, AXIS::HEAVE, AXIS::ROLL}, {14.0, 6.0, 46.0}, 3.0,
  //     StateMachine::Marker::alias());
  //
  QUEUE_ACTION(Move, {AXIS::YAW, AXIS::HEAVE, AXIS::ROLL}, {14.0, 6.0, 46.0},
               3.0);

  QUEUE_ACTION(Timer, 1.0);
}

void Test::Whatever::run() { setState(ph().alias("Example")); }
}
