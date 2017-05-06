#include <auv_mission_control/TemplateStates.hpp>
#include <auv_mission_control/task/Buoy.hpp>

namespace Task {

int Buoy::execute() {
  if (checkEventQueue()) {
    sm_->run();
  }
}

void Buoy::Init::run() { setState<Timer<BumpBuoys>>(3.0); }

void Buoy::ScanLeft::run() {
  // code
}

void Buoy::BumpBuoys::run() {

  // int gateState = 0;
  //
  // if (self()->iterator == 0) {
  //   pm()->setZero(AXIS::YAW);
  //   self()->iterator++;
  // }
  //
  // switch (gateState) {
  //
  // case 0: // go to initial depth
  //   pm()->setEnabled(AXIS::SURGE, false);
  //   pm()->setEnabled(AXIS::SWAY, false);
  //   pm()->setEnabled(AXIS::HEAVE, true);
  //   pm()->setEnabled(AXIS::YAW, true);
  //   pm()->setControlEffort(AXIS::SURGE, 0);
  //   pm()->setControlEffort(AXIS::SWAY, 0);
  //   pm()->setSetpoint(AXIS::YAW, INPUT::DEPTH, 0);
  //   pm()->updatePlantState(AXIS::YAW);
  //
  //   pm()->setSetpoint(AXIS::HEAVE, INPUT::DEPTH, 3); // set depth setpoint
  //   pm()->updatePlantState(AXIS::HEAVE);
  //   // check if stable at depth
  //   if (pm()->checkPidStability(AXIS::HEAVE, 3, .25, 3)) {
  //     gateState = 1; // if it is move on
  //     break;
  //   } else {
  //     break;
  //   }
  // case 1: // Finding the gate
  //   if (ph().vision_->isDetected(OBJECT::GATE)) {
  //       case = 2;
  //       break;
  //   } else {
  //       int searchState = 0;
  //       switch(searchState){
  //         case 0://turning left to look
  //         pm()->setSetpoint(AXIS::YAW, INPUT::IMU::POS, -90);
  //         pm()->updatePlantState(AXIS::YAW);
  //         pm()->updatePlantState(AXIS::HEAVE);
  //         if (checkPidStability(AXIS::YAW, -90, 5, 3)) {
  //           searchState = 1;
  //           break;
  //         } else {
  //           break;
  //         }
  //       case 1: // turing right to look
  //         pm()->setSetpoint(AXIS::YAW, INPUT::IMU::POS, 90);
  //         pm()->updatePlantState(AXIS::YAW);
  //         pm()->updatePlantState(AXIS::HEAVE);
  //         if (checkPidStability(AXIS::YAW, -90, 5, 3)) {
  //           searchState = 1;
  //           break;
  //         } else {
  //           break;
  //         }
  //
  //       case 2: // aborting
  //         if (!ph().vision_->isDetected(OBJECT::GATE)) {
  //           setState<Kill<Whatever>>();
  //         } else {
  //           gateState = 2;
  //           break;
  //         }
  //       }
  //       if (ph().vision_->isDetected(OBJECT::GATE)) {
  //         gateState = 2;
  //       } else {
  //         gateState = 1;
  //       }
  // }
  // }
}
}
