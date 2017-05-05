#ifndef GOTODEPTH_H
#define GOTODEPTH_H

#include <auv_mission_control/Macho.hpp>
#include <auv_mission_control/StateMachine.hpp>
#include <auv_mission_control/range.hpp>
#include <memory>

template <class T> TSUBSTATE(Depth, T){
  struct Box {
    Box()
        : depth_(1), wait_time_(5), alias_(TOP::alias()), startYaw(0),
        deadband(.1), isAliasSet(false){}

    Macho::Alias alias_;
    bool isAliasSet;


    double wait_time_;
    double depth_;
    double startYaw = pm()->getYaw();

  };


AUV_TSTATE(GoToDepth);
inline void run(){
  pm()->setEnabled(AXIS::SURGE, false);
  pm()->setEnabled(AXIS::SWAY, false);
  pm()->setEnabled(AXIS::HEAVE, true);
  pm()->setEnabled(AXIS::YAW, true);

  pm()->setControlEffort(AXIS::SURGE, 0);
  pm()->setControlEffort(AXIS::SWAY, 0);
  pm()->setSetpoint(AXIS::HEAVE, INPUT_DEPTH, depth_);
  pm()->updatePlantState(AXIS::SURGE);
  pm()->updatePlantState(AXIS::SWAY);
  pm()->updatePlantState(AXIS::HEAVE);

  if(pm().isPidStable(AXIS::DEPTH, deadband, wait_time_));

}
