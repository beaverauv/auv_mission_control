#ifndef KILL_H
#define KILL_H

class Task_Gate{
public:
  Task_Gate();
  Task_Gate(PidManager* pm);
  ~Task_Gate();

  int execute();

private:
  //variables go here;
  PidManager pm_;
  CameraManager cam_;
};
