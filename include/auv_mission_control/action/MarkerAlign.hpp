#ifndef MARKERALIGN_H
#define MARKERALIGN_H

#include <auv_mission_control/Macho.hpp>
#include <auv_mission_control/StateMachine.hpp>
#include <auv_mission_control/range.hpp>
#include <memory>

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

  double startYaw = T::pm()->getYaw();

  AUV_TSTATE(MarkerAlign);
  inline void run() {
    T::pm()->setEnabled(AXIS::SURGE, true);
    T::pm()->setEnabled(AXIS::SWAY, true);
    T::pm()->setEnabled(AXIS::HEAVE, true);
    T::pm()->setEnabled(AXIS::YAW, true);

    T::pm()->setSetpoint(AXIS::SURGE, INPUT_CAM_BTM, SCREEN_CENTER_VERTICAL);
    T::pm()->setSetpoint(AXIS::SWAY, INPUT_CAM_BTM, SCREEN_CENTER_HORIZONTAL);
    T::pm()->setSetpoint(AXIS::HEAVE, INPUT_DEPTH, depth);
    T::pm()->updatePlantState(AXIS::SURGE);
    T::pm()->updatePlantState(AXIS::SWAY);
    T::pm()->updatePlantState(AXIS::HEAVE);

    if (!box().is_centered_) {
      if (box().was_centered_) {
        box().startYaw = T::pm()->getYaw();
      }
      T::pm()->setSetpoint(AXIS::YAW, INPUT_IMU_POS, startYaw);
      T::pm()->updatePlantState(AXIS::YAW);
      box().was_centered_ = false;

    } else {
      T::pm()->setSetpoint(AXIS::YAW, INPUT_CAM_BTM, 0);
      T::pm()->updatePlantState(AXIS::YAW);
      box().was_centered_ = true;
    }

  if(T::pm()->isPidStable(AXIS::YAW, 10, wiat_time_){
      T::ph().mission()->queueEnable();
      T::setState()
  }
  }

#endif
