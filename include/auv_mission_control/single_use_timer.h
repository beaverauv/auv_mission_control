#ifndef SINGLE_USE_TIMER_H
#define SINGLE_USE_TIMER_H

#include "ros/ros.h"

class Timer{

private:
  double startTime;

public:
  Timer();
  ~Timer();

  void start();
  double getTime();



};

#endif
