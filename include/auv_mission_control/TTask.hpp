#ifndef TTASK_H_
#define TTASK_H_

#include <auv_mission_control/Macho.hpp>
#include <auv_mission_control/Task.hpp>

// template <class T> class TTask_first {
//   TTask_first();
//   ~TTask_first();
// };
//
// template <class T> TTask_first<T>::TTask_first() {}
//
// template <class T> TTask_first<T>::~TTask_first() {}
//
template <class T> class TTask {
public:
  TTask();
  ~TTask();

  // template <class S> void queueState();
  //
  // template <class S>
  // void queueState(std::vector<AXIS> axis, std::vector<double> values,
  //                 double wait_time);
  //
  // void queueStateAlias(Macho::Alias alias);
  //
  // int checkEventQueue();

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

  // QUEUE(T)

private:
  // Macho::Machine<typename T::Top> state_;
  // template <class C>
  // std::vector<Macho::IEvent<typename T::Top> *> eventqueue_;
  // typedef Macho::IEvent<typename T::Top> EventQueue;

  // template <class C>
  // using EventQueueu =
  //     typename decltype(std::declval<T>().create())::element_type;

  // typedef
  //     typename decltype(std::declval<T>().create())::element_type
  //     EventQueue;
  //
  // EventQueue<T> eventqueue_;

  // bool eventqueue_should_switch_ = false;
  // bool eventqueue_last_empty_ = false;

protected:
  TTask(const TTask &task) { /*...*/
  }
};

template <class T> TTask<T>::TTask() {}

template <class T> TTask<T>::~TTask() {}

// template <class S> void TTask<T>::queueState() {
//   eventqueue_.push_back(Macho::Event(&TTask<T>::Top::setMachineState<S>));
// }
//
// template <class S>
// void TTask<T>::queueState(std::vector<AXIS> axis, std::vector<double> values,
//                       double wait_time) {
//   eventqueue_.push_back(
//       Macho::Event(&TTask<T>::Top::setMachineState<S>, axis, values,
//       wait_time));
// }
//
// void TTask<T>::queueStateAlias(Macho::Alias alias) {
//   eventqueue_.push_back(Macho::Event(&TTask<T>::Top::setMachineStateAlias,
//   alias));
// }
//
// int TTask<T>::checkEventQueue() {
//   // ROS_INFO("queue size: %d, last empty: %d, should switch: %d",
//   //          eventqueue_.size(), eventqueue_last_empty_,
//   //          eventqueue_should_switch_);
//
//   if (eventqueue_.empty()) {
//     eventqueue_last_empty_ = true;
//     eventqueue_should_switch_ = false;
//     return 1;
//   }
//
//   // eventqueue_last_empty_ = false;
//
//   if (eventqueue_last_empty_ || eventqueue_should_switch_) {
//     state_.dispatch(eventqueue_.at(0));
//
//     eventqueue_.erase(eventqueue_.begin());
//     if (eventqueue_last_empty_)
//       eventqueue_should_switch_ = false;
//     eventqueue_last_empty_ = false;
//     return 0;
//   }
//
//   return 1;
// }

template <class T> void TTask<T>::AUV_FATAL(const char *format, ...) {
  char buffer[1024];
  std::va_list args;
  va_start(args, format);
  vsnprintf(buffer, 1023, format, args);

  ROS_FATAL("%s %s", getTag().c_str(), buffer);
  // do  something with the error

  va_end(args);
}

template <class T> void TTask<T>::AUV_FATAL(std::string string, ...) {
  const char *format = string.c_str();
  char buffer[1024];
  std::va_list args;
  va_start(args, string);
  vsnprintf(buffer, 1023, format, args);

  ROS_FATAL("%s %s", getTag().c_str(), buffer);
  // do  something with the error

  va_end(args);
}

template <class T> void TTask<T>::AUV_ERROR(const char *format, ...) {
  char buffer[1024];
  std::va_list args;
  va_start(args, format);
  vsnprintf(buffer, 1023, format, args);

  ROS_ERROR("%s %s", getTag().c_str(), buffer);
  // do  something with the error

  va_end(args);
}

template <class T> void TTask<T>::AUV_ERROR(std::string string, ...) {
  const char *format = string.c_str();
  char buffer[1024];
  std::va_list args;
  va_start(args, string);
  vsnprintf(buffer, 1023, format, args);

  ROS_ERROR("%s %s", getTag().c_str(), buffer);
  // do  something with the error

  va_end(args);
}

template <class T> void TTask<T>::AUV_WARN(const char *format, ...) {
  char buffer[1024];
  std::va_list args;
  va_start(args, format);
  vsnprintf(buffer, 1023, format, args);

  ROS_WARN("%s %s", getTag().c_str(), buffer);
  // do  something with the error

  va_end(args);
}

template <class T> void TTask<T>::AUV_WARN(std::string string, ...) {
  const char *format = string.c_str();
  char buffer[1024];
  std::va_list args;
  va_start(args, string);
  vsnprintf(buffer, 1023, format, args);

  ROS_WARN("%s %s", getTag().c_str(), buffer);
  // do  something with the error

  va_end(args);
}

template <class T> void TTask<T>::AUV_INFO(const char *format, ...) {
  char buffer[1024];
  std::va_list args;
  va_start(args, format);
  vsnprintf(buffer, 1023, format, args);

  ROS_INFO("%s %s", getTag().c_str(), buffer);
  // do  something with the error

  va_end(args);
}

template <class T> void TTask<T>::AUV_INFO(std::string string, ...) {
  const char *format = string.c_str();
  char buffer[1024];
  std::va_list args;
  va_start(args, string);
  vsnprintf(buffer, 1023, format, args);

  ROS_INFO("%s %s", getTag().c_str(), buffer);
  // do  something with the error

  va_end(args);
}

template <class T> void TTask<T>::AUV_DEBUG(const char *format, ...) {
  char buffer[1024];
  std::va_list args;
  va_start(args, format);
  vsnprintf(buffer, 1023, format, args);

  ROS_DEBUG("%s %s", getTag().c_str(), buffer);
  // do  something with the error

  va_end(args);
}

template <class T> void TTask<T>::AUV_DEBUG(std::string string, ...) {
  const char *format = string.c_str();
  char buffer[1024];
  std::va_list args;
  va_start(args, string);
  vsnprintf(buffer, 1023, format, args);

  ROS_DEBUG("%s %s", getTag().c_str(), buffer);
  // do  something with the error

  va_end(args);
}

template <class T> std::string TTask<T>::getTag() {
  return std::string("[Default TTask]");
}

#endif
