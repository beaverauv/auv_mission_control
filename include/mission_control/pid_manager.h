#ifndef PID_MANAGER_H
#define PID_MANAGER_H
#include <string>

class Pid_Manager{

public:
  void setpoint_set(std::string axis, std::string input_type, double value);
  void plantState_get(std::string axis);
  void getDepth();
  void zero(std::string sensor);
  void pidInit_all();
  void pidEnable(std::string axis, bool enabled);
  void onlyPID_set(bool state);
  ~Pid_Manager();
  Pid_Manager();

};

#endif
