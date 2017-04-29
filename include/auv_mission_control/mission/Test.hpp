#ifndef MISSIONTEST_H_
#define MISSIONTEST_H_

#include <auv_mission_control/mission/Base.hpp>

namespace Mission {

class Test : public Base {
public:
  Test(std::shared_ptr<PointerHandler> ph)
      : INHERITED(ph), sm_(Macho::State<Top>(this)) {}

  ~Test() {}

  AUV_LOG_TAG(Test);

  int execute();

  auto self() { return this; }

  AUV_TOPSTATE(Top) {

    struct Box {
      Box() {}
      std::shared_ptr<Test> self_;
    };

    AUV_STATE(Top);

    AUV_MACHINE_FUNCTIONS();

    virtual void run() { setState<Test::Init>(); }

    auto self() { return box().self_; }
    auto ph() { return self()->ph(); }
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
  typedef Base INHERITED;
};
}

#endif
