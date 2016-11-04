#ifndef TASK_H
#define TASK_H

#include <stdio.h>
#include <cstdarg>
#include <ros/ros.h>


class Task {
public:
        Task();
        ~Task();



        void AUV_INFO(char * format, ...);
        void AUV_ERROR(char * format, ...);
        void AUV_INFO(std::string string, ...);
        void AUV_ERROR(std::string string, ...);


        virtual std::string getTag();

private:


protected:
    Task(const Task & task) { /*...*/ }

};




#endif
