#include <auv_mission_control/TaskBuoy.h>

TaskBuoy::TaskBuoy(){
}


TaskBuoy::TaskBuoy(std::shared_ptr<PidManager> pm, std::shared_ptr<TaskVision> vision){
        AUV_INFO("Constructor");
        AUV_DEBUG("[Pointers] [PM]: %x", pm);
        AUV_DEBUG("[Pointers] [VISION]: %x", vision);
        stateBuoy_->setLocalPointers(pm, vision);
        //stateBuoy_->setTaskVision(vision);


}

TaskBuoy::~TaskBuoy(){

}

void TaskBuoy::prepare(){
        stateBuoy_->initialize();
}


int TaskBuoy::execute(){

        stateBuoy_->run();
        //pm_->setPidEnabled("ALL", true);//turns on all 6 pid controllers

        // pm_->setZero(AXIS_YAW);
        // pm_->setSetpoint(AXIS_YAW, INPUT_IMU_POS, 0);
        // pm_->setSetpoint(AXIS_HEAVE, INPUT_DEPTH, -1.25);
        // // pm_->taskDelay(5);
        // currentColor = COLOR_RED;
        // depthCounter = 0;
        // while (ros::ok) {
        //
        //         ros::spinOnce();
        //
        //         pm_->setSetpoint(AXIS_YAW, INPUT_IMU_POS, 0);
        //
        //         ROS_INFO("\033[2J\033[1;1H");
        //
        //         vision_->findBuoy(currentColor);
        //         double posXcorrected = vision_->getBuoyCoordX();
        //         double posYcorrected = vision_->getBuoyCoordY();
        //         double dArea = vision_->getBuoyArea();
        //         switch(action) {
        //
        //         case 0: {
        //                 pm_->setSetpoint(AXIS_HEAVE, INPUT_DEPTH, -2.75);
        //                 double errorBuoy = fabs(-2.75 - pm_->getDepth());
        //                 if(false)
        //                         //if(errorBuoy >= .2)
        //                         action = 0;
        //                 else
        //                         action = 1;
        //                 break;
        //         }
        //         case 1: {  //approach first buoy (red) YO MA OG GIMMME CENTROID OF RED
        //                 currentColor = COLOR_RED;
        //                 if(waitCounter < 1) {
        //                         waitTimer.start();
        //                         waitCounter++;
        //                 }
        //
        //                 if(waitTimer.getTime() < 10) {
        //                         pm_->setControlEffort(AXIS_SURGE, 0);
        //                 }
        //                 else{
        //                         pm_->setControlEffort(AXIS_SURGE, 15);
        //                 }
        //                 pm_->setSetpoint(AXIS_YAW, INPUT_CAM_FRONT, 360);
        //                 pm_->setSetpoint(AXIS_SWAY, INPUT_CAM_FRONT, 360);
        //                 pm_->setSetpoint(AXIS_HEAVE, INPUT_CAM_FRONT, 240);
        //
        //                 if(posYcorrected == posYcorrected) //checks for NaN, doesn't set if is
        //                         pm_->setPlantState(AXIS_HEAVE, posYcorrected);
        //                 else
        //                         pm_->setPlantState(AXIS_HEAVE, 240);
        //                 if(posXcorrected == posXcorrected) {
        //                         pm_->setPlantState(AXIS_YAW, posXcorrected);
        //                         pm_->setPlantState(AXIS_YAW, posXcorrected);
        //                 }
        //                 else{
        //                         pm_->setPlantState(AXIS_YAW, 360);
        //                         pm_->setPlantState(AXIS_SWAY, 360);
        //                 }
        //                 if(vision_->getBuoyArea() > 7000000)
        //                         action = 2;
        //                 else
        //                         action = 1;
        //                 break;
        //         }
        //
        //
        //         case 2: {  //BOP IT! (ram it)
        //                 ROS_INFO("YAAARRRGGGG I BE RAMMING IT");
        //                 if(ramRedCounter < 1) {
        //                         ramRed.start();
        //                         ramRedCounter++;
        //                         pm_->setZero(AXIS_YAW);
        //                 }
        //
        //                 pm_->setSetpoint(AXIS_HEAVE, INPUT_DEPTH, redDepth);
        //                 ROS_INFO("RED DEPTH %f", redDepth);
        //                 pm_->setPlantState(AXIS_HEAVE, pm_->getDepth());
        //                 pm_->setPidEnabled(AXIS_SWAY, 0);
        //                 pm_->setControlEffort(AXIS_SWAY, 0);
        //                 pm_->setSetpoint(AXIS_YAW, INPUT_IMU_POS, 0);
        //                 pm_->setPlantState(AXIS_YAW, pm_->getYaw());
        //                 if(ramRed.getTime() <=3)
        //                         pm_->setControlEffort(AXIS_SURGE, 45);
        //                 else if(ramRed.getTime() >3 && ramRed.getTime() <= 6)
        //                         pm_->setControlEffort(AXIS_SURGE, 0);
        //                 else if(ramRed.getTime() > 6 && ramRed.getTime() <= 10)
        //                         pm_->setControlEffort(AXIS_SURGE, -35);
        //                 else{
        //                         pm_->setControlEffort(AXIS_SURGE, 0);
        //                         action = 3;
        //                         return 0;
        //                         break;
        //                 }
        //         }
        //         };
        // }
}

void TaskBuoy::Init::run() {
        AUV_DEBUG("Init::run");
        AUV_DEBUG("Waiting for 3 seconds");
        //setState<Waiting>(3.0, Macho::State<Init>());
}
