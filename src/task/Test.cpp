#include <auv_mission_control/TemplateStates.hpp>
#include <auv_mission_control/task/Test.hpp>

namespace Task {

AUV_CREATE_PARAM_LOADER(Test);

int Test::execute() {
  if (checkEventQueue()) {
    sm_->run();
  }
}

void Test::Init::run() {

  // setState<Timer::TimerOld<Whatever>>(3.0, Macho::Event(&Top::here));
  // setState<Timer::TimerOld<Init> >(3.0, Top::box().self_);
  // setState<Timer::TimerOld<Init> >(3.0, StateMachine::Test::alias());

  AUV_INFO("Currently in: %s", getParamPath().c_str());

  // setState<Timer::TimerOld<Whatever>>(3.0, Macho::State<Whatever>(),
  //                                  Macho::Event(&Test::Whatever::here));

  self()->queueEnable();
  //
  // self().queueState<Move<Nowhere>>({AXIS::YAW, AXIS::HEAVE, AXIS::ROLL},
  //                                  {6.0, 5.0, 45.0}, 3.0);

  QUEUE_ACTION(Move, {AXIS::YAW, AXIS::HEAVE, AXIS::ROLL}, {6.0, 5.0, 45.0},
               3.0);

  QUEUE_ACTION(Move, {AXIS::YAW, AXIS::HEAVE, AXIS::ROLL}, {14.0, 6.0, 46.0},
               3.0);

  // self().queueState<Move<Nowhere>>({AXIS::YAW, AXIS::HEAVE, AXIS::ROLL},
  //                                  {14.0, 6.0, 46.0}, 3.0);
  QUEUE_ACTION(Timer, 1.0, SwitchTask::alias());
  // QUEUE_STATE(Whatever);
}

void Test::SwitchTask::run() { ph()->queueTask("Example"); }
}
