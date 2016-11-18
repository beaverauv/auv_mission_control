#ifndef TASKKILL_H
#define TASKKILL_H

#include <auv_mission_control/PidManager.h>
#include <auv_mission_control/Task.h>


class TaskKill : public Task {
public:
        TaskKill();
        TaskKill(PidManager* pm);
        ~TaskKill();

        int execute();

        std::string getTag(){
                return std::string("[Task Kill]");
        }

private:
        //variables go here;
        PidManager* pm_;
};

#endif
