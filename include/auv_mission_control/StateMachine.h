#ifndef STATEMACHINE_H
#define STATEMACHINE_H

#include <memory>

#include <auv_mission_control/Task.h>
#include <auv_mission_control/Timer.h>
#include <auv_mission_control/Macho.hpp>
#include <auv_mission_control/Camera.h>
#include <auv_mission_control/PidManager.h>
#include <auv_mission_control/Vision.h>
#include <auv_mission_control/TaskTest.h>
#include <auv_mission_control/TaskGate.h>
#include <auv_mission_control/TaskBuoy.h>
#include <auv_mission_control/TaskMarker.h>



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
                                vision_(new Vision(cam_)),
                                gate_(new TaskGate(pm_, vision_)),
                                buoy_(new TaskBuoy(pm_, vision_)),
                                marker_(new TaskMarker(pm_, vision_)),
                                test_(new TaskTest(pm_, vision_))

                        {

                        }
                        ros::NodeHandle nh_;
                        std::shared_ptr<PidManager> pm_;
                        std::shared_ptr<Camera> cam_;
                        std::shared_ptr<Vision> vision_;
                        std::shared_ptr<TaskGate> gate_;
                        std::shared_ptr<TaskBuoy> buoy_;
                        std::shared_ptr<TaskMarker> marker_;
                        std::shared_ptr<TaskTest> test_;


                };



                STATE(Top)

                virtual void run(){

                }

                void whatever(){
                        AUV_INFO("HERE");
                }


private:
                // special actions
                void entry(){
                        AUV_DEBUG("Top::entry");
                }
                void init(){
                        AUV_DEBUG("Initalize");
                        AUV_DEBUG("[Pointers] [PM]: %x", box().pm_.get());
                        AUV_DEBUG("[Pointers] [CAM]: %x", box().cam_.get());
                        AUV_DEBUG("[Pointers] [VISION]: %x", box().vision_.get());
                        AUV_DEBUG("[Pointers] [GATE]: %x", box().gate_.get());
                        AUV_DEBUG("[Pointers] [BUOY]: %x", box().buoy_.get());

                        setState<Init>();

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

        };

        SUBSTATE(Kill, Top) {


                STATE(Kill)

                void run();

private:
                void entry(){
                        AUV_DEBUG("Kill::entry");
                }

        };

        SUBSTATE(Test, Top) {


                STATE(Test)

                void run();

private:
                template<class S >
                void init(Macho::Machine machine, Macho::IEvent<S> * event){
                        AUV_DEBUG("Test::entry");
                        Top::box().test_->prepare(event);
                }

        };

        SUBSTATE(Gate, Top) {


                STATE(Gate)

                void run();

private:
                void entry(){
                        AUV_DEBUG("Gate::entry");
                        Top::box().gate_->prepare();
                }

        };

        SUBSTATE(Buoy, Top) {


                STATE(Buoy)

                void run();

private:
                void entry(){
                        AUV_DEBUG("Buoy::entry");
                        Top::box().buoy_->prepare();
                }

        };

        SUBSTATE(Marker, Top) {


                STATE(Marker)

                void run();

private:
                void entry(){
                        AUV_DEBUG("Marker::entry");
                        Top::box().marker_->prepare();
                }

        };




        Macho::Machine<StateMachine::Top> state_;


};






#endif
