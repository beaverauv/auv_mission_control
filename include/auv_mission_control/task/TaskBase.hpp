#ifndef TASKBASE_H
#define TASKBASE_H

#include <cstdarg>
#include <ros/ros.h>
#include <stdio.h>

class TaskBase {
public:
  TaskBase();
  ~TaskBase();

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
  TaskBase(const TaskBase &task) {}
};

#endif
