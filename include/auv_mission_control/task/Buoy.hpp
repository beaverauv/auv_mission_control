#ifndef TASKBUOY_H
#define TASKBUOY_H

#include <auv_mission_control/Task.hpp>

namespace Task {

class Buoy : public Logger {
public:
  Buoy(std::shared_ptr<PointerHandler> ph)
      : INHERITED(ph), sm_(Macho::State<Top>(this)) {}

  ~Buoy() {}

  AUV_LOG_TAG(Buoy);

  AUV_CREATE_TOP_STATE(Buoy);

  AUV_CREATE_STATE(Init);
  AUV_CREATE_EMPTY_STATE(Idle);
  AUV_CREATE_STATE(SwitchTask);
  AUV_CREATE_STATE(GoToDepth);
  AUV_CREATE_EMPTY_STATE(Nowhere);
  AUV_CREATE_STATE(BumpBuoys);


  AUV_CREATE_FUNCTIONS(Buoy);

private:
  // variables go here
  int currentColor;
  int objectFound;
  int action = 0;
  // Timer goToDepth_time;
  int depthCounter = 0;
  // Timer driveForwards_time;
  int forwardCounter = 0;
  // Timer waitTimer;
  int waitCounter = 0;
  double surgeSpeed = 25;
  double previousDepth;
  double distanceFromEdge_left;
  double distanceFromEdge_right;
  bool outOfSight;
  double plantState_sway;
  double plantState_heave;
  double setpoint_sway;
  double setpoint_heave;
  double setpoint_surge;
  double plantState_surge;

  double redDepth; // depth of red buoy

  // Timer ramRed;
  int ramRedCounter = 0;
};
}

#endif
