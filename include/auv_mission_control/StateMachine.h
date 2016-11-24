#ifndef STATEMACHINE_H
#define STATEMACHINE_H


#include <auv_mission_control/Task.h>
#include <auv_mission_control/Macho.hpp>
//#include <auv_mission_control/State.hpp>
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

        struct Top : public Macho::Link< Top, Macho::TopBase< Top >  >, Task {
                std::string getTag(){
                        return std::string("[State]");
                }


                //TOPSTATE(Top) {
                // Top state variables (visible to all substates)
                struct Box {
                        Box() : data(0){
                        }
                        long data;
                };

                STATE(Top)

                virtual void run(){

                }

                void initalize(){
                        AUV_DEBUG("Initalize");
                        setState<Init>();

                }

private:
                // special actions
                void entry(){
                        AUV_DEBUG("Top::entry");
                }
                void exit(){
                        AUV_DEBUG("Top::exit");
                }
        };

// A substate
        SUBSTATE(Init, Top) {
                // State variables
                struct Box {
                        Box() : data(0) {
                        }
                        int data;
                };

                STATE(Init)

                // Event handler
                void run();


private:
                void entry(){
                        AUV_DEBUG("Init::entry");
                }
                void exit(){
                        AUV_DEBUG("Init::exit");
                }

        };

// A substate
        SUBSTATE(Kill, Top) {


                STATE(Kill)

                void run();

private:
                void entry(){
                        AUV_DEBUG("Kill::entry");
                }
                void exit(){
                        AUV_DEBUG("Kill::exit");
                }
        };

        SUBSTATE(Timer, Top) {

                struct Box {
                        Box() : waitTime(0), currentState(Top::alias()), startTime(0) {
                        }
                        double waitTime;
                        Macho::Alias currentState;
                        double startTime;
                };

                STATE(Timer)

                void run();


private:
                void entry(){
                        AUV_DEBUG("Timer::entry");
                }
                void exit(){
                        AUV_DEBUG("Timer::exit");
                }
                void init(double waitTime, Macho::Alias currentState){
                        box().waitTime = waitTime;
                        box().currentState = currentState;
                        box().startTime = ros::Time::now().toSec();
                }
        };

        Macho::Machine<StateMachine::Top> state_;


};






#endif
