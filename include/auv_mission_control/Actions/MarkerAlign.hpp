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

  double startYaw = T::ph().pm_->getYaw();

AUV_TSTATE(Align);
inline void run(){
  T::ph().pm_->setEnabled(AXIS::SURGE, true);
  T::ph().pm_->setEnabled(AXIS::SWAY, true);
  T::ph().pm_->setEnabled(AXIS::HEAVE, true);
  T::ph().pm_->setEnabled(AXIS::YAW, true);

  T::ph().pm_->setSetpoint(AXIS::SURGE, INPUT_CAM_BTM, SCREEN_CENTER_VERTICAL);
  T::ph().pm_->setSetpoint(AXIS::SWAY, INPUT_CAM_BTM, SCREEN_CENTER_HORIZONTAL);
  T::ph().pm_->setSetpoint(AXIS::HEAVE, INPUT_DEPTH, depth);
  T::ph().pm_->updatePlantState(AXIS::SURGE);
  T::ph().pm_->updatePlantState(AXIS::SWAY);
  T::ph().pm_->updatePlantState(AXIS::HEAVE);



  if(!box().isCentered){
    if(box().wasCentered){
      box().startYaw = T::ph().pm_->getYaw();
    }
    T::ph().pm_->setSetpoint(AXIS::YAW, INPUT_IMU_POS, startYaw);
    T::ph().pm_->updatePlantState(AXIS::YAW);
    box().wasCentered = false;

  }
  else{
    T::ph().pm_->setSetpoint(AXIS::YAW, INPUT_CAM_BTM, 0);
    T::ph().pm_->updatePlantState(AXIS::YAW);
    box().wasCentered = true;
  }

  if(fabs(T::ph().pm_->getPlantState()) <= box().markerThreshold){
    box().isAligned = true;
  }

  if(box().isAligned){
    if(ros::Time::now().toSec() - box().wait_time_ >= box().stability_threshold_time){
      T::ph().mission()->queueEnable();
      T::setState()
    }
  }
  else
    box().stability_start_time = ros::Time::now().toSec();
}

}


//height = height of box found by cnn
//width = ^but width
//p = height / width
//theta = atan((8p-1)/(8-p))
//important to note that that angle is left/right ambiguous
//check two top quadrants of box. See which has more orange
//if ^ == left, theta *= -1
