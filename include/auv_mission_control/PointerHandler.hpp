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
    CHECK_TASK(Top, M);                                                        \
    CHECK_TASK(Buoy, M);                                                       \
    CHECK_TASK(Example, M);                                                    \
    CHECK_TASK(Gate, M);                                                       \
    CHECK_TASK(Marker, M);                                                     \
    CHECK_TASK(Tested, M);                                                     \
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
  std::shared_ptr<Task::Test> test_;
  std::shared_ptr<Task::Example> example_;
  std::shared_ptr<Task::Gate> gate_;
  std::shared_ptr<Task::Buoy> buoy_;
  std::shared_ptr<Task::Marker> marker_;

  int execute();

  void setMission(std::string mission) {
    CHECK_SET_MISSION(Base);
    CHECK_SET_MISSION(Test);
  }

  template <class S> std::shared_ptr<S> makeMission() {
    return std::make_shared<S>(std::shared_ptr<PointerHandler>(this));
  }

  std::shared_ptr<Mission::Base> mission() { return mission_; }

  auto alias(std::string task) {
    CHECK_MISSION(Test);
    // CHECK_MISSION(FullRun);
  }

  std::string mission_str() { return Enum<MISSION>::Parse(current_mission_); }

  void queueTask(std::string task) {
    mission()->queueEnable();
    mission()->queueState(alias(task));
  }

  MISSION current_mission_;
  bool debug_ = true;
};

#endif
