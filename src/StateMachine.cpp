#include <auv_mission_control/StateMachine.h>


StateMachine::StateMachine(){
        AUV_INFO("Init");
        pm_ = new PidManager(&nh_);
        AUV_DEBUG("Created PM Pointer: %x", pm_);

        cam_ = new Camera();
        AUV_DEBUG("Created Cam pointer: %x", cam_);
        //cam_->startRecording();
        vision_ = new TaskVision(cam_);
        AUV_DEBUG("Created Vision pointer: %x", vision_);


        //gate_ = new TaskGate(pm_, vision_);
        //marker_ = new TaskMarker(pm_, vision_);
        buoy_ = new TaskBuoy(pm_, vision_);

}

StateMachine::~StateMachine(){

}


int StateMachine::execute(){
        AUV_INFO("Execute");
        // while(true){
        //   ros::spinOnce();
        //   AUV_DEBUG("%f", pm_->getDepth());
        // }
}
