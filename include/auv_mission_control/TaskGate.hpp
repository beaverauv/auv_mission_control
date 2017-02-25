#ifndef TASKGATE_H
#define TASKGATE_H

#include <cmath>
#include <iostream>
#include <memory>
#include <unistd.h>

#include <auv_mission_control/Macho.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

// #include <auv_mission_control/Timer.hpp>
#include <auv_mission_control/Camera.hpp>
#include <auv_mission_control/PidManager.hpp>
#include <auv_mission_control/Task.hpp>
#include <auv_mission_control/Vision.hpp>
//#include <auv_mission_control/StateMachine.hpp>

class StateMachine;

class TaskGate : public Task {
public:
  TaskGate();
  TaskGate(std::shared_ptr<PidManager> pm, std::shared_ptr<Vision> vision);
  ~TaskGate();

  std::string getTag() { return std::string("[Task Gate]"); }

  int execute();
  void prepare(std::shared_ptr<StateMachine> statemachine);

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

  TOPSTATE(Top) {
    std::string getTag() { return std::string("[StateGate]"); }

    // TOPSTATE(Top) {
    // Top state variables (visible to all substates)
    createStateBox(TaskGate);

    STATE(Top);

    createMachineFunctions();

    virtual void run() {}

    void initialize() { setState<Init>(); }

    createPointerFunctions(TaskGate);

  private:
    void entry() { AUV_DEBUG("Top::entry"); }
  };

  SUBSTATE(Init, Top) {
    // State variables

    STATE(Init)
    // Event handler
    void run();

  private:
    void entry() { AUV_DEBUG("Init::entry"); }
  };

  Macho::Machine<TaskGate::Top> state_gate_;

  createQueue(TaskGate, state_gate_);
};

#endif
