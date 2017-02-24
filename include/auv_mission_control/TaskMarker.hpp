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

  std::string getTag() { return std::string("[Task Marker]"); }

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
    std::string getTag() { return std::string("[StateMarker]"); }

    // TOPSTATE(Top) {
    // Top state variables (visible to all substates)
    struct Box {
      Box() : pm_(0), vision_(0), statemachine_(0) {}
      std::shared_ptr<StateMachine> statemachine_;
      std::shared_ptr<TaskMarker> self_;
      std::shared_ptr<PidManager> pm_;
      std::shared_ptr<Vision> vision_;
      // template<class S>
      // Macho::IEvent<S> * event_;
    };

    STATE(Top);

    createMachineFunctions();

    virtual void run() {}

    void initialize() { setState<Init>(); }

    createPointerFunctions(TaskMarker);

  private:
    void entry() { AUV_DEBUG("Top::entry"); }
  };

  SUBSTATE(Init, Top) {
    // State variables

    STATE(Init);
    // Event handler
    void run();

  private:
    void entry() { AUV_DEBUG("Init::entry"); }
  };

  Macho::Machine<TaskMarker::Top> state_marker_;

  createQueue(TaskMarker, state_marker_);
};

#endif
