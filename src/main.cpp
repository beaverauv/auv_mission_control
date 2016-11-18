#include <auv_mission_control/StateMachine.h>


int main(int argc, char* argv[]){
        ros::init(argc, argv, "state_machine");
        if( ros::console::set_logger_level(ROSCONSOLE_DEFAULT_NAME, ros::console::levels::Debug) ) {
                ros::console::notifyLoggerLevelsChanged();
        }

        StateMachine state;

        state.execute();
}
