#include "task.h"

task::Task() {
  for(int i = 0; i < taskList.size(); i++){
    delete taskList[i];
  }
}
