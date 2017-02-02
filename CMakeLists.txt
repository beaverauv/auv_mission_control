cmake_minimum_required(VERSION 2.8.3)

project(auv_mission_control)


add_definitions("-std=c++14")

find_package(catkin REQUIRED COMPONENTS
  roscpp
  rospy
  std_msgs
  message_generation
  cv_bridge
)


find_package(OpenCV 2.4.11)

add_message_files(
   FILES
   axes.msg
 )

 generate_messages(
   DEPENDENCIES
   std_msgs
 )


catkin_package()


## Specify additional locations of header files
## Your package locations should be listed before other locations
# include_directories(include)
include_directories(
  ${catkin_INCLUDE_DIRS}
  include/
${OpenCV_INCLUDE_DIRS}
)


add_library(state_machine
  src/StateMachine.cpp
  src/Macho.cpp
  src/PidManager.cpp
  src/Camera.cpp
  src/Vision.cpp
  src/tasks/Task.cpp
  src/tasks/TaskTest.cpp
  src/tasks/TaskGate.cpp
  src/tasks/TaskBuoy.cpp
  src/tasks/TaskMarker.cpp
)

target_link_libraries(state_machine ${catkin_LIBRARIES}  ${OpenCV_LIBRARIES})
add_dependencies(state_machine
  ${${PROJECT_NAME}_EXPORTED_TARGETS}
  ${catkin_EXPORTED_TARGETS}
)

add_executable(state_machine_cont src/StateMachine.cpp)

target_link_libraries(state_machine_cont ${catkin_LIBRARIES} state_machine  ${OpenCV_LIBRARIES})
add_dependencies(state_machine_cont
  ${${PROJECT_NAME}_EXPORTED_TARGETS}
  ${catkin_EXPORTED_TARGETS}
)
