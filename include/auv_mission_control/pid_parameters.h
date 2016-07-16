#ifndef PID_PARAMETERS_H
#define PID_PARAMETERS_H


class Pid_Parameters{

public:
  double kp;
  double kd;
  double ki;
  virtual ~Pid_Parameters();

};

#endif
