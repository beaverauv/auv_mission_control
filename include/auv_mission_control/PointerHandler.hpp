#ifndef POINTER_CONTROLLER_
#define POINTER_CONTROLLER_

#include <auv_mission_control/Camera.hpp>
#include <auv_mission_control/Macho.hpp>
#include <auv_mission_control/Mission.hpp>
#include <auv_mission_control/PidManager.hpp>
#include <auv_mission_control/Task.hpp>
#include <auv_mission_control/TaskBuoy.hpp>
#include <auv_mission_control/TaskGate.hpp>
#include <auv_mission_control/TaskMarker.hpp>
#include <auv_mission_control/TaskTest.hpp>
#include <auv_mission_control/Vision.hpp>

class PointerController : public Task {
public:
  PointerController() {}
  ~PointerController() {}

  std::string getTag() { return std::string("[Pointer Holder]"); }

  void print() {
    AUV_DEBUG("StateMachine: %x", sm_.get());
    AUV_DEBUG("Mission: %x", mission_.get());
    AUV_DEBUG("PidManager: %x", pm_.get());
    AUV_DEBUG("Camera: %x", cam_.get());
    AUV_DEBUG("Vision: %x", vision_.get());
    AUV_DEBUG("TaskTest: %x", test_.get());
    AUV_DEBUG("TaskGate: %x", gate_.get());
    AUV_DEBUG("TaskBuoy: %x", buoy_.get());
    AUV_DEBUG("TaskMarker: %x", marker_.get());
  }

  std::shared_ptr<StateMachine> sm_;
  std::shared_ptr<Mission> mission_;
  std::shared_ptr<PidManager> pm_;
  std::shared_ptr<Camera> cam_;
  std::shared_ptr<Vision> vision_;
  std::shared_ptr<TaskTest> test_;
  std::shared_ptr<TaskGate> gate_;
  std::shared_ptr<TaskBuoy> buoy_;
  std::shared_ptr<TaskMarker> marker_;
};

#endif
