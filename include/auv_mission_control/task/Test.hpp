#ifndef TASKTEST_H
#define TASKTEST_H

#include <auv_mission_control/Task.hpp>

namespace Task {

class Test : public Logger {
public:
  Test(std::shared_ptr<PointerHandler> ph)
      : INHERITED(ph), sm_(Macho::State<Top>(this)) {}

  ~Test() {}

  int execute();

  auto self() { return this; }

  AUV_LOG_TAG(Test);

  AUV_TOPSTATE(Top) {

    struct Box {
      Box() {}
      std::shared_ptr<Test> self_;
    };

    AUV_STATE(Top);

    AUV_MACHINE_FUNCTIONS();

    virtual void run() { setState<Init>(); }

    auto self() { return box().self_; }
    auto ph() { return self()->ph_; }
    auto pm() { return self()->pm(); }
    auto sm() { return self()->sm(); }
    auto mission() { return self()->mission(); }
    auto cam() { return self()->cam(); }
    auto vision() { return self()->vision(); }
    auto test() { return self()->test(); }
    auto example() { return self()->example(); }
    auto gate() { return self()->gate(); }
    auto buoy() { return self()->buoy(); }
    auto marker() { return self()->marker(); }

  private:
    void init(Test * self) { box().self_ = std::shared_ptr<Test>(self); }
  };

  AUV_CREATE_STATE(Init);

  AUV_CREATE_EMPTY_STATE(Nowhere);

  AUV_SUBSTATE(Whatever, Top) {

    AUV_STATE(Whatever);

    void run();
  };

  Macho::Machine<Top> sm_;

  AUV_CREATE_QUEUE(Test, sm_);

  typedef Logger INHERITED;
};
}

#endif
