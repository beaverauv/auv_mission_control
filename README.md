# AUV Mission Control

The Mission control package handles task queuing and execution for BeaverAUV's Prospero Mark II autonomous underwater vehicle. It primarily implements a C++ hirearchical state machine. It is specifically written for the 2017 BeaverAUV submission to the international RoboSub competition. This package makes all major decisions regarding what tasks to execute when. Each task may call other sub-tasks. This is a drastic improvement over our previous state machine not only because it allows us to execute tasks in a hirearchical manner, but also because it allows us to seamlessly and constantly run top-level 'system-checks'. For example, we never need to worry about breaching the surface of the water (which in RoboSub causes your run to end) because the state machine constantly checks the depth and will interrupt the tasks if a minimum depth is reached. Not having this feature caused issues in the 2016 competition.

It's primary outputs are to the eight PID controllers (via ROS). Depending on the task, it will publish the desired steady-state values based on inputs from our IMU (interial measurement unit), depth sensor, [F-RCNN deap-learning vision algorithms](https://github.com/beaverauv/ros_frcnn_object_detector), or our sonar multilateration algortithms (although at the time of the 2017 competition, these have not been fully implemented). The PID controllers then send vectors representing the desired direction of travel (both translational and rotational) to control algorithms, which determine how fast to spin each motor.

The Mission Control package also provides a PID Manager class which effectively multiplexes the different input types, and adjusts the PID controllers accordingly. For example, when executing a vision-based task the PID manager will direct the relevant PID controllers to accept input from the vision algorithms, and will also set the tuning values of the PID controller accordingly.

# Getting Started

## Installation

This package cannot be run standalone, and requires many other tools. You will also need several other repositories for the code to run without error. Note that this 2017 version of the code is on the RoboSub2017 branch. You will need Ros Kinetic (ideally installed on Ubuntu 16.04 - this build very specifically works well with ROS). You also need python 2.7. It's possible to get around it, but very frustrating. 

Clone all of these repositories into one directory (catkin_ws is convention since we use the catkin build tool).

First clone this repository:

```unix
git clone https://github.com/beaverauv/auv_mission_control
git checkout RoboSub2017
```

And the minimum additional packages you need to compile:

```unix
git clone https://github.com/beaverauv/auv_thruster_control
git clone http://github.com/beaverauv/pid
```

To have I/O control:

```unix
git clone https://github.com/beaverauv/BlueRobotics_MS5837_Library
git clone https://gihub.com/beaverauv/auv_backplane
```

For fancy machine vision:

```unix
https://github.com/beaverauv/caffe-fast-rcnn
https://github.com/beaverauv/ros_frcnn_object_detector
```

If you (for some reason) want to retrain that model from scratch, we use this tool for that. Thanks to MSU RoboSub for sharing:
```unix
https://github.com/beaverauv/sloth_msurobosub
```

## Running

Go to the directory you installed in
```unix
cd /catkin_ws
```

Build:
```unix
catkin_make
```
Run:

```unix
roslaunch auv_mission_control start.launch
```
Cross your fingers and hope for the best.


# About the Code

This is a very general outline of how the code is structured. It is not intended to give a full understanding.

The code is all based on ROS. We built our hirearchical state machine based on the [MACHO C++ library](http://ehiti.de/machine_objects/).

Each individual task exists in the /src/task directory, and is accompanied by a header file in the auv_mission_control/include/auv_mission_control/ directory. Tasks must have an integer function execute() which steps through logic for that task. See src/pidmanager.cpp for available functions.

Missions are stored in the src/mission directory. Each mission must also have an integer function execute(). 

Each mission must first enable the task queue:

```C++
 self()->queueEnable();
 ```
 
 You should then queue a timer 'action' (mini template states, using C++ templates) to wait an appropriate amount of time between starting the task - usually initialized with a physical switch - and actually spinning motors (for human swimmer safety):
 
 ```C++
  QUEUE_ACTION(timer, 2.0);   //starts a 2 second timer once the queue is run
```
From there, you can queue your first state, usually gate:

```C++
QUEUE_STATE(Gate);        //adds the task specified in the gate class to the queue
```

Note that substates and other actions are queued from within these states, this is just the top-level.

Other states can then be added to the queue or run directly as follows. Note that you can tell the sub to run many tasks one after another, and it will wait until each has finished to run the next.

```C++
void Test::Tested::run() { test()->execute(); }
```

# Authors

* **Seth Isaacson** - isaacsons17@bcdschool.org
* **Oliver Geller** - gellero17@bcdschool.org
