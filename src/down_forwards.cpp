#include "ros/ros.h"
#include "std_msgs/Float64.h"
#include "math.h"

int main(int argc, char **argv)
{


  ros::init(argc, argv, "down_forwards");
  ros::NodeHandle n;
  ros::Publisher controlEffort_surge_pub = n.advertise<std_msgs::Float64>("controlEffort_Surge", 100);
//  ros::Subscriber sub_movement = n.subscribe("movement", 1, headingCallback);
  ros::Publisher setpoint_heave = n.advertise<std_msgs::Float64>("yum", 100);


std_msgs::Float64 depth;
depth.data = -0.35;
setpoint_heave.publish(depth);

ROS_INFO("going to depth");

std_msgs::Float64 effort;
effort.data = 0;

controlEffort_surge_pub.publish(effort);


ros::Duration(5).sleep();
ROS_INFO("going forwards");
effort.data = 20;

controlEffort_surge_pub.publish(effort);

ros::Duration(5).sleep();
ROS_INFO("all done");

ros::shutdown();


}
