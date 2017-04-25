#include <auv_mission_control/StateMachine.hpp>
#include <auv_mission_control/TemplateStates.hpp>

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
