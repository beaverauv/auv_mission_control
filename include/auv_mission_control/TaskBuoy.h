#ifndef TaskBuoy_H
#define TaskBuoy_H

#include <memory>
#include <unistd.h>
#include <auv_mission_control/Task.h>
#include <auv_mission_control/Macho.hpp>
#include <auv_mission_control/PidManager.h>
#include <auv_mission_control/TaskVision.h>
#include <auv_mission_control/Camera.h>

class TaskBuoy : public Task {
public:
        TaskBuoy();
        TaskBuoy(std::shared_ptr<PidManager> pm, std::shared_ptr<TaskVision> vision);
        ~TaskBuoy();

        std::string getTag(){
                return std::string("[Task Buoy]");
        }
        int execute();
        void prepare();

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
                        return std::string("[StateBuoy]");
                }


                //TOPSTATE(Top) {
                // Top state variables (visible to all substates)
                struct Box {
                        Box() : pm_(0), vision_(0){
                        }
                        std::shared_ptr<PidManager> pm_;
                        std::shared_ptr<TaskVision> vision_;
                };

                STATE(Top)

                virtual void run(){

                }

                void initialize(){
                        setState<Init>();
                }
                void setLocalPointers(std::shared_ptr<PidManager> pm, std::shared_ptr<TaskVision> vision){
                        box().pm_ = pm;
                        box().vision_ = vision;
                }

private:
                void entry(){
                        AUV_DEBUG("Top::entry");
                }
        };

        SUBSTATE(Waiting, Top) {
                struct Box {
                        Box() : waitTime(0), currentState(Top::alias()), startTime(0) {
                        }
                        double waitTime;
                        Macho::Alias currentState;
                        double startTime;
                };
                STATE(Waiting)
                void run();
private:
                void entry(){
                        AUV_DEBUG("Timer::entry");
                }
                void init(double waitTime, Macho::Alias currentState){
                        box().waitTime = waitTime;
                        box().currentState = currentState;
                        box().startTime = ros::Time::now().toSec();
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
                        AUV_DEBUG("Current PM pointer: %x", Top::box().pm_.get());
                        AUV_DEBUG("Current Vision pointer: %x", Top::box().vision_.get());

                }
        };

        Macho::Machine<TaskBuoy::Top> stateBuoy_;



};

#endif
