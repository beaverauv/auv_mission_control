#ifndef Timer_H
#define Timer_H

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
