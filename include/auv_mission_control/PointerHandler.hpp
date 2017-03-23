#ifndef POINTER_CONTROLLER_
#define POINTER_CONTROLLER_

#include <auv_mission_control/Camera.hpp>
#include <auv_mission_control/Macho.hpp>
#include <auv_mission_control/Mission.hpp>
#include <auv_mission_control/PidManager.hpp>
#include <auv_mission_control/Task.hpp>
#include <auv_mission_control/TaskBuoy.hpp>
#include <auv_mission_control/TaskExample.hpp>
#include <auv_mission_control/TaskGate.hpp>
#include <auv_mission_control/TaskMarker.hpp>
#include <auv_mission_control/TaskTest.hpp>
#include <auv_mission_control/Vision.hpp>
#include <ros/ros.h>

class PointerHandler {
public:
  PointerHandler() {}
  ~PointerHandler() {}

  std::shared_ptr<StateMachine> sm_;
  std::shared_ptr<Mission> mission_;
  std::shared_ptr<PidManager> pm_;
  std::shared_ptr<Camera> cam_;
  std::shared_ptr<Vision> vision_;
  std::shared_ptr<TaskTest> test_;
  std::shared_ptr<TaskExample> example_;
  std::shared_ptr<TaskGate> gate_;
  std::shared_ptr<TaskBuoy> buoy_;
  std::shared_ptr<TaskMarker> marker_;

  // StateMachine &sm() { return *sm_; }
  // Mission &mission() { return *mission_; }
  // PidManager &pm() { return *pm_; }
  // Camera &cam() { return *cam_; }
  // Vision &vision() { return *vision_; }
  // TaskTest &test() { return *test_; }
  // TaskExample &example() { return *example_; }
  // TaskGate &gate() { return *gate_; }
  // TaskBuoy &buoy() { return *buoy_; }
  // TaskMarker &marker() { return *marker_; }
};

#endif
