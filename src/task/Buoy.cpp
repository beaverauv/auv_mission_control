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
  ph().pm->setEnabled(AXIS::SURGE, false);
  ph().pm->setEnabled(AXIS::SWAY, true);
  ph().pm->setEnabled(AXIS::HEAVE, true);
  ph().pm->setEnabled(AXIS::YAW, true);

  int gateState = 0;

  switch(gateState){

    case 0: //go to initial depth
      ph().pm->setSetpoint(AXIS::HEAVE, INPUT::DEPTh, 3);
      if(ph().pm->)

    case 1:

    case 2:

  }

}

}
