#include <auv_mission_control/Task.h>



Task::Task(){

}


Task::~Task(){

}


void Task::AUV_INFO(char * format, ...){
        char buffer[1024];
        std::va_list args;
        va_start (args, format);
        vsnprintf (buffer, 1023, format, args);

        ROS_INFO("%s %s", getTag().c_str(), buffer);
        //do  something with the error

        va_end (args);
}

void Task::AUV_ERROR(char * format, ...){
        char buffer[1024];
        std::va_list args;
        va_start (args, format);
        vsnprintf (buffer, 1023, format, args);

        ROS_INFO("%s %s", getTag().c_str(), buffer);
        //do  something with the error

        va_end (args);


}

std::string Task::getTag(){
        return std::string("[Default Task]");
}
