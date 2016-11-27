#include <auv_mission_control/StateMachine.h>

int main(int argc, char* argv[]){
        ros::init(argc, argv, "state_machine");
        if( ros::console::set_logger_level(ROSCONSOLE_DEFAULT_NAME, ros::console::levels::Debug) ) {
                ros::console::notifyLoggerLevelsChanged();
        }

        StateMachine statemachine_;

        statemachine_.execute();

}


StateMachine::StateMachine(){
        AUV_INFO("Init");
        // pm_ = new PidManager(&nh_);
        // AUV_DEBUG("Created PM Pointer: %x", pm_);
        //
        // cam_ = new Camera();
        // AUV_DEBUG("Created Cam pointer: %x", cam_);
        // //cam_->startRecording();
        // vision_ = new Vision(cam_);
        // AUV_DEBUG("Created Vision pointer: %x", vision_);
        //
        //
        // //gate_ = new TaskGate(pm_, vision_);
        // //marker_ = new TaskMarker(pm_, vision_);
        // buoy_ = new TaskBuoy(pm_, vision_);


}

StateMachine::~StateMachine(){

}


int StateMachine::execute(){
        AUV_INFO("Execute");
        // while(true){
        //   ros::spinOnce();
        //   AUV_DEBUG("%f", pm_->getDepth());
        // }
        ros::Rate stateRate(20);
        //state_->initialize();
        //buoy_->execute();

        while(true) {
                ros::spinOnce();
                stateRate.sleep();

                state_->run();

        }

}

void StateMachine::Init::run() {
        AUV_DEBUG("Init::run");
        AUV_DEBUG("Waiting for 3 seconds");
        //setState<Timer>(3.0, Macho::State<Buoy>());
        //AUV_DEBUG("StateMachine::Init::run: ID %d", Macho::State<Top>()._KeyData)
        //Macho::IEvent<StateMachine::Top> * event = Macho::Event(&StateMachine::Top::whatever);
        setState<Timer::Timer<Top> >(3.0, Macho::State<Test>(machine(), Macho::Event(&StateMachine::Top::whatever) ));
        //setState<Test>(Macho::State<Top>);
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

// void StateMachine::Timer::run(){
//         if ((ros::Time::now().toSec() - box().startTime) > box().waitTime) {
//                 AUV_DEBUG("Waiting done, switching states");
//                 setState(box().currentState);
//
//         }
// }
