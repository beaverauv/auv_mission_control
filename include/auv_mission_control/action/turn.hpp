#ifndef TURN_H
#define TURN_H

#include <auv_mission_control/Macho.hpp>
#include <auv_mission_control/StateMachine.hpp>
#include <auv_mission_control/range.hpp>
#include <memory>

template <class T> TSUBSTATE(Turn, T) {
  struct Box {
    Box()
        : angle_(0), wait_time_(5), alias_(TOP::alias()), start_yaw_(0),
          deadband_(0.1), is_alias_set_(false), old_zero_(0) {}

    Macho::Alias alias_;
    bool is_alias_set_;

    int iterator = 0;

    double old_zero_;
    double deadband_;
    double wait_time_;
    double angle_;
    double start_yaw_;
  };

  AUV_TSTATE(Turn);
  inline void run() {
    T::pm()->setSetpoint(AXIS::YAW, INPUT::IMU_POS, box().angle_);
    if(T::pm()->isPidStable(AXIS::YAW, 5, .75)){
      if (box().is_alias_set_) {
        T::mission()->queueEnable();
        T::mission()->queueState(box().alias_);
        TOP::setState(T::alias());
      } else {
        T::self()->queueEnable();
        TOP::setState(T::alias());
      }
    }
    T::pm()->setPlantState(AXIS::YAW, T::pm()->getYaw());
  }

private:
  inline void init(double angle){
    if(!isPidStable(AXIS::YAW, box().angle_/4, .5)){
      box().angle_ = angle;
      T::pm()->setZeroTo(AXIS::YAW, box().start_yaw_);
      box().start_yaw_ = T::pm()->getYaw();
    }
    else{

    }

  }
  inline void init(double angle, Macho::Alias alias) {
    box().angle_ = angle;
    box().old_zero_ = T::pm()->getZero();
    box().start_yaw_ = T::pm()->getYaw();
    box().alias_ = alias;
  }
};

#endif
