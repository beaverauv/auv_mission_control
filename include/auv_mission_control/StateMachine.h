#ifndef STATEMACHINE_H
#define STATEMACHINE_H

#include <memory>

#include <auv_mission_control/Task.h>
#include <auv_mission_control/Timer.h>
#include <auv_mission_control/Macho.hpp>
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
        TOPSTATE(Top) {
                std::string getTag(){
                        return std::string("[State]");
                }


                struct Box {
                        Box() : pm_(new PidManager(&nh_)),
                                cam_(new Camera()),
                                vision_(new TaskVision(cam_)),
                                gate_(new TaskGate(pm_.get(), vision_.get())),
                                buoy_(new TaskBuoy(pm_, vision_))
                        {

                        }
                        ros::NodeHandle nh_;
                        std::shared_ptr<PidManager> pm_;
                        std::shared_ptr<Camera> cam_;
                        std::shared_ptr<TaskVision> vision_;
                        std::shared_ptr<TaskGate> gate_;
                        std::shared_ptr<TaskBuoy> buoy_;

                };



                STATE(Top)

                virtual void run(){

                }


private:
                // special actions
                void entry(){
                        AUV_DEBUG("Top::entry");
                }
                void init(){
                        AUV_DEBUG("Initalize");
                        AUV_DEBUG("Created PM Pointer: %x", box().pm_.get());
                        AUV_DEBUG("Created Cam pointer: %x", box().cam_.get());
                        AUV_DEBUG("Created Vision pointer: %x", box().vision_.get());
                        AUV_DEBUG("Created Gate pointer: %x", box().gate_.get());
                        AUV_DEBUG("Created Buoy pointer: %x", box().buoy_.get());

                        setState<Init>();

                }



        };

//         SUBSTATE(Timer, Top) {
//                 struct Box {
//                         Box() : waitTime(0), currentState(Top::alias()), startTime(0) {
//                         }
//                         double waitTime;
//                         Macho::Alias currentState;
//                         double startTime;
//                 };
//                 STATE(Timer)
//                 void run();
// private:
//                 void entry(){
//                         AUV_DEBUG("Timer::entry");
//                 }
//                 void init(double waitTime, Macho::Alias currentState){
//                         box().waitTime = waitTime;
//                         box().currentState = currentState;
//                         box().startTime = ros::Time::now().toSec();
//                 }
//         };

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

        };

        SUBSTATE(Gate, Top) {


                STATE(Gate)

                void run();

private:
                void entry(){
                        AUV_DEBUG("Gate::entry");
                }

        };

        SUBSTATE(Buoy, Top) {


                STATE(Buoy)

                void run();

private:
                void entry();

        };


        SUBSTATE(Kill, Top) {


                STATE(Kill)

                void run();

private:
                void entry(){
                        AUV_DEBUG("Kill::entry");
                }

        };



        Macho::Machine<StateMachine::Top> state_;


};






#endif
