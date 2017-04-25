#ifndef MISSIONBASE_H_
#define MISSIONBASE_H_

#include <auv_mission_control/Task.hpp>

namespace Mission {

class Base : public Logger {
public:
  Base(std::shared_ptr<PointerHandler> ph) : INHERITED(ph) {}

  ~Base() {}

  auto self() { return this; }

  AUV_TOPSTATE(Top) {

    struct Box {
      Box() {}
      std::shared_ptr<Base> self_;
    };

    AUV_STATE(Top);

    virtual void run() {}

    std::shared_ptr<Base> self() { return box().self_; }
    std::shared_ptr<PointerHandler> ph() { return self()->ph(); }

  private:
    void init(Base * self);
  };

  virtual int execute() {}

  virtual void queueEnable() {}

  virtual void queueDisable() {}

  void virtual queueStateAlias(Macho::Alias alias) {}

  Macho::Machine<Base::Top> sm_;
  typedef Logger INHERITED;
};
}

#endif
