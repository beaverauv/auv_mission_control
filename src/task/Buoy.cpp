#include <auv_mission_control/task/Buoy.hpp>
#include <auv_mission_control/template_states.hpp>

namespace Task {

int Buoy::execute() {
  if (checkEventQueue()) {
    sm_->run();
  }
}

void Buoy::Init::run() { setState<Timer<Init>>(3.0); }

void Buoy::Whatever::run() {
  setState<Timer<Init>>(0.0, ph().alias("Buoy"));

  int gateState = 0;

  if(Buoy.iterator == 0){
    ph().pm->setZero(AXIS::YAW);
    Buoy.iterator++;
  }

  switch(gateState){

    case 0: //go to initial depth
      ph().pm->setEnabled(AXIS::SURGE, false);
      ph().pm->setEnabled(AXIS::SWAY, false);
      ph().pm->setEnabled(AXIS::HEAVE, true);
      ph().pm->setEnabled(AXIS::YAW, true);
      ph().pm->setControlEffort(AXIS::SURGE, 0);
      ph().pm->setControlEffort(AXIS::SWAY, 0);
      ph().pm->setSetpoint(AXIS::YAW, INPUT::DEPTH, 0);
      ph().pm->updatePlantState(AXIS::YAW);

      ph().pm->setSetpoint(AXIS::HEAVE, INPUT::DEPTH, 3); //set depth setpoint
      ph().pm->updatePlantState(AXIS::HEAVE);
      if(ph().pm->checkPidStability(AXIS::HEAVE, 3, .25, 3)){//check if stable at depth
        gateState = 1; //if it is move on
        break;
      }
      else{
        break;
      }
    case 1: //Finding the gate
      if(ph().vision_->isDetected(OBJECT::GATE)){
        case = 2;
        break;
      }
      else{
        int searchState = 0;
        switch(searchState){
          case 0://turning left to look
            ph().pm->setSetpoint(AXIS::YAW, INPUT::IMU::POS, -90);
            ph().pm->updatePlantState(AXIS::YAW);
            ph().pm->updatePlantState(AXIS::HEAVE);
            if(checkPidStability(AXIS::YAW, -90, 5, 3)){
              searchState = 1;
              break;
            }
            else{
              break;
            }
          case 1://turing right to look
            ph().pm->setSetpoint(AXIS::YAW, INPUT::IMU::POS, 90);
            ph().pm->updatePlantState(AXIS::YAW);
            ph().pm->updatePlantState(AXIS::HEAVE);
            if(checkPidStability(AXIS::YAW, -90, 5, 3)){
              searchState = 1;
              break;
            }
            else{
              break;
            }

          case 2://aborting
            if(!ph().vision_->isDetected(OBJECT::GATE)){
              setState<Kill<Whatever>>();
            }
            else{
              gateState = 2;
              break;
            }
        }
        if(ph().vision_->isDetected(OBJECT::GATE)){
          gateState = 2;
        }
        else{
          gateState = 1;
        }
      }

    case 2:
      

  }

}

}
