#ifndef MISSIONBASE_H_
#define MISSIONBASE_H_

#include <auv_mission_control/Task.hpp>

namespace Mission {

class Base : public Logger {
public:
  Base(std::shared_ptr<PointerHandler> ph) : ph_(ph) {}

  ~Base() {}

  virtual Base &self(void) { return *this; }
  virtual PointerHandler &ph(void) { return *ph_; }

  AUV_TOPSTATE(Top) {

    struct Box {
      Box() {}
      std::shared_ptr<Base> self_;
    };

    AUV_STATE(Top);

    virtual void run() {}

    Base &self(void) { return *box().self_; }
    PointerHandler &ph(void) { return self().ph(); }

  private:
    void init(Base * self);
  };

  virtual int execute() {}

  virtual void queueEnable() {}

  virtual void queueDisable() {}

  void virtual queueStateAlias(Macho::Alias alias) {}

  std::shared_ptr<PointerHandler> ph_;
  Macho::Machine<Base::Top> sm_;
};
}

#endif
