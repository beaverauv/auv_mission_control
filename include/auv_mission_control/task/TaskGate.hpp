#ifndef TASKGATE_H
#define TASKGATE_H

#include <auv_mission_control/task/Task.hpp>

class TaskGate : public TaskBase {
public:
  TaskGate(std::shared_ptr<PointerHandler> ph)
      : ph_(ph), sm_(Macho::State<Top>(this, ph_)) {}

  ~TaskGate() {}

  AUV_LOG_TAG(TaskGate);

  AUV_CREATE_TOP_STATE(TaskGate);

  AUV_CREATE_STATE(Init);

  AUV_CREATE_EMPTY_STATE(Nowhere);

  SUBSTATE(Whatever, Top) {

    STATE(Whatever);

    void run();
  };

  AUV_CREATE_FUNCTIONS(TaskGate);

private:
  // variables go here;
  bool startTimer = 0;
  double thisDepth = -0.25;

  int ColorSpace = 0;
  int minObjectArea = 20 * 20; // 20x20 blob
  bool objectFound;

  bool killSwitch = 0;

  int action = 0;
  // Timer goToDepth_time;
  int depthCounter = 0;
  // Timer driveForwards_time;
  int forwardCounter = 0;
  // Timer markerTimer;
  int markerCounter = 0;
  int reZeroCounter = 0;
  double surgeSpeed = 25;
  double previousDepth;
  double currentDepth;
  int rosInfoCounter;
  double distanceFromEdge_left;
  double distanceFromEdge_right;
  bool outOfSight;
  double plantState_sway;
  double plantState_heave;
  double setpoint_sway;
  double setpoint_heave;
  double setpoint_surge;
  double plantState_surge;
};

#endif
