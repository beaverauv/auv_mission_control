#ifndef GOTODEPTH_H
#define GOTODEPTH_H

#include <auv_mission_control/Macho.hpp>
#include <auv_mission_control/StateMachine.hpp>
#include <auv_mission_control/range.hpp>
#include <memory>

template <class T> TSUBSTATE(GoToDepth, T) {
  struct Box {
    Box()
        : depth_(1), wait_time_(5), alias_(TOP::alias()), start_yaw_(0),
          deadband_(0.1), is_alias_set_(false) {}

    Macho::Alias alias_;
    bool is_alias_set_;

    double deadband_;
    double wait_time_;
    double depth_;
    double start_yaw_;
  };

  AUV_TSTATE(GoToDepth);
  inline void run() {
    T::pm()->setEnabled(AXIS::SURGE, false);
    T::pm()->setEnabled(AXIS::SWAY, false);
    T::pm()->setEnabled(AXIS::HEAVE, true);
    T::pm()->setEnabled(AXIS::YAW, true);

    T::pm()->setControlEffort(AXIS::SURGE, 0);
    T::pm()->setControlEffort(AXIS::SWAY, 0);
    T::pm()->setSetpoint(AXIS::HEAVE, INPUT_DEPTH, box().depth_);
    T::pm()->updatePlantState(AXIS::SURGE);
    T::pm()->updatePlantState(AXIS::SWAY);
    T::pm()->updatePlantState(AXIS::HEAVE);

    if (T::pm().isPidStable(AXIS::HEAVE, box().deadband, box().wait_time_)) {
    }
  }

  inline void init() {}
};

#endif
