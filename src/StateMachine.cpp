#include <auv_mission_control/StateMachine.h>


StateMachine::StateMachine(){
        AUV_INFO("Init");
        pm_ = new PidManager(&nh_);
        cam_ = new Camera();
        //cam_->startRecording();
        vision_ = new TaskVision(cam_);
//         if( ros::console::set_logger_level(ROSCONSOLE_DEFAULT_NAME, ros::console::levels::Debug) ) {
//    ros::console::notifyLoggerLevelsChanged();
// }

        //gate_ = new TaskQualGate(pm_, vision_);

}

StateMachine::~StateMachine(){

}


int StateMachine::execute(){
        AUV_INFO("Execute");
        while(true){
          ros::spinOnce();
          AUV_DEBUG("%f", pm_->getDepth());
        }
}
