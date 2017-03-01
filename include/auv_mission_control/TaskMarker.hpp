#ifndef TASKMARKER_H
#define TASKMARKER_H

#include <cmath>
#include <unistd.h>

// #include <auv_mission_control/Timer.hpp>
#include <auv_mission_control/Camera.hpp>
#include <auv_mission_control/Macho.hpp>
#include <auv_mission_control/PidManager.hpp>
#include <auv_mission_control/Task.hpp>
#include <auv_mission_control/Vision.hpp>
//#include <auv_mission_control/StateMachine.hpp>

class StateMachine;

class TaskMarker : public Task {
public:
  TaskMarker();
  TaskMarker(std::shared_ptr<PidManager> pm, std::shared_ptr<Vision> vision);
  ~TaskMarker();

  std::string getTaskTag() { return std::string("[Task Mark]"); }

  int execute();
  void prepare(std::shared_ptr<StateMachine> statemachine);

private:
  // variables go here;
  // Timer swayTimer;
  // Timer depthTimer;
  // Timer yawTimer;
  int action = 0;
  bool objectFound;
  double plantState_sway;
  double setpoint_sway;
  int counter_sway = 0;
  int counter_depth = 0;

  TOPSTATE(Top) {

    createStateBox(TaskMarker);

    STATE(Top);

    createMachineFunctions();

    virtual void run() {}

    void initialize() { setState<Init>(); }

    createPointerFunctions(TaskMarker);
  };

  SUBSTATE(Init, Top) {

    STATE(Init);

    void run();
  };

  Macho::Machine<TaskMarker::Top> state_marker_;

  createQueue(TaskMarker, state_marker_);
};

#endif
