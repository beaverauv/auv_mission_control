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

  AUV_CREATE_MISSION_TOP_STATE(Test);

  AUV_CREATE_STATE(Init);

  AUV_CREATE_STATE(Kill);

  AUV_CREATE_EMPTY_STATE(Idle);

  AUV_CREATE_STATE(Tested);

  AUV_CREATE_STATE(Example);

  AUV_CREATE_STATE(Gate);

  AUV_CREATE_STATE(Buoy);

  AUV_CREATE_STATE(Marker);

  AUV_CREATE_MISSION_FUNCTIONS(Test);
};
}

#endif
