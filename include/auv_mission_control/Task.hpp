#ifndef TASK_H
#define TASK_H

#include <cstdarg>
#include <ros/ros.h>
#include <stdio.h>

#include <auv_mission_control/Macho.hpp>

class StateMachine;

enum class INPUT { CAM_FRONT, CAM_BOTTOM, IMU_POS, IMU_ACCEL, DEPTH };
enum class AXIS { SURGE, SWAY, HEAVE, ROLL, PITCH, YAW };

#define SUB_SUCCEEDED 0
#define SUB_TIMEOUT 1
#define SUB_KILL 2

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
  }                                                                            \
  std::string getTag() { return getTaskTag(); }

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

#define createPointerFunctions(class)                                          \
  void setPointer(std::shared_ptr<PidManager> pm) { box().pm_ = pm; }          \
                                                                               \
  void setPointer(std::shared_ptr<Vision> vision) { box().vision_ = vision; }  \
                                                                               \
  void setPointer(std::shared_ptr<StateMachine> statemachine) {                \
    box().statemachine_ = statemachine;                                        \
  }                                                                            \
                                                                               \
  void setPointer(std::shared_ptr<class> test) { box().self_ = test; }

#define createStateBox(class)                                                  \
  struct Box {                                                                 \
    Box() {}                                                                   \
    std::shared_ptr<StateMachine> statemachine_;                               \
    std::shared_ptr<PidManager> pm_;                                           \
    std::shared_ptr<Vision> vision_;                                           \
    std::shared_ptr<class> self_;                                              \
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
  Task(const Task &task) { /*...*/
  }
};

#endif
