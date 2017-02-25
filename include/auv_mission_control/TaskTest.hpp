#ifndef TaskTest_H
#define TaskTest_H

#include <auv_mission_control/Camera.hpp>
#include <auv_mission_control/Macho.hpp>
#include <auv_mission_control/PidManager.hpp>
#include <auv_mission_control/Task.hpp>
#include <auv_mission_control/Vision.hpp>
#include <memory>
#include <unistd.h>
//#include <auv_mission_control/StateMachine.hpp>

class StateMachine;

class TaskTest : public Task {
public:
  TaskTest();
  TaskTest(std::shared_ptr<PidManager> pm, std::shared_ptr<Vision> vision);
  ~TaskTest();

  std::string getTag() { return std::string("[Task Test]"); }
  int execute();
  void prepare(std::shared_ptr<StateMachine> statemachine);

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

  TOPSTATE(Top) {
    std::string getTag() { return std::string("[StateTest]"); }

    // TOPSTATE(Top) {
    // Top state variables (visible to all substates)
    createStateBox(TaskTest);

    STATE(Top);

    createMachineFunctions();

    virtual void run() {}

    void initialize() { setState<Init>(); }

    createPointerFunctions(TaskTest);

    virtual void here() { AUV_INFO("TOP HERE"); }

  private:
    void entry() { AUV_DEBUG("Top::entry"); }
  };

  SUBSTATE(Init, Top) {
    // State variables

    STATE(Init)
    // Event handler
    void run();
    void here() { AUV_INFO("HERE"); }

  private:
    void entry() { AUV_DEBUG("Init::entry"); }

    void init() {}
  };

  SUBSTATE(Nowhere, Top){

    STATE(Nowhere)

        void run(){}

    private : void entry(){}
  };

  SUBSTATE(Whatever, Top) {
    // State variables

    STATE(Whatever)
    // Event handler

    void run();

    void here() { AUV_INFO("WHATEVER HERE"); }

  private:
    void entry() { AUV_DEBUG("Init::entry"); }

    void init() { AUV_DEBUG("Whatever init"); }
  };

  Macho::Machine<TaskTest::Top> state_test_;

  createQueue(TaskTest, state_test_);
};

#endif
