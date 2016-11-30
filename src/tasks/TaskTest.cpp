#include <auv_mission_control/TaskTest.hpp>
#include <auv_mission_control/StateMachine.hpp>
#include <auv_mission_control/Timer.hpp>


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

void TaskTest::prepare(std::shared_ptr<StateMachine> statemachine){
        stateTest_->setPointer(statemachine);
        stateTest_->initialize();
}


int TaskTest::execute(){

        stateTest_->run();

}

void TaskTest::Init::run() {
        AUV_DEBUG("Init::run");
        AUV_DEBUG("Waiting for 3 seconds");
        //setState<Timer::Timer<Whatever> >(3.0, Macho::Event(&Top::here));
        //setState<Timer::Timer<Init> >(3.0, Top::box().statemachine_);
        setState<Timer::Timer<Init> >(3.0, StateMachine::Gate::alias());


        //setState<Timer::Timer<Whatever> >(3.0, Macho::State<Whatever>(), Macho::Event(&TaskTest::Whatever::here));
        // setState<Timer::Timer<Init> >(3.0);
}

void TaskTest::Whatever::run() {
        Top::box().statemachine_->queueState<StateMachine::Marker>();
}
