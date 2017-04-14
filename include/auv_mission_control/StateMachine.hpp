#ifndef STATEMACHINE_H
#define STATEMACHINE_H

#include <memory>
#include <vector>

#include <auv_mission_control/Macho.hpp>
#include <auv_mission_control/PointerHandler.hpp>

class StateMachine : public Logger {
public:
  StateMachine(std::shared_ptr<PointerHandler> ph) : ph_(ph) {}

  ~StateMachine() {}

  AUV_LOG_TAG_NO_SM(Main);

  int execute();

  virtual StateMachine &self(void) { return *this; }
  virtual PointerHandler &ph(void) { return *ph_; }

  std::shared_ptr<PointerHandler> ph_;
};

#endif
