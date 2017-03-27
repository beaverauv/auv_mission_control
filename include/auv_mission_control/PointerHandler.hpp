#ifndef POINTER_CONTROLLER_
#define POINTER_CONTROLLER_

#include <auv_mission_control/Camera.hpp>
#include <auv_mission_control/Macho.hpp>
#include <auv_mission_control/Mission.hpp>
#include <auv_mission_control/PidManager.hpp>
#include <auv_mission_control/Task.hpp>
#include <auv_mission_control/Vision.hpp>

#include <auv_mission_control/task/Buoy.hpp>
#include <auv_mission_control/task/Example.hpp>
#include <auv_mission_control/task/Gate.hpp>
#include <auv_mission_control/task/Marker.hpp>
#include <auv_mission_control/task/Test.hpp>
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
  std::shared_ptr<Task::Test> test_;
  std::shared_ptr<Task::Example> example_;
  std::shared_ptr<Task::Gate> gate_;
  std::shared_ptr<Task::Buoy> buoy_;
  std::shared_ptr<Task::Marker> marker_;

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
