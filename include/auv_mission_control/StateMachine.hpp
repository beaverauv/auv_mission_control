#ifndef STATEMACHINE_H
#define STATEMACHINE_H

#include <memory>
#include <vector>

#include <auv_mission_control/Camera.hpp>
#include <auv_mission_control/Macho.hpp>
#include <auv_mission_control/PidManager.hpp>
#include <auv_mission_control/TTask.hpp>
// #include <auv_mission_control/Task.hpp>
#include <auv_mission_control/TaskBuoy.hpp>
#include <auv_mission_control/TaskGate.hpp>
#include <auv_mission_control/TaskMarker.hpp>
#include <auv_mission_control/TaskTest.hpp>
#include <auv_mission_control/Vision.hpp>

class StateMachine : public Task {
public:
  StateMachine();
  ~StateMachine();

  std::string getTag() { return std::string("[State Machine]"); }

  int execute();

  TOPSTATE(Top) {

    std::string getTag() { return std::string("[State]"); }

    struct Box {
      Box()
          : nh_(std::make_shared<ros::NodeHandle>()),
            pm_(std::make_shared<PidManager>()),
            cam_(std::make_shared<Camera>()),
            vision_(std::make_shared<Vision>(cam_)),
            test_(std::make_shared<TaskTest>(pm_, vision_)),
            gate_(std::make_shared<TaskGate>(pm_, vision_)),
            buoy_(std::make_shared<TaskBuoy>(pm_, vision_)),
            marker_(std::make_shared<TaskMarker>(pm_, vision_)) {}

      std::shared_ptr<ros::NodeHandle> nh_;
      std::shared_ptr<StateMachine> self_;
      std::shared_ptr<StateMachine> statemachine_;
      std::shared_ptr<PidManager> pm_;
      std::shared_ptr<Camera> cam_;
      std::shared_ptr<Vision> vision_;
      std::shared_ptr<TaskTest> test_;
      std::shared_ptr<TaskGate> gate_;
      std::shared_ptr<TaskBuoy> buoy_;
      std::shared_ptr<TaskMarker> marker_;
    };

    STATE(Top);

    createMachineFunctions();

    virtual void run() {}

    void setPointer(std::shared_ptr<StateMachine> statemachine);

  private:
    // special actions
    void entry() { AUV_DEBUG("Top::entry"); }
    void init() {
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
    STATE(Init)

    // Event handler
    void run();

  private:
    void entry() { AUV_DEBUG("Init::entry"); }
  };

  SUBSTATE(Kill, Top) {

    STATE(Kill)

    void run();

  private:
    void entry() { AUV_DEBUG("Kill::entry"); }
  };

  SUBSTATE(Nowhere, Top){

    STATE(Nowhere)

        void run(){}

    private : void entry(){}
  };

  SUBSTATE(Test, Top) {

    STATE(Test)

    void run();

  private:
    void init() {
      AUV_DEBUG("Test::entry");
      Top::box().test_->prepare(Top::box().self_);
    }
  };

  SUBSTATE(Gate, Top) {

    STATE(Gate)

    void run();

  private:
    void entry() {
      AUV_DEBUG("Gate::entry");
      Top::box().gate_->prepare(Top::box().self_);
    }
  };

  SUBSTATE(Buoy, Top) {

    STATE(Buoy)

    void run();

  private:
    void entry() {
      AUV_DEBUG("Buoy::entry");
      Top::box().buoy_->prepare(Top::box().self_);
    }
  };

  SUBSTATE(Marker, Top) {

    STATE(Marker)

    void run();

  private:
    void entry() {
      AUV_DEBUG("Marker::entry");
      Top::box().marker_->prepare(Top::box().self_);
    }
  };

  Macho::Machine<StateMachine::Top> state_;

  createQueue(StateMachine, state_)
};

#endif
