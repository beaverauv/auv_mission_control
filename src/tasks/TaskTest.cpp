#include <auv_mission_control/TaskTest.h>

TaskTest::TaskTest(){
}


TaskTest::TaskTest(std::shared_ptr<PidManager> pm, std::shared_ptr<Vision> vision){
        AUV_INFO("Constructor");
        AUV_DEBUG("[Pointers] [PM]: %x", pm);
        AUV_DEBUG("[Pointers] [VISION]: %x", vision);
        stateTest_->setLocalPointers(pm, vision);
        //stateTest_->setVision(vision);


}

TaskTest::~TaskTest(){

}

template<class S >
void TaskTest::prepare(Macho::IEvent<S> * event){
        stateTest_->initialize(event);
}


int TaskTest::execute(){

        stateTest_->run();

}

void TaskTest::Init::run() {
        AUV_DEBUG("Init::run");
        AUV_DEBUG("Waiting for 3 seconds");
        setState<Timer::Timer<Top> >(3.0, Macho::State<Init>());
}
