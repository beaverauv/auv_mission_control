// #include <auv_mission_control/StateMachine.hpp>
#include <auv_mission_control/TaskGate.hpp>
#include <auv_mission_control/template_states.hpp>

TaskGate::TaskGate() {}

TaskGate::TaskGate(std::shared_ptr<PidManager> pm,
                   std::shared_ptr<Vision> vision) {
  state_gate_->setPointer(pm);
  state_gate_->setPointer(vision);
  state_gate_->setPointer(std::shared_ptr<TaskGate>(this));
}

TaskGate::~TaskGate() {}

void TaskGate::prepare(std::shared_ptr<StateMachine> statemachine) {
  state_gate_->setPointer(statemachine);
  state_gate_->initialize();
}

int TaskGate::execute() { state_gate_->run(); } // execute

void TaskGate::Init::run() { setState<Timer<Init>>(3.0); }
