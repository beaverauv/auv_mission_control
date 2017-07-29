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

  AUV_CREATE_STATE(Scan);
  AUV_CREATE_STATE(Heave);
  AUV_CREATE_STATE(Sway);
  AUV_CREATE_STATE(YawDrive);
  AUV_CREATE_STATE(BackScan);
  AUV_CREATE_STATE(Kill);

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
  void checkForBuoy();

  double redDepth; // depth of red buoy

  int scanIterator = 0;
  double scanAngle = 0;
  // Timer ramRed;
  int ramRedCounter = 0;
};
}

#endif
