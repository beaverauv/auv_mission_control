#ifndef STATEMACHINE_H
#define STATEMACHINE_H

#include <memory>
#include <vector>

#include <auv_mission_control/Macho.hpp>
#include <auv_mission_control/PointerHandler.hpp>

class StateMachine : public Task {
public:
  StateMachine();
  ~StateMachine();

  createTaskTag(TaskMain);

  int execute();

  virtual StateMachine &self(void) { return *this; }
  virtual PointerHandler &ph(void) { return *ph_; }

  AUV_TOPSTATE(Top) {

    struct Box {
      Box() {}
      std::shared_ptr<StateMachine> self_;
    };

    AUV_STATE(Top);

    createMachineFunctions();

    virtual void run() {}

    StateMachine &self(void) { return *box().self_; }
    PointerHandler &ph(void) { return *self().ph_; }

  private:
    void init(StateMachine * self);
  };

  createState(Init);

  createState(Kill);

  createNullState(Nowhere);

  createState(Test);

  createState(Example);

  createState(Gate);

  createState(Buoy);

  createState(Marker);

  createTaskFunctions(StateMachine)
};

#endif
