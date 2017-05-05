#ifndef MARKERALIGN_H
#define MARKERALIGN_H

#include <auv_mission_control/Macho.hpp>
#include <auv_mission_control/StateMachine.hpp>
#include <auv_mission_control/range.hpp>
#include <memory>

template <class T> TSUBSTATE(Align, T){
  struct Box {
    Box()
        : depth_(1), markerThreshold(1), theta_(0), wait_time_(0),
        isCentered(false), stability_start_time(0),
        isAligned(false), wasCentered(false), alias_(TOP::alias()),
        isAliasSet(false){}

    Macho::Alias alias_;
    bool isAliasSet;

    bool isAligned;
    bool wasCentered;
    bool isCentered;
    double wait_time_;
    double depth_;
    double theta_;
    int stability_start_time;
  };

  double startYaw = pm()->getYaw();

AUV_TSTATE(MarkerAlign);
inline void run(){
  pm()->setEnabled(AXIS::SURGE, true);
  pm()->setEnabled(AXIS::SWAY, true);
  pm()->setEnabled(AXIS::HEAVE, true);
  pm()->setEnabled(AXIS::YAW, true);

  pm()->setSetpoint(AXIS::SURGE, INPUT_CAM_BTM, SCREEN_CENTER_VERTICAL);
  pm()->setSetpoint(AXIS::SWAY, INPUT_CAM_BTM, SCREEN_CENTER_HORIZONTAL);
  pm()->setSetpoint(AXIS::HEAVE, INPUT_DEPTH, depth);
  pm()->updatePlantState(AXIS::SURGE);
  pm()->updatePlantState(AXIS::SWAY);
  pm()->updatePlantState(AXIS::HEAVE);



  if(!box().isCentered){
    if(box().wasCentered){
      box().startYaw = pm()->getYaw();
    }
    pm()->setSetpoint(AXIS::YAW, INPUT_IMU_POS, startYaw);
    pm()->updatePlantState(AXIS::YAW);
    box().wasCentered = false;

  }
  else{
    pm()->setSetpoint(AXIS::YAW, INPUT_CAM_BTM, 0);
    pm()->updatePlantState(AXIS::YAW);
    box().wasCentered = true;
  }


  if(pm()->isPidStable(AXIS::YAW, 10, wiat_time_){
    T::ph().mission()->queueEnable();
    T::setState()
  }

}
