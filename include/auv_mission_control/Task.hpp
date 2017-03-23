#ifndef TASK_H
#define TASK_H

#include <cstdarg>
#include <ros/ros.h>
#include <stdio.h>

#include <auv_mission_control/Macho.hpp>
#include <boost/type_index.hpp>

class StateMachine;
class PointerHandler;

enum class INPUT { CAM_FRONT, CAM_BOTTOM, IMU_POS, IMU_ACCEL, DEPTH };
enum class AXIS { SURGE, SWAY, HEAVE, ROLL, PITCH, YAW };

#define SUB_SUCCEEDED 0
#define SUB_TIMEOUT 1
#define SUB_KILL 2

#define MAXWIDTH 7

// #define AUV_BOX(Class)                                                         \
//   struct Box {                                                                 \
//     Box() {}                                                                   \
//     std::shared_ptr<decltype(*this)> self_;                                    \
//   };

#define AUV_TOPSTATE(Top) TOPSTATE(Top), Task

#define AUV_SUBSTATE(STATE, SUPERSTATE) SUBSTATE(STATE, SUPERSTATE)

#define AUV_STATE(S)                                                           \
  STATE(S);                                                                    \
  virtual std::string getStateTag() {                                          \
    return std::string("[State ") + std::string(#S) + std::string("]");        \
  }                                                                            \
                                                                               \
  std::string getTag() { return TOP::box().self_->getTag(); }

#define AUV_TSTATE(S)                                                          \
  TSTATE(S);                                                                   \
  virtual std::string getStateTag() {                                          \
    return std::string("[State ") + std::string(#S) + std::string("]");        \
  }                                                                            \
                                                                               \
  std::string getTag() { return TOP::box().self_->getTag(getStateTag()); }

#define createTaskTag(Class)                                                   \
  std::string getTag() {                                                       \
    std::stringstream ss;                                                      \
    std::string task =                                                         \
        std::string(#Class).substr(4, std::string(#Class).length()) +          \
        std::string("]");                                                      \
                                                                               \
    ss << std::string("[") << std::right << std::setw(MAXWIDTH)                \
       << std::setfill(' ') << task;                                           \
    std::string str = ss.str();                                                \
    std::transform(str.begin(), str.end(), str.begin(), ::toupper);            \
    return str + sm_->getStateTag();                                           \
  }                                                                            \
  std::string getTag(std::string state_tag) {                                  \
    std::stringstream ss;                                                      \
    std::string task =                                                         \
        std::string(#Class).substr(4, std::string(#Class).length()) +          \
        std::string("]");                                                      \
                                                                               \
    ss << std::string("[") << std::right << std::setw(MAXWIDTH)                \
       << std::setfill(' ') << task;                                           \
    std::string str = ss.str();                                                \
    std::transform(str.begin(), str.end(), str.begin(), ::toupper);            \
    return str + state_tag;                                                    \
  }

#define createTaskFunctions(T)                                                 \
  std::shared_ptr<PointerHandler> ph_;                                         \
                                                                               \
  Macho::Machine<T::Top> sm_;                                                  \
                                                                               \
  createQueue(T, sm_);

#define createTopState(Class)                                                  \
                                                                               \
  int execute();                                                               \
                                                                               \
  virtual Class &self(void) { return *this; }                                  \
  virtual PointerHandler &ph(void) { return *ph_; }                            \
                                                                               \
  AUV_TOPSTATE(Top) {                                                          \
                                                                               \
    struct Box {                                                               \
      Box() {}                                                                 \
      std::shared_ptr<Class> self_;                                            \
    };                                                                         \
                                                                               \
    AUV_STATE(Top);                                                            \
                                                                               \
    createMachineFunctions();                                                  \
                                                                               \
    virtual void run() { setState<Init>(); }                                   \
                                                                               \
    Class &self() { return *box().self_; }                                     \
    PointerHandler &ph() { return *self().ph_; }                               \
                                                                               \
  private:                                                                     \
    void init(Class *self, std::shared_ptr<PointerHandler> ph) {               \
      box().self_ = std::shared_ptr<Class>(self);                              \
    }                                                                          \
  };

#define createState(State)                                                     \
  AUV_SUBSTATE(State, Top) {                                                   \
                                                                               \
    AUV_STATE(State);                                                          \
                                                                               \
    void run();                                                                \
  };

#define createNullState(State)                                                 \
  AUV_SUBSTATE(State, Top) {                                                   \
                                                                               \
    AUV_STATE(State);                                                          \
                                                                               \
    void run() {}                                                              \
  };

class Task {
public:
  Task();
  ~Task();

  void AUV_FATAL(const char *format, ...);
  void AUV_FATAL(std::string string, ...);

  void AUV_ERROR(const char *format, ...);
  void AUV_ERROR(std::string string, ...);

  void AUV_WARN(const char *format, ...);
  void AUV_WARN(std::string string, ...);

  void AUV_INFO(const char *format, ...);
  void AUV_INFO(std::string string, ...);

  void AUV_DEBUG(const char *format, ...);
  void AUV_DEBUG(std::string string, ...);

  virtual std::string getTag();

private:
protected:
  Task(const Task &task) {}
};

#define createMachineFunctions()                                               \
  template <class S> void setMachineState() { setState<S>(); }                 \
                                                                               \
  template <class S> void setMachineState(double wait_time) {                  \
    setState<S>(wait_time);                                                    \
  }                                                                            \
                                                                               \
  template <class S>                                                           \
  void setMachineState(std::vector<AXIS> axis, std::vector<double> values,     \
                       double wait_time) {                                     \
    setState<S>(axis, values, wait_time);                                      \
  }                                                                            \
                                                                               \
  template <class S>                                                           \
  void setMachineState(std::vector<AXIS> axis, std::vector<double> values,     \
                       double wait_time, Macho::Alias alias) {                 \
    setState<S>(axis, values, wait_time, alias);                               \
  }                                                                            \
                                                                               \
  void setMachineStateAlias(Macho::Alias alias) { setState(alias); }

#define createQueue(T, state)                                                  \
public:                                                                        \
  typedef std::vector<Macho::IEvent<typename T::Top> *> EventQueue;            \
  EventQueue eventqueue_;                                                      \
                                                                               \
  bool eventqueue_should_switch_ = false;                                      \
  bool eventqueue_last_empty_ = false;                                         \
                                                                               \
  void queueEnable() { eventqueue_should_switch_ = true; }                     \
                                                                               \
  void queueDisable() { eventqueue_should_switch_ = false; }                   \
                                                                               \
  template <class S> void queueState() {                                       \
    eventqueue_.push_back(Macho::Event(&T::Top::setMachineState<S>));          \
  }                                                                            \
                                                                               \
  template <class S> void queueState(double wait_time) {                       \
    eventqueue_.push_back(                                                     \
        Macho::Event(&T::Top::setMachineState<S>, wait_time));                 \
  }                                                                            \
                                                                               \
  template <class S>                                                           \
  void queueState(std::vector<AXIS> axis, std::vector<double> values,          \
                  double wait_time) {                                          \
    eventqueue_.push_back(                                                     \
        Macho::Event(&T::Top::setMachineState<S>, axis, values, wait_time));   \
  }                                                                            \
                                                                               \
  template <class S>                                                           \
  void queueState(std::vector<AXIS> axis, std::vector<double> values,          \
                  double wait_time, Macho::Alias alias) {                      \
    eventqueue_.push_back(Macho::Event(&T::Top::setMachineState<S>, axis,      \
                                       values, wait_time, alias));             \
  }                                                                            \
                                                                               \
  void queueStateAlias(Macho::Alias alias) {                                   \
    eventqueue_.push_back(Macho::Event(&T::Top::setMachineStateAlias, alias)); \
  }                                                                            \
                                                                               \
  int checkEventQueue() {                                                      \
    if (eventqueue_.empty()) {                                                 \
      eventqueue_last_empty_ = true;                                           \
      eventqueue_should_switch_ = false;                                       \
      return 1;                                                                \
    }                                                                          \
                                                                               \
    if (eventqueue_last_empty_ || eventqueue_should_switch_) {                 \
      state.dispatch(eventqueue_.at(0));                                       \
                                                                               \
      eventqueue_.erase(eventqueue_.begin());                                  \
      if (eventqueue_last_empty_)                                              \
        eventqueue_should_switch_ = false;                                     \
      eventqueue_last_empty_ = false;                                          \
      return 0;                                                                \
    }                                                                          \
                                                                               \
    return 1;                                                                  \
  }

#endif
