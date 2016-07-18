#ifndef STATE_MACHINE_H
#define STATE_MACHINE_H

ros::Time startTime;
ros::Duration maxTime;

ros::Duration timeSinceStart();
void startTimer();
bool getTimeout();

#endif
