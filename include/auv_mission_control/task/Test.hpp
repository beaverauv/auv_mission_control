#ifndef TASKTEST_H
#define TASKTEST_H

#include <auv_mission_control/Task.hpp>

namespace Task {

class Test : public Base {
public:
  Test(std::shared_ptr<PointerHandler> ph)
      : ph_(ph), sm_(Macho::State<Top>(this, ph_)) {}

  ~Test() {}

  int execute();

  virtual Test &self(void) { return *this; }
  virtual PointerHandler &ph(void) { return *ph_; }

  AUV_LOG_TAG(Test);

  AUV_TOPSTATE(Top) {

    struct Box {
      Box() {}
      std::shared_ptr<Test> self_;
    };

    AUV_STATE(Top);

    AUV_MACHINE_FUNCTIONS();

    virtual void run() { setState<Init>(); }

    Test &self() { return *box().self_; }
    PointerHandler &ph() { return *self().ph_; }

  private:
    void init(Test * self, std::shared_ptr<PointerHandler> ph) {
      box().self_ = std::shared_ptr<Test>(self);
    }
  };

  AUV_CREATE_STATE(Init);

  AUV_CREATE_EMPTY_STATE(Nowhere);

  AUV_SUBSTATE(Whatever, Top) {

    AUV_STATE(Whatever);

    void run();
  };

  std::shared_ptr<PointerHandler> ph_;

  Macho::Machine<Top> sm_;

  AUV_CREATE_QUEUE(Test, sm_);
};
}

#endif
