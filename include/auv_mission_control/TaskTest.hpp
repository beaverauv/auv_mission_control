#ifndef TaskTest_H
#define TaskTest_H

#include <auv_mission_control/Camera.hpp>
#include <auv_mission_control/Macho.hpp>
#include <auv_mission_control/PidManager.hpp>
// #include <auv_mission_control/PointerHandler.hpp>
#include <auv_mission_control/Task.hpp>
#include <auv_mission_control/Vision.hpp>
#include <memory>
#include <unistd.h>
//#include <auv_mission_control/StateMachine.hpp>

class StateMachine;
class PointerHandler;

class TaskTest : public Task {
public:
  TaskTest(std::shared_ptr<PointerHandler> ph);

  ~TaskTest() {}

  std::string getTaskTag() { return std::string("[Task Test]"); }

  int execute();

  TOPSTATE(Top) {

    struct Box {
      Box() {}
      std::shared_ptr<PointerHandler> ph_;
      std::shared_ptr<TaskTest> self_;
    };

    STATE(Top);

    createMachineFunctions();

    virtual void run() { setState<Init>(); }

  private:
    void init(TaskTest * self, std::shared_ptr<PointerHandler> ph);
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

  std::shared_ptr<PointerHandler> ph_;

  Macho::Machine<Top> state_test_;

  createQueue(TaskTest, state_test_);
};

#endif
