#ifndef TASK_H
#define TASK_H

#include <cstdarg>
#include <ros/ros.h>
#include <stdio.h>

#include <auv_mission_control/Macho.hpp>

class StateMachine;

enum class INPUT { CAM_FRONT, CAM_BOTTOM, IMU_POS, IMU_VEL, DEPTH };
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
  template <class S>                                                           \
  void queueState(std::vector<AXIS> axis, std::vector<double> values,          \
                  double wait_time) {                                          \
    eventqueue_.push_back(                                                     \
        Macho::Event(&T::Top::setMachineState<S>, axis, values, wait_time));   \
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

#define createMachineFunctions()                                               \
  template <class S> void setMachineState() { setState<S>(); }                 \
                                                                               \
  template <class S>                                                           \
  void setMachineState(std::vector<AXIS> axis, std::vector<double> values,     \
                       double wait_time) {                                     \
    setState<S>(axis, values, wait_time);                                      \
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

  // template <class S> void queueState();
  //
  // template <class S>
  // void queueState(std::vector<AXIS> axis, std::vector<double> values,
  //                 double wait_time);

private:
protected:
  Task(const Task &task) { /*...*/
  }
};
//
// template <class T> class TTask {
// public:
//   TTask();
//   ~TTask();
//
//   // template <class S> void queueState();
//   //
//   // template <class S>
//   // void queueState(std::vector<AXIS> axis, std::vector<double> values,
//   //                 double wait_time);
//   //
//   // void queueStateAlias(Macho::Alias alias);
//   //
//   // int checkEventQueue();
//
//   void AUV_FATAL(const char *format, ...);
//   void AUV_FATAL(std::string string, ...);
//
//   void AUV_ERROR(const char *format, ...);
//   void AUV_ERROR(std::string string, ...);
//
//   void AUV_WARN(const char *format, ...);
//   void AUV_WARN(std::string string, ...);
//
//   void AUV_INFO(const char *format, ...);
//   void AUV_INFO(std::string string, ...);
//
//   void AUV_DEBUG(const char *format, ...);
//   void AUV_DEBUG(std::string string, ...);
//
//   virtual std::string getTag();
//
// private:
//   // typedef std::vector<Macho::IEvent<typename T::Top> *> EventQueue;
//   template <class C>
//   using EventQueue = std::vector<Macho::IEvent<typename C::Top> *>;
//
//   //
//   EventQueue<T> eventqueue_;
//   //
//   // bool eventqueue_should_switch_ = false;
//   // bool eventqueue_last_empty_ = false;
//
// protected:
//   TTask(const TTask &task) { /*...*/
//   }
// };
//
// template <class T> TTask<T>::TTask() {}
//
// template <class T> TTask<T>::~TTask() {}
//
// // template <class S> void TTask<T>::queueState() {
// //   eventqueue_.push_back(Macho::Event(&TTask<T>::Top::setMachineState<S>));
// // }
// //
// // template <class S>
// // void TTask<T>::queueState(std::vector<AXIS> axis, std::vector<double>
// values,
// //                       double wait_time) {
// //   eventqueue_.push_back(
// //       Macho::Event(&TTask<T>::Top::setMachineState<S>, axis, values,
// //       wait_time));
// // }
// //
// // void TTask<T>::queueStateAlias(Macho::Alias alias) {
// //   eventqueue_.push_back(Macho::Event(&TTask<T>::Top::setMachineStateAlias,
// //   alias));
// // }
// //
// // int TTask<T>::checkEventQueue() {
// //   // ROS_INFO("queue size: %d, last empty: %d, should switch: %d",
// //   //          eventqueue_.size(), eventqueue_last_empty_,
// //   //          eventqueue_should_switch_);
// //
// //   if (eventqueue_.empty()) {
// //     eventqueue_last_empty_ = true;
// //     eventqueue_should_switch_ = false;
// //     return 1;
// //   }
// //
// //   // eventqueue_last_empty_ = false;
// //
// //   if (eventqueue_last_empty_ || eventqueue_should_switch_) {
// //     state_.dispatch(eventqueue_.at(0));
// //
// //     eventqueue_.erase(eventqueue_.begin());
// //     if (eventqueue_last_empty_)
// //       eventqueue_should_switch_ = false;
// //     eventqueue_last_empty_ = false;
// //     return 0;
// //   }
// //
// //   return 1;
// // }
//
// template <class T> void TTask<T>::AUV_FATAL(const char *format, ...) {
//   char buffer[1024];
//   std::va_list args;
//   va_start(args, format);
//   vsnprintf(buffer, 1023, format, args);
//
//   ROS_FATAL("%s %s", getTag().c_str(), buffer);
//   // do  something with the error
//
//   va_end(args);
// }
//
// template <class T> void TTask<T>::AUV_FATAL(std::string string, ...) {
//   const char *format = string.c_str();
//   char buffer[1024];
//   std::va_list args;
//   va_start(args, string);
//   vsnprintf(buffer, 1023, format, args);
//
//   ROS_FATAL("%s %s", getTag().c_str(), buffer);
//   // do  something with the error
//
//   va_end(args);
// }
//
// template <class T> void TTask<T>::AUV_ERROR(const char *format, ...) {
//   char buffer[1024];
//   std::va_list args;
//   va_start(args, format);
//   vsnprintf(buffer, 1023, format, args);
//
//   ROS_ERROR("%s %s", getTag().c_str(), buffer);
//   // do  something with the error
//
//   va_end(args);
// }
//
// template <class T> void TTask<T>::AUV_ERROR(std::string string, ...) {
//   const char *format = string.c_str();
//   char buffer[1024];
//   std::va_list args;
//   va_start(args, string);
//   vsnprintf(buffer, 1023, format, args);
//
//   ROS_ERROR("%s %s", getTag().c_str(), buffer);
//   // do  something with the error
//
//   va_end(args);
// }
//
// template <class T> void TTask<T>::AUV_WARN(const char *format, ...) {
//   char buffer[1024];
//   std::va_list args;
//   va_start(args, format);
//   vsnprintf(buffer, 1023, format, args);
//
//   ROS_WARN("%s %s", getTag().c_str(), buffer);
//   // do  something with the error
//
//   va_end(args);
// }
//
// template <class T> void TTask<T>::AUV_WARN(std::string string, ...) {
//   const char *format = string.c_str();
//   char buffer[1024];
//   std::va_list args;
//   va_start(args, string);
//   vsnprintf(buffer, 1023, format, args);
//
//   ROS_WARN("%s %s", getTag().c_str(), buffer);
//   // do  something with the error
//
//   va_end(args);
// }
//
// template <class T> void TTask<T>::AUV_INFO(const char *format, ...) {
//   char buffer[1024];
//   std::va_list args;
//   va_start(args, format);
//   vsnprintf(buffer, 1023, format, args);
//
//   ROS_INFO("%s %s", getTag().c_str(), buffer);
//   // do  something with the error
//
//   va_end(args);
// }
//
// template <class T> void TTask<T>::AUV_INFO(std::string string, ...) {
//   const char *format = string.c_str();
//   char buffer[1024];
//   std::va_list args;
//   va_start(args, string);
//   vsnprintf(buffer, 1023, format, args);
//
//   ROS_INFO("%s %s", getTag().c_str(), buffer);
//   // do  something with the error
//
//   va_end(args);
// }
//
// template <class T> void TTask<T>::AUV_DEBUG(const char *format, ...) {
//   char buffer[1024];
//   std::va_list args;
//   va_start(args, format);
//   vsnprintf(buffer, 1023, format, args);
//
//   ROS_DEBUG("%s %s", getTag().c_str(), buffer);
//   // do  something with the error
//
//   va_end(args);
// }
//
// template <class T> void TTask<T>::AUV_DEBUG(std::string string, ...) {
//   const char *format = string.c_str();
//   char buffer[1024];
//   std::va_list args;
//   va_start(args, string);
//   vsnprintf(buffer, 1023, format, args);
//
//   ROS_DEBUG("%s %s", getTag().c_str(), buffer);
//   // do  something with the error
//
//   va_end(args);
// }
//
// template <class T> std::string TTask<T>::getTag() {
//   return std::string("[Default TTask]");
// }

#endif
