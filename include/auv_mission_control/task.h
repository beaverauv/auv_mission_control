#ifndef TASK_H
#define TASK_H
#include <vector>

class Task {
  public:
    virtual ~Task();

    virtual void excecute() = 0;

  protected:
      std::vector<Task*> taskList;
};

#endif
