#ifndef TASKMARKER_H
#define TASKMARKER_H

#include <cmath>
#include <unistd.h>

#include <auv_mission_control/Task.h>
#include <auv_mission_control/Timer.h>
#include <auv_mission_control/Vision.h>
#include <auv_mission_control/PidManager.h>
#include <auv_mission_control/Camera.h>
//#include <auv_mission_control/StateMachine.h>



class TaskMarker : public Task {
public:
        TaskMarker();
        TaskMarker(std::shared_ptr<PidManager> pm, std::shared_ptr<Vision> vision);
        ~TaskMarker();

        std::string getTag(){
                return std::string("[Task Marker]");
        }



        int execute();
        void prepare();

private:
        //variables go here;
        // Timer swayTimer;
        // Timer depthTimer;
        // Timer yawTimer;
        int action = 0;
        bool objectFound;
        double plantState_sway;
        double setpoint_sway;
        int counter_sway = 0;
        int counter_depth = 0;

        TOPSTATE(Top) {
                std::string getTag(){
                        return std::string("[StateMarker]");
                }


                //TOPSTATE(Top) {
                // Top state variables (visible to all substates)
                struct Box {
                        Box() : pm_(0), vision_(0){
                        }
                        std::shared_ptr<PidManager> pm_;
                        std::shared_ptr<Vision> vision_;
                };

                STATE(Top)

                virtual void run(){

                }

                void initialize(){
                        setState<Init>();
                }
                void setLocalPointers(std::shared_ptr<PidManager> pm, std::shared_ptr<Vision> vision){
                        box().pm_ = pm;
                        box().vision_ = vision;
                }

private:
                void entry(){
                        AUV_DEBUG("Top::entry");
                }
        };

        SUBSTATE(Init, Top) {
                // State variables

                STATE(Init)
                // Event handler
                void run();
private:
                void entry(){
                        AUV_DEBUG("Init::entry");
                }
        };

        Macho::Machine<TaskMarker::Top> stateMarker_;


};

#endif
