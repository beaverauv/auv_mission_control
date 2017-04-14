#ifndef MISSIONTEST_H_
#define MISSIONTEST_H_

#include <auv_mission_control/mission/Base.hpp>

namespace Mission {

class Test : public Base {
public:
  Test(std::shared_ptr<PointerHandler> ph)
      : Base(ph), sm_(Macho::State<Top>(this)) {}

  ~Test() {}

  AUV_LOG_TAG(Test);

  int execute();

  virtual Test &self(void) { return *this; }
  virtual PointerHandler &ph(void) { return *Base::ph_; }

  AUV_TOPSTATE(Top) {

    struct Box {
      Box() {}
      std::shared_ptr<Test> self_;
    };

    AUV_STATE(Top);

    AUV_MACHINE_FUNCTIONS();

    virtual void run() { setState<Test::Init>(); }

    Test &self(void) { return *box().self_; }
    PointerHandler &ph(void) { return self().ph(); }

  private:
    void init(Test * self);
  };

  AUV_CREATE_STATE(Init);

  AUV_CREATE_STATE(Kill);

  AUV_CREATE_EMPTY_STATE(Nowhere);

  AUV_CREATE_STATE(Tested);

  AUV_CREATE_STATE(Example);

  AUV_CREATE_STATE(Gate);

  AUV_CREATE_STATE(Buoy);

  AUV_CREATE_STATE(Marker);

  AUV_CREATE_FUNCTIONS(Test)

private:
  // std::shared_ptr<PointerHandler> ph_;
};
}

#endif
