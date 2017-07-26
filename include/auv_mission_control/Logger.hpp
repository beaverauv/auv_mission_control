#ifndef TASKBASE_H
#define TASKBASE_H

#include <cstdarg>
#include <ros/ros.h>
#include <stdio.h>
namespace Mission {
class Base;
};

namespace Task {
class Test;
class Example;
class Gate;
class Buoy;
class Marker;
}

class BackplaneManager;
class PidManager;
class PointerHandler;
class StateMachine;
class Camera;
class Vision;

class Logger {
public:
  Logger(std::shared_ptr<PointerHandler> ph) : ph_(ph) {}
  Logger() {}
  ~Logger() {}

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

  void AUV_HERE();
  void AUV_HERE(int num);

  virtual std::string getTag();

  std::shared_ptr<PointerHandler> ph();
  std::shared_ptr<StateMachine> sm();
  std::shared_ptr<Mission::Base> mission();
  std::shared_ptr<PidManager> pm();
  std::shared_ptr<BackplaneManager> bm();
  std::shared_ptr<Camera> cam();
  std::shared_ptr<Vision> vision();
  std::shared_ptr<Task::Test> test();
  std::shared_ptr<Task::Example> example();
  std::shared_ptr<Task::Gate> gate();
  std::shared_ptr<Task::Buoy> buoy();
  std::shared_ptr<Task::Marker> marker();

  std::shared_ptr<PointerHandler> ph_;

private:
protected:
  Logger(const Logger &task) {}
};

#endif
