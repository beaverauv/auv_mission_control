#include <auv_mission_control/TemplateStates.hpp>
#include <auv_mission_control/task/Example.hpp>

namespace Task {

int Example::execute() {

  if (checkEventQueue()) {
    sm_->run();
  }
}

void Example::Init::run() {
  self()->queueEnable();
  //
  QUEUE_ACTION(Timer, 1.0);
  QUEUE_STATE(SwitchTask);
}

void Example::SwitchTask::run() {
  //
  ph()->queueTask("Marker");
}
}
