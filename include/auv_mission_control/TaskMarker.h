#ifndef TASKMARKER_H
#define TASKMARKER_H

#include <cmath>
#include <unistd.h>

#include <auv_mission_control/Task.h>
#include <auv_mission_control/PidManager.h>
#include <auv_mission_control/Camera.h>
//#include <auv_mission_control/StateMachine.h>



class TaskMarker : public Task {
public:
        TaskMarker();
        TaskMarker(PidManager* pm, TaskVision* vision);
        ~TaskMarker();

        std::string getTag(){
                return std::string("[Task Marker]");
        }



        int execute();

private:
        //variables go here;
        PidManager* pm_;
        TaskVision* vision_;
        Timer swayTimer;
        Timer depthTimer;
        Timer yawTimer;
        int action = 0;
        bool objectFound;
        double plantState_sway;
        double setpoint_sway;
        int counter_sway = 0;
        int counter_depth = 0;


};

#endif
