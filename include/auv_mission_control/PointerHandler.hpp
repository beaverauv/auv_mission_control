#ifndef POINTER_CONTROLLER_
#define POINTER_CONTROLLER_

#include <auv_mission_control/Camera.hpp>
#include <auv_mission_control/Macho.hpp>
#include <auv_mission_control/PidManager.hpp>
#include <auv_mission_control/Task.hpp>
#include <auv_mission_control/Vision.hpp>
#include <auv_mission_control/mission/Base.hpp>
#include <auv_mission_control/mission/Test.hpp>

#include <auv_mission_control/task/Buoy.hpp>
#include <auv_mission_control/task/Example.hpp>
#include <auv_mission_control/task/Gate.hpp>
#include <auv_mission_control/task/Marker.hpp>
#include <auv_mission_control/task/Test.hpp>
#include <ros/ros.h>

#include <type_traits>
#include <utility>

#define CHECK_TASK(T, M)                                                       \
  if (!task.compare(#T)) {                                                     \
    return Mission::M::T::alias();                                             \
  }

#define CHECK_MISSION(M)                                                       \
  if (current_mission_ == MISSION::M) {                                        \
    MAP_PAIRS(CHECK_TASK, EMPTY, TASK_NAMES_M(M));                             \
  }

#define CHECK_SET_MISSION(M)                                                   \
  if (!mission.compare(Enum<MISSION>::Parse(MISSION::M))) {                    \
    mission_ = makeMission<Mission::M>();                                      \
    current_mission_ = MISSION::M;                                             \
  }

class PointerHandler {
public:
  PointerHandler(){};

  ~PointerHandler() {}

  std::shared_ptr<StateMachine> sm_;
  std::shared_ptr<Mission::Base> mission_;
  std::shared_ptr<PidManager> pm_;
  std::shared_ptr<Camera> cam_;
  std::shared_ptr<Vision> vision_;
  MAKE_ALL_PH_TASKS();

  int execute();

  void setMission(std::string mission) {
    MAP(CHECK_SET_MISSION, SEMICOLON, MISSION_NAMES());
  }

  template <class S> std::shared_ptr<S> makeMission() {
    return std::make_shared<S>(std::shared_ptr<PointerHandler>(this));
  }

  std::shared_ptr<Mission::Base> mission() { return mission_; }

  auto alias(std::string task) { CHECK_MISSIONS(); }

  std::string mission_str() { return Enum<MISSION>::Parse(current_mission_); }

  void queueTask(std::string task) {
    mission()->queueEnable();
    mission()->queueState(alias(task));
  }

  MISSION current_mission_;
  bool debug_ = true;
};

#endif
