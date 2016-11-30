#ifndef STATEMACHINE_H
#define STATEMACHINE_H

#include <memory>
#include <vector>

#include <auv_mission_control/Task.hpp>
#include <auv_mission_control/Macho.hpp>
#include <auv_mission_control/Camera.hpp>
#include <auv_mission_control/PidManager.hpp>
#include <auv_mission_control/Vision.hpp>
#include <auv_mission_control/TaskTest.hpp>
#include <auv_mission_control/TaskGate.hpp>
#include <auv_mission_control/TaskBuoy.hpp>
#include <auv_mission_control/TaskMarker.hpp>


class StateMachine : public Task {
public:
        StateMachine();
        ~StateMachine();

        std::string getTag(){
                return std::string("[State Machine]");
        }

        int execute();

        void setPointer(std::shared_ptr<StateMachine> statemachine){
                state_->setPointer(statemachine);
        }

        template<class S>
        void queueState(){
                eventqueue_.push_back(Macho::Event(&StateMachine::Top::setStateLocal<S>));
        }

        void queueState(Macho::Alias alias){
                eventqueue_.push_back(Macho::Event(&StateMachine::Top::setStateLocal, alias));
        }

        int checkEventQueue(){
                if (eventqueue_.empty()) return 1;
                for (EventQueue::iterator it = eventqueue_.begin(); it != eventqueue_.end(); ++it)
                        state_.dispatch(*it);

                eventqueue_.clear();
                return 0;
        }



        TOPSTATE(Top) {
                std::string getTag(){
                        return std::string("[State]");
                }


                struct Box {
                        Box() : statemachine_(0),
                                pm_(std::make_shared<PidManager>(&nh_)),
                                cam_(std::make_shared<Camera>()),
                                vision_(std::make_shared<Vision>(cam_)),
                                test_(std::make_shared<TaskTest>(pm_, vision_)),
                                gate_(std::make_shared<TaskGate>(pm_, vision_)),
                                buoy_(std::make_shared<TaskBuoy>(pm_, vision_)),
                                marker_(std::make_shared<TaskMarker>(pm_, vision_))

                        {
                        }

                        std::shared_ptr<StateMachine> statemachine_;
                        ros::NodeHandle nh_;
                        std::shared_ptr<PidManager> pm_;
                        std::shared_ptr<Camera> cam_;
                        std::shared_ptr<Vision> vision_;
                        std::shared_ptr<TaskTest> test_;
                        std::shared_ptr<TaskGate> gate_;
                        std::shared_ptr<TaskBuoy> buoy_;
                        std::shared_ptr<TaskMarker> marker_;
                };

                STATE(Top)

                virtual void run(){
                }

                void setPointer(std::shared_ptr<StateMachine> statemachine){
                        box().statemachine_ = statemachine;
                }

                template<class S>
                void setStateLocal(){
                        setState<S>();
                }

                void setStateLocal(Macho::Alias alias){
                        setState(alias);
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
                void init(){
                        AUV_DEBUG("Test::entry");
                        Top::box().test_->prepare(Top::box().statemachine_);
                }

        };

        SUBSTATE(Gate, Top) {

                STATE(Gate)

                void run();

private:

                void entry(){
                        AUV_DEBUG("Gate::entry");
                        Top::box().gate_->prepare(Top::box().statemachine_);
                }

        };

        SUBSTATE(Buoy, Top) {


                STATE(Buoy)

                void run();

private:
                void entry(){
                        AUV_DEBUG("Buoy::entry");
                        Top::box().buoy_->prepare(Top::box().statemachine_);
                }

        };

        SUBSTATE(Marker, Top) {


                STATE(Marker)

                void run();

private:
                void entry(){
                        AUV_DEBUG("Marker::entry");
                        Top::box().marker_->prepare(Top::box().statemachine_);
                }

        };


        typedef std::vector<Macho::IEvent<StateMachine::Top> *> EventQueue;
        EventQueue eventqueue_;

        Macho::Machine<StateMachine::Top> state_;



};






#endif
