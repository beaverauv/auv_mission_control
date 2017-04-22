#ifndef TASKBUOY_H
#define TASKBUOY_H

#include <auv_mission_control/Task.hpp>

namespace Task {

class Buoy : public Logger {
public:
  Buoy(std::shared_ptr<PointerHandler> ph)
      : ph_(ph), sm_(Macho::State<Top>(this, ph_)) {}

  ~Buoy() {}

  AUV_LOG_TAG(Buoy);

  AUV_CREATE_TOP_STATE(Buoy);

  AUV_CREATE_STATE(Init);

  AUV_CREATE_EMPTY_STATE(Nowhere);

  SUBSTATE(Whatever, Top) {

    STATE(Whatever);

    void run();
  };

  AUV_CREATE_FUNCTIONS(Buoy);

private:
  int iterator = 0;
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
