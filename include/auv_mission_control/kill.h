#ifndef KILL_H
#define KILL_H

class Task_Gate{
public:
  Task_Gate();
  Task_Gate(Pid_Manager* pm);
  ~Task_Gate();

  int execute();

private:
  //variables go here;
  Pid_Manager pm_;
  CameraManager cam_;
};
