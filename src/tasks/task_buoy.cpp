Task_Buoy::Task_Buoy(){
}


Task_Buoy::Task_Buoy(Pid_Manager* pm, CameraManager* cam) : pm_(*pm), cam_(*cam){
}

Task_Buoy::~Task_Buoy(){

}


int Task_Buoy::execute(){

  while (ros::ok){

    ros::spinOnce();

  }


}
