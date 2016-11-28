#ifndef TaskTest_H
#define TaskTest_H

#include <memory>
#include <unistd.h>
#include <auv_mission_control/Task.h>
#include <auv_mission_control/Timer.h>
#include <auv_mission_control/Macho.hpp>
#include <auv_mission_control/PidManager.h>
#include <auv_mission_control/Vision.h>
#include <auv_mission_control/Camera.h>
//#include <auv_mission_control/StateMachine.h>

class StateMachine;

class TaskTest : public Task {
public:
        TaskTest();
        TaskTest(std::shared_ptr<PidManager> pm, std::shared_ptr<Vision> vision);
        ~TaskTest();

        std::string getTag(){
                return std::string("[Task Test]");
        }
        int execute();
        void prepare();
        void prepare(std::shared_ptr<StateMachine> statemachine);

private:
        //variables go here
        int currentColor;
        int objectFound;
        int action = 0;
        // Timer goToDepth_time;
        int depthCounter = 0;
        // Timer driveForwards_time;
        int forwardCounter = 0;
        // Timer waitTimer;
        int waitCounter = 0;
        double surgeSpeed = 25;
        double previousDepth;
        double distanceFromEdge_left;
        double distanceFromEdge_right;
        bool outOfSight;
        double plantState_sway;
        double plantState_heave;
        double setpoint_sway;
        double setpoint_heave;
        double setpoint_surge;
        double plantState_surge;

        double redDepth; //depth of red buoy

        //Timer ramRed;
        int ramRedCounter = 0;



        TOPSTATE(Top) {
                std::string getTag(){
                        return std::string("[StateTest]");
                }


                //TOPSTATE(Top) {
                // Top state variables (visible to all substates)
                struct Box {
                        Box() : pm_(0), vision_(0), statemachine_(0){
                        }
                        std::shared_ptr<StateMachine> statemachine_;
                        std::shared_ptr<PidManager> pm_;
                        std::shared_ptr<Vision> vision_;
                        //template<class S>
                        //Macho::IEvent<S> * event_;
                };

                STATE(Top)

                virtual void run(){

                }

                void initialize(){
                        // AUV_INFO("%x", Top::box().statemachine_.get());

                        setState<Init>();
                }

                void initialize(std::shared_ptr<StateMachine> statemachine){
                        Top::box().statemachine_ = statemachine;
                        // AUV_INFO("%x", Top::box().statemachine_.get());

                        setState<Init>();
                        //stateTest_.dispatch(event);
                }



                void setPointer(std::shared_ptr<PidManager> pm){
                        box().pm_ = pm;
                }

                void setPointer(std::shared_ptr<Vision> vision){
                        box().vision_ = vision;
                }

                void setPointer(std::shared_ptr<StateMachine> statemachine){
                        box().statemachine_ = statemachine;
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

                void init(){
                        //Top::box().alias_ = alias;
                }
        };


        SUBSTATE(Whatever, Top) {
                // State variables

                STATE(Whatever)
                // Event handler
                void run();
private:
                void entry(){
                        AUV_DEBUG("Init::entry");
                }

                void init(){
                        //Top::box().alias_ = alias;
                }
        };

        Macho::Machine<TaskTest::Top> stateTest_;



};

#endif
