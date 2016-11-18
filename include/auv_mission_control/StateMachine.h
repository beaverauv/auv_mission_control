#ifndef STATEMACHINE_H
#define STATEMACHINE_H


#include <auv_mission_control/Task.h>
#include <auv_mission_control/Camera.h>
#include <auv_mission_control/PidManager.h>
#include <auv_mission_control/TaskVision.h>
#include <auv_mission_control/TaskGate.h>
#include <auv_mission_control/TaskMarker.h>
#include <auv_mission_control/TaskBuoy.h>




class StateMachine : public Task {
public:
        StateMachine();
        ~StateMachine();

        std::string getTag(){
                return std::string("[State Machine]");
        }

        int execute();

private:
        ros::NodeHandle nh_;
        PidManager* pm_;
        Camera* cam_;
        TaskVision* vision_;
        //TaskGate* gate_;
        //TaskMarker* marker_;
        TaskBuoy* buoy_;

};






#endif
