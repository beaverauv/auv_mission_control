#include <auv_mission_control/TaskBuoy.hpp>
#include <auv_mission_control/template_states.hpp>

TaskBuoy::TaskBuoy() {}

TaskBuoy::TaskBuoy(std::shared_ptr<PidManager> pm,
                   std::shared_ptr<Vision> vision) {
  AUV_DEBUG("[Pointers] [PM]: %x", pm.get());
  AUV_DEBUG("[Pointers] [VISION]: %x", vision.get());
  state_buoy_->setPointer(pm);
  state_buoy_->setPointer(vision);
  state_buoy_->setPointer(std::shared_ptr<TaskBuoy>(this));
}

TaskBuoy::~TaskBuoy() {}

void TaskBuoy::prepare(std::shared_ptr<StateMachine> statemachine) {
  state_buoy_->setPointer(statemachine);
  state_buoy_->initialize();
}

int TaskBuoy::execute() { state_buoy_->run(); }

void TaskBuoy::Init::run() {
  AUV_DEBUG("Init::run");
  AUV_DEBUG("Waiting for 3 seconds");
  setState<Timer<Init>>(3.0);
}
