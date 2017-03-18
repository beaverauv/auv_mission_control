#ifndef TaskTest_H
#define TaskTest_H

#include <auv_mission_control/Task.hpp>

class TaskTest : public Task {
public:
  TaskTest(std::shared_ptr<PointerHandler> ph)
      : ph_(ph), sm_(Macho::State<Top>(this, ph_)) {}

  ~TaskTest() {}

  int execute();

  virtual TaskTest &self(void) { return *this; }
  virtual PointerHandler &ph(void) { return *ph_; }

  createTaskTag(TaskTest);

  TOPSTATE(Top) {

    struct Box {
      Box() {}
      std::shared_ptr<TaskTest> self_;
    };

    STATE(Top);

    createMachineFunctions();

    virtual void run() { setState<Init>(); }

    TaskTest &self() { return *box().self_; }
    PointerHandler &ph() { return *self().ph_; }

  private:
    void init(TaskTest * self, std::shared_ptr<PointerHandler> ph) {
      box().self_ = std::shared_ptr<TaskTest>(self);
    }
  };

  createState(Init);

  createNullState(Nowhere);

  SUBSTATE(Whatever, Top) {

    STATE(Whatever);

    void run();
  };

  std::shared_ptr<PointerHandler> ph_;

  Macho::Machine<Top> sm_;

  createQueue(TaskTest, sm_);
};

#endif
