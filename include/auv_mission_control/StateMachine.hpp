#ifndef STATEMACHINE_H
#define STATEMACHINE_H

#include <memory>
#include <vector>

#include <auv_mission_control/Macho.hpp>
#include <auv_mission_control/PointerHandler.hpp>

class StateMachine : public Task::Base {
public:
  StateMachine(std::shared_ptr<PointerHandler> ph)
      : ph_(ph), sm_(Macho::State<Top>(this)) {}

  ~StateMachine() {}

  AUV_LOG_TAG(Main);

  int execute();

  virtual StateMachine &self(void) { return *this; }
  virtual PointerHandler &ph(void) { return *ph_; }

  AUV_TOPSTATE(Top) {

    struct Box {
      Box() {}
      std::shared_ptr<StateMachine> self_;
    };

    AUV_STATE(Top);

    AUV_MACHINE_FUNCTIONS();

    virtual void run() {}

    StateMachine &self(void) { return *box().self_; }
    PointerHandler &ph(void) { return *self().ph_; }

  private:
    void init(StateMachine * self);
  };

  AUV_CREATE_STATE(Init);

  AUV_CREATE_STATE(Kill);

  AUV_CREATE_EMPTY_STATE(Nowhere);

  AUV_CREATE_STATE(Test);

  AUV_CREATE_STATE(Example);

  AUV_CREATE_STATE(Gate);

  AUV_CREATE_STATE(Buoy);

  AUV_CREATE_STATE(Marker);

  AUV_CREATE_FUNCTIONS(StateMachine)
};

#endif
