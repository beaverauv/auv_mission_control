#include <auv_mission_control/Task.hpp>



Task::Task(){

}


Task::~Task(){

}

void Task::AUV_FATAL(const char * format, ...){
        char buffer[1024];
        std::va_list args;
        va_start (args, format);
        vsnprintf (buffer, 1023, format, args);

        ROS_FATAL("%s %s", getTag().c_str(), buffer);
        //do  something with the error

        va_end (args);


}

void Task::AUV_FATAL(std::string string, ...){
        const char * format = string.c_str();
        char buffer[1024];
        std::va_list args;
        va_start (args, string);
        vsnprintf (buffer, 1023, format, args);

        ROS_FATAL("%s %s", getTag().c_str(), buffer);
        //do  something with the error

        va_end (args);


}


void Task::AUV_ERROR(const char * format, ...){
        char buffer[1024];
        std::va_list args;
        va_start (args, format);
        vsnprintf (buffer, 1023, format, args);

        ROS_ERROR("%s %s", getTag().c_str(), buffer);
        //do  something with the error

        va_end (args);


}

void Task::AUV_ERROR(std::string string, ...){
        const char * format = string.c_str();
        char buffer[1024];
        std::va_list args;
        va_start (args, string);
        vsnprintf (buffer, 1023, format, args);

        ROS_ERROR("%s %s", getTag().c_str(), buffer);
        //do  something with the error

        va_end (args);


}

void Task::AUV_WARN(const char * format, ...){
        char buffer[1024];
        std::va_list args;
        va_start (args, format);
        vsnprintf (buffer, 1023, format, args);

        ROS_WARN("%s %s", getTag().c_str(), buffer);
        //do  something with the error

        va_end (args);


}

void Task::AUV_WARN(std::string string, ...){
        const char * format = string.c_str();
        char buffer[1024];
        std::va_list args;
        va_start (args, string);
        vsnprintf (buffer, 1023, format, args);

        ROS_WARN("%s %s", getTag().c_str(), buffer);
        //do  something with the error

        va_end (args);


}


void Task::AUV_INFO(const char * format, ...){
        char buffer[1024];
        std::va_list args;
        va_start (args, format);
        vsnprintf (buffer, 1023, format, args);

        ROS_INFO("%s %s", getTag().c_str(), buffer);
        //do  something with the error

        va_end (args);
}

void Task::AUV_INFO(std::string string, ...){
        const char * format = string.c_str();
        char buffer[1024];
        std::va_list args;
        va_start (args, string);
        vsnprintf (buffer, 1023, format, args);

        ROS_INFO("%s %s", getTag().c_str(), buffer);
        //do  something with the error

        va_end (args);
}

void Task::AUV_DEBUG(const char * format, ...){
        char buffer[1024];
        std::va_list args;
        va_start (args, format);
        vsnprintf (buffer, 1023, format, args);

        ROS_DEBUG("%s %s", getTag().c_str(), buffer);
        //do  something with the error

        va_end (args);
}

void Task::AUV_DEBUG(std::string string, ...){
        const char * format = string.c_str();
        char buffer[1024];
        std::va_list args;
        va_start (args, string);
        vsnprintf (buffer, 1023, format, args);

        ROS_DEBUG("%s %s", getTag().c_str(), buffer);
        //do  something with the error

        va_end (args);
}

std::string Task::getTag(){
        return std::string("[Default Task]");
}
