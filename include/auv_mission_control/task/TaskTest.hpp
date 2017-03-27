#ifndef TaskTest_H
#define TaskTest_H

#include <auv_mission_control/task/Task.hpp>

class TaskTest : public Task {
public:
  TaskTest(std::shared_ptr<PointerHandler> ph)
      : ph_(ph), sm_(Macho::State<Top>(this, ph_)) {}

  ~TaskTest() {}

  int execute();

  virtual TaskTest &self(void) { return *this; }
  virtual PointerHandler &ph(void) { return *ph_; }

  AUV_LOG_TAG(TaskTest);

  AUV_TOPSTATE(Top) {

    struct Box {
      Box() {}
      std::shared_ptr<TaskTest> self_;
    };

    AUV_STATE(Top);

    AUV_MACHINE_FUNCTIONS();

    virtual void run() { setState<Init>(); }

    TaskTest &self() { return *box().self_; }
    PointerHandler &ph() { return *self().ph_; }

  private:
    void init(TaskTest * self, std::shared_ptr<PointerHandler> ph) {
      box().self_ = std::shared_ptr<TaskTest>(self);
    }
  };

  AUV_CREATE_STATE(Init);

  AUV_CREATE_EMPTY_STATE(Nowhere);

  AUV_SUBSTATE(Whatever, Top) {

    AUV_STATE(Whatever);

    void run();
  };

  std::shared_ptr<PointerHandler> ph_;

  Macho::Machine<Top> sm_;

  AUV_CREATE_QUEUE(TaskTest, sm_);
};

#endif
