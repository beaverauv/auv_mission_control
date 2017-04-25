#ifndef STATEMACHINE_H
#define STATEMACHINE_H

#include <memory>
#include <vector>

#include <auv_mission_control/Macho.hpp>
#include <auv_mission_control/PointerHandler.hpp>

class StateMachine : public Logger {
public:
  StateMachine(std::shared_ptr<PointerHandler> ph) : INHERITED(ph) {}

  ~StateMachine() {}

  AUV_LOG_TAG_NO_SM(Main);

  int execute();

  auto self() { return this; }

  typedef Logger INHERITED;
};

#endif
