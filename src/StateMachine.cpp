#include <auv_mission_control/StateMachine.hpp>
#include <auv_mission_control/Timer.hpp>

int main(int argc, char* argv[]){
        ros::init(argc, argv, "state_machine");
        if( ros::console::set_logger_level(ROSCONSOLE_DEFAULT_NAME, ros::console::levels::Debug) ) {
                ros::console::notifyLoggerLevelsChanged();
        }

        auto statemachine = std::make_shared<StateMachine>();
        statemachine->setPointer(statemachine);
        statemachine->queueState<StateMachine::Test>();
        statemachine->execute();

}


StateMachine::StateMachine(){
        AUV_INFO("Init");
}

StateMachine::~StateMachine(){

}


int StateMachine::execute(){
        AUV_INFO("Execute");

        ros::Rate stateRate(20);

        while(ros::ok) {
                ros::spinOnce();
                stateRate.sleep();
                if (checkEventQueue()) {
                        state_->run();
                }

        }

}

void StateMachine::Init::run() {
        AUV_INFO("%x", Top::box().statemachine_.get());
        AUV_DEBUG("Init::run");
        AUV_DEBUG("Waiting for 3 seconds");
        //setState<Timer>(3.0, Macho::State<Buoy>());
        //AUV_DEBUG("StateMachine::Init::run: ID %d", Macho::State<Top>()._KeyData)
        //Macho::IEvent<StateMachine::Top> * event = Macho::Event(&StateMachine::Top::whatever);


        setState<Timer::Timer<Init> >(3.0);
}

void StateMachine::Test::run(){
        Top::box().test_->execute();
}


void StateMachine::Gate::run(){
        Top::box().gate_->execute();

}

void StateMachine::Buoy::run(){
        Top::box().buoy_->execute();
}

void StateMachine::Marker::run(){
        Top::box().marker_->execute();
}



void StateMachine::Kill::run() {
        // AUV_ERROR("Kill::run");
        // setState<Init>();
}
