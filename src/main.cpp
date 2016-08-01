#include <auv_mission_control/StateMachine.h>


int main(int argc, char* argv[]){
        ros::init(argc, argv, "state_machine");
        StateMachine state;

        state.execute();
}
