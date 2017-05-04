#include <auv_mission_control/StateMachine.hpp>
#include <auv_mission_control/TemplateStates.hpp>

ROBOT_ENUM(MISSION) {
  ROBOT_ENUM_MAP(MISSION::Base, "mission_base");
  ROBOT_ENUM_MAP(MISSION::Test, "mission_test");
  ROBOT_ENUM_MAP(MISSION::Full, "mission_full");
  ROBOT_ENUM_MAP(MISSION::GateOnly, "mission_gate_only");
}

ROBOT_ENUM(TASK) {
  ROBOT_ENUM_MAP(TASK::Test, "task_test");
  ROBOT_ENUM_MAP(TASK::Example, "task_example");
  ROBOT_ENUM_MAP(TASK::Gate, "task_gate");
  ROBOT_ENUM_MAP(TASK::Buoy, "task_buoy");
  ROBOT_ENUM_MAP(TASK::Marker, "task_marker");
}

int main(int argc, char *argv[]) {
  ros::init(argc, argv, "state_machine");

  auto ph = std::make_shared<PointerHandler>();

  ros::param::get("~debug", ph->debug_);

  if (ros::console::set_logger_level(ROSCONSOLE_DEFAULT_NAME,
                                     ph->debug_ ? ros::console::levels::Debug
                                                : ros::console::levels::Info)) {
    ros::console::notifyLoggerLevelsChanged();
  }

  ph->sm_ = std::make_shared<StateMachine>(ph);
  ph->pm_ = std::make_shared<PidManager>();
  ph->cam_ = std::make_shared<Camera>();
  ph->vision_ = std::make_shared<Vision>(ph);
  ph->test_ = std::make_shared<Task::Test>(ph);
  ph->example_ = std::make_shared<Task::Example>(ph);
  ph->gate_ = std::make_shared<Task::Gate>(ph);
  ph->buoy_ = std::make_shared<Task::Buoy>(ph);
  ph->marker_ = std::make_shared<Task::Marker>(ph);

  std::string mission;
  ros::param::get("~mission", mission);
  ROS_INFO("Starting with mission %s", mission.c_str());
  ph->setMission(mission);

  ph->sm_->execute();
}

int StateMachine::execute() {
  AUV_INFO("Execute");

  ros::Rate state_rate(20);

  AUV_INFO("Waiting for IMU initialization...");
  while (!pm()->isImuCalled() && ros::ok() && !ph()->debug_) {
    ros::spinOnce();
    pm()->updatePlantState(AXIS::YAW);
    pm()->setZero(AXIS::YAW);
  }

  pm()->startEnsuringDepth();
  pm()->startEnsuringYaw();

  while (ros::ok()) {
    ros::spinOnce();

    state_rate.sleep();

    mission()->execute();
  }
}
