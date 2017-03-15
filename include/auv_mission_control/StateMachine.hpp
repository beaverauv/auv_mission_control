#ifndef STATEMACHINE_H
#define STATEMACHINE_H

#include <memory>
#include <vector>

#include <auv_mission_control/Camera.hpp>
#include <auv_mission_control/Macho.hpp>
#include <auv_mission_control/PidManager.hpp>
#include <auv_mission_control/PointerHandler.hpp>
#include <auv_mission_control/Task.hpp>
#include <auv_mission_control/TaskBuoy.hpp>
#include <auv_mission_control/TaskGate.hpp>
#include <auv_mission_control/TaskMarker.hpp>
#include <auv_mission_control/TaskTest.hpp>
#include <auv_mission_control/Vision.hpp>

class StateMachine : public Task {
public:
  StateMachine();
  ~StateMachine();

  std::string getTaskTag() { return std::string("[Task Main]"); }

  int execute();

  TOPSTATE(Top) {

    struct Box {
      Box() {}
      std::shared_ptr<PointerHandler> ph_;
      std::shared_ptr<StateMachine> self_;
    };

    STATE(Top);

    createMachineFunctions();

    virtual void run() {}
  };

  SUBSTATE(Init, Top) {

    STATE(Init);

    void run();
  };

  SUBSTATE(Kill, Top) {

    STATE(Kill);

    void run();
  };

  SUBSTATE(Nowhere, Top) {

    STATE(Nowhere);

    void run() {}
  };

  SUBSTATE(Test, Top) {

    STATE(Test);

    void run();

  private:
    void init() { Top::box().ph_->test_->prepare(Top::box().self_); }
  };

  SUBSTATE(Gate, Top) {

    STATE(Gate);

    void run();

  private:
    void init() { Top::box().ph_->gate_->prepare(Top::box().self_); }
  };

  SUBSTATE(Buoy, Top) {

    STATE(Buoy);

    void run();

  private:
    void init() { Top::box().ph_->buoy_->prepare(Top::box().self_); }
  };

  SUBSTATE(Marker, Top) {

    STATE(Marker)

    void run();

  private:
    void init() { Top::box().ph_->marker_->prepare(Top::box().self_); }
  };

  std::shared_ptr<PointerHandler> ph_;

  Macho::Machine<StateMachine::Top> state_;

  createQueue(StateMachine, state_)
};

#endif
