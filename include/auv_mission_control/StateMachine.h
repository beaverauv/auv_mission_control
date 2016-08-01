#ifndef STATEMACHINE_H
#define STATEMACHINE_H

#include <auv_mission_control/Task.h>
#include <auv_mission_control/Camera.h>
#include <auv_mission_control/PidManager.h>
#include <auv_mission_control/TaskGate.h>
#include <auv_mission_control/TaskMarker.h>
#include <auv_mission_control/TaskBuoy.h>

#define succeeded 0
#define timeout 1
#define kill 2


class StateMachine : public Task {
public:
        StateMachine();
        ~StateMachine();

        std::string getTag();

        int execute();

private:
        ros::NodeHandle nh_;
        PidManager pm_;
        Camera cam_;
        TaskVision vision_;
        //TaskGate gate_;


};






#endif
