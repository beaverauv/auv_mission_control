#include <auv_mission_control/Logger.hpp>
#include <auv_mission_control/TemplateStates.hpp>

std::shared_ptr<PointerHandler> Logger::ph() { return ph_; }
std::shared_ptr<StateMachine> Logger::sm() { return ph()->sm_; }
std::shared_ptr<Mission::Base> Logger::mission() { return ph()->mission(); }
std::shared_ptr<PidManager> Logger::pm() { return ph()->pm_; }
std::shared_ptr<Camera> Logger::cam() { return ph()->cam_; }
std::shared_ptr<Vision> Logger::vision() { return ph()->vision_; }
std::shared_ptr<Task::Test> Logger::test() { return ph()->test_; }
std::shared_ptr<Task::Example> Logger::example() { return ph()->example_; }
std::shared_ptr<Task::Gate> Logger::gate() { return ph()->gate_; }
std::shared_ptr<Task::Buoy> Logger::buoy() { return ph()->buoy_; }
std::shared_ptr<Task::Marker> Logger::marker() { return ph()->marker_; }

void Logger::AUV_FATAL(const char *format, ...) {
  char buffer[1024];
  std::va_list args;
  va_start(args, format);
  vsnprintf(buffer, 1023, format, args);

  ROS_FATAL("%s %s", getTag().c_str(), buffer);
  // do  something with the error

  va_end(args);
}

void Logger::AUV_FATAL(std::string string, ...) {
  const char *format = string.c_str();
  char buffer[1024];
  std::va_list args;
  va_start(args, string);
  vsnprintf(buffer, 1023, format, args);

  ROS_FATAL("%s %s", getTag().c_str(), buffer);
  // do  something with the error

  va_end(args);
}

void Logger::AUV_ERROR(const char *format, ...) {
  char buffer[1024];
  std::va_list args;
  va_start(args, format);
  vsnprintf(buffer, 1023, format, args);

  ROS_ERROR("%s %s", getTag().c_str(), buffer);
  // do  something with the error

  va_end(args);
}

void Logger::AUV_ERROR(std::string string, ...) {
  const char *format = string.c_str();
  char buffer[1024];
  std::va_list args;
  va_start(args, string);
  vsnprintf(buffer, 1023, format, args);

  ROS_ERROR("%s %s", getTag().c_str(), buffer);
  // do  something with the error

  va_end(args);
}

void Logger::AUV_WARN(const char *format, ...) {
  char buffer[1024];
  std::va_list args;
  va_start(args, format);
  vsnprintf(buffer, 1023, format, args);

  ROS_WARN("%s %s", getTag().c_str(), buffer);
  // do  something with the error

  va_end(args);
}

void Logger::AUV_WARN(std::string string, ...) {
  const char *format = string.c_str();
  char buffer[1024];
  std::va_list args;
  va_start(args, string);
  vsnprintf(buffer, 1023, format, args);

  ROS_WARN("%s %s", getTag().c_str(), buffer);
  // do  something with the error

  va_end(args);
}

void Logger::AUV_INFO(const char *format, ...) {
  char buffer[1024];
  std::va_list args;
  va_start(args, format);
  vsnprintf(buffer, 1023, format, args);

  ROS_INFO("%s %s", getTag().c_str(), buffer);
  // do  something with the error

  va_end(args);
}

void Logger::AUV_INFO(std::string string, ...) {
  const char *format = string.c_str();
  char buffer[1024];
  std::va_list args;
  va_start(args, string);
  vsnprintf(buffer, 1023, format, args);

  ROS_INFO("%s %s", getTag().c_str(), buffer);
  // do  something with the error

  va_end(args);
}

void Logger::AUV_DEBUG(const char *format, ...) {
  char buffer[1024];
  std::va_list args;
  va_start(args, format);
  vsnprintf(buffer, 1023, format, args);

  ROS_DEBUG("%s %s", getTag().c_str(), buffer);
  // do  something with the error

  va_end(args);
}

void Logger::AUV_DEBUG(std::string string, ...) {
  const char *format = string.c_str();
  char buffer[1024];
  std::va_list args;
  va_start(args, string);
  vsnprintf(buffer, 1023, format, args);

  ROS_DEBUG("%s %s", getTag().c_str(), buffer);
  // do  something with the error

  va_end(args);
}

std::string Logger::getTag() { return std::string("[Default Task]"); }
