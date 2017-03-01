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

  std::string getTaskTag() { return std::string("[Task Test]"); }

  int execute();
  void prepare(std::shared_ptr<StateMachine> statemachine);

private:
  TOPSTATE(Top) {

    createStateBox(TaskTest);

    STATE(Top);

    createMachineFunctions();

    virtual void run() {}

    void initialize() { setState<Init>(); }

    createPointerFunctions(TaskTest);
  };

  SUBSTATE(Init, Top) {

    STATE(Init);

    void run();
  };

  SUBSTATE(Nowhere, Top) {

    STATE(Nowhere);

    void run() {}
  };

  SUBSTATE(Whatever, Top) {

    STATE(Whatever);

    void run();
  };

  Macho::Machine<TaskTest::Top> state_test_;

  createQueue(TaskTest, state_test_);
};

#endif
