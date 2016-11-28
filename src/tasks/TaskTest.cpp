#include <auv_mission_control/TaskTest.h>
#include <auv_mission_control/StateMachine.h>

TaskTest::TaskTest(){
}


TaskTest::TaskTest(std::shared_ptr<PidManager> pm, std::shared_ptr<Vision> vision){
        AUV_DEBUG("[Pointers] [PM]: %x", pm.get());
        AUV_DEBUG("[Pointers] [VISION]: %x", vision.get());

        stateTest_->setPointer(pm);
        stateTest_->setPointer(vision);

}

TaskTest::~TaskTest(){

}

void TaskTest::prepare(){
        //stateTest_->initialize();
        stateTest_->initialize();
}

void TaskTest::prepare(std::shared_ptr<StateMachine> statemachine){
        stateTest_->initialize(statemachine);
}


int TaskTest::execute(){

        stateTest_->run();

}

void TaskTest::Init::run() {
        AUV_DEBUG("Init::run");
        AUV_DEBUG("Waiting for 3 seconds");
        setState<Timer::Timer<Top> >(3.0, Macho::State<Whatever>());
}

void TaskTest::Whatever::run() {
        Top::box().statemachine_->queueState<StateMachine::Gate>();
}
