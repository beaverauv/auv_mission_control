// #include <auv_mission_control/StateMachine.hpp>
#include <auv_mission_control/TaskMarker.hpp>
#include <auv_mission_control/template_states.hpp>

TaskMarker::TaskMarker() {}

TaskMarker::TaskMarker(std::shared_ptr<PidManager> pm,
                       std::shared_ptr<Vision> vision) {
  AUV_DEBUG("[Pointers] [PM]: %x", pm.get());
  AUV_DEBUG("[Pointers] [VISION]: %x", vision.get());
  state_marker_->setPointer(pm);
  state_marker_->setPointer(vision);
  state_marker_->setPointer(std::shared_ptr<TaskMarker>(this));
}

TaskMarker::~TaskMarker() {}

void TaskMarker::prepare(std::shared_ptr<StateMachine> statemachine) {
  state_marker_->setPointer(statemachine);
  state_marker_->initialize();
}

int TaskMarker::execute() { state_marker_->run(); }

void TaskMarker::Init::run() {
  AUV_DEBUG("Init::run");
  AUV_DEBUG("Waiting for 3 seconds");
  setState<Timer<Init>>(3.0);
}
