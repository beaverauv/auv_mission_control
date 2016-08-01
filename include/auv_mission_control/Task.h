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

        virtual std::string getTag();

private:



};




#endif
