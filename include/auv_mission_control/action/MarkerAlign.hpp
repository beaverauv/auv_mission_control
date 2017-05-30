#ifndef MARKERALIGN_H
#define MARKERALIGN_H

#include <auv_mission_control/Macho.hpp>
#include <auv_mission_control/StateMachine.hpp>
#include <auv_mission_control/range.hpp>
#include <memory>
#define SCREEN_CENTER_VERTICAL 0
#define SCREEN_CENTER_HORIZONTAL 1

template <class T> TSUBSTATE(MarkerAlign, T) {
  struct Box {
    Box()
        : depth_(1), marker_threshold(1), theta_(0), wait_time_(0),
          is_centered_(false), stability_start_time_(0), is_aligned_(false),
          was_centered_(false), alias_(TOP::alias()), is_alias_set_(false) {}

    Macho::Alias alias_;
    bool is_alias_set_;

    bool is_aligned_;
    bool was_centered_;
    bool is_centered_;
    double wait_time_;
    double depth_;
    double theta_;
    double marker_threshold;
    int stability_start_time_;
  };

  double start_yaw = T::pm()->getYaw();

  AUV_TSTATE(MarkerAlign);
  inline void run() {
    T::pm()->setEnabled(AXIS::SURGE, true);
    T::pm()->setEnabled(AXIS::SWAY, true);
    T::pm()->setEnabled(AXIS::HEAVE, true);
    T::pm()->setEnabled(AXIS::YAW, true);

    //moves fwd/bkwd/lft/right to match marker
    T::pm()->setSetpoint(AXIS::SURGE, INPUT_CAM_BTM, SCREEN_CENTER_VERTICAL);
    T::pm()->setSetpoint(AXIS::SWAY, INPUT_CAM_BTM, SCREEN_CENTER_HORIZONTAL);
    //locks heave at input depth
    T::pm()->setSetpoint(AXIS::HEAVE, INPUT_DEPTH, box().depth_);

    //update all plantstates
    T::pm()->updatePlantState(AXIS::SURGE);
    T::pm()->updatePlantState(AXIS::SWAY);
    T::pm()->updatePlantState(AXIS::HEAVE);

    //if stable in xy
    if(T::pm()->isPidStable(AXIS::SURGE) && T::pm()->isPidStable(AXIS::SWAY)){
      T::pm()->setSetpoint(AXIS::YAW, INPUT_CAM_BTM, 0);
      Box().was_centered_ = true;
    }
    else{ //if it's not centered
      if(Box().was_centered_){//only reset the start yaw if first iteration off center
        Box().start_yaw = T::pm()->getYaw();
      }
      T::pm()->setSetpoint(AXIS::YAW, INPUT_IMU_POS, Box().start_yaw);
    }
    T::pm()->updatePlantState(AXIS::YAW);
  }
};
#endif
