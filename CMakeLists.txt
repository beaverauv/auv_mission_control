cmake_minimum_required(VERSION 2.8.3)

project(auv_mission_control)


add_definitions("-std=c++14")
add_definitions("-g")


find_package(catkin REQUIRED COMPONENTS
  roscpp
  rospy
  std_msgs
  message_generation
  cv_bridge
)


find_package(OpenCV 2.4.11)


catkin_package()


## Specify additional locations of header files
## Your package locations should be listed before other locations
# include_directories(include)
include_directories(
  ${catkin_INCLUDE_DIRS}
  include/
${OpenCV_INCLUDE_DIRS}
)


add_executable(state_machine_node
  src/StateMachine.cpp
  src/Mission.cpp
  src/Macho.cpp
  src/Axis.cpp
  src/PidManager.cpp
  src/Camera.cpp
  src/Vision.cpp
  src/task/TaskBase.cpp
  src/task/TaskTest.cpp
  src/task/TaskExample.cpp
  src/task/TaskGate.cpp
  src/task/TaskBuoy.cpp
  src/task/TaskMarker.cpp
)

target_link_libraries(state_machine_node ${catkin_LIBRARIES}  ${OpenCV_LIBRARIES})
add_dependencies(state_machine_node
  ${${PROJECT_NAME}_EXPORTED_TARGETS}
  ${catkin_EXPORTED_TARGETS}
)
