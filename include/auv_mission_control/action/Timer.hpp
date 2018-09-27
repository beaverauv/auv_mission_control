#ifndef TIMER_H_
#define TIMER_H_

#include <auv_mission_control/Macho.hpp>
#include <auv_mission_control/StateMachine.hpp>
#include <auv_mission_control/range.hpp>
#include <memory>

// EXAMPLES

// Wait a specified amount of time and call an event while switching to any
// state
// Event must be virtual function in the T state, with the same event
// implemented in the state being called
// It will call the version in the state you are switching too
// setState<Timer::TimerOld<Whatever> >(3.0, Macho::Event(&T::here));
// setState<Timer::TimerOld<StateToSwitchTo> >(TimeToWait,
// Macho::Event(&T::eventToCall));

// Wait a specified amount of time and switch to any state
// setState<Timer::TimerOld<Init> >(3.0);
// setState<Timer::TimerOld<StateToSwitchTo> >(TimeToWait);

// Queue state switch in main state machine
// setState<Timer::TimerOld<Init> >(3.0, StateMachine::Gate::alias());
// setState<Timer::TimerOld<CurrentState> >(3.0, StateToSwitchTo::alias());

template <class T> TSUBSTATE(Timer, T) {
  struct Box {
    Box()
        : wait_time_(0), start_time_(0), event_(0), is_event_set_(false),
          alias_(TOP::alias()), is_alias_set_(false) {}

    double wait_time_;
    double start_time_;

    Macho::IEvent<typename T::Top> *event_;
    bool is_event_set_;

    Macho::Alias alias_;
    bool is_alias_set_;
  };
  AUV_TSTATE(Timer);
  inline void run() {
    if ((ros::Time::now().toSec() - box().start_time_) > box().wait_time_) {
      if (box().is_event_set_) {
        LINK::dispatch(box().event_);
      }
      printState();
      if (box().is_alias_set_) {
        T::mission()->queueEnable();
        T::mission()->queueState(box().alias_);
        TOP::setState(T::alias());
      } else {
        T::self()->queueEnable();
        TOP::setState(T::alias());
      }
    }
    return;
  }
  inline bool isAliasNamed(std::string state) {
    return !state.compare(T::alias().name());
  }
  inline void printState() {
    if (box().is_alias_set_) {
      T::AUV_INFO("Switching to Task %s", box().alias_.name());
    } else {
      if ((isAliasNamed("Nowhere") || isAliasNamed("Test")) ||
          (isAliasNamed("Whatever") || isAliasNamed("Init")))
        return;
      T::AUV_INFO("Switching to State %s", T::alias().name());
    }
  }

private:
  inline void init(double wait_time) {
    box().wait_time_ = wait_time;
    box().start_time_ = ros::Time::now().toSec();
    T::AUV_INFO("Switched to Timer State");
    T::AUV_INFO("Waiting for %.1f seconds", box().wait_time_);
  }
  inline void init(double wait_time, Macho::IEvent<typename T::Top> *event) {
    box().wait_time_ = wait_time;
    box().start_time_ = ros::Time::now().toSec();
    box().is_event_set_ = true;
    box().event_ = event;
    T::AUV_INFO("Switched to Timer State");
    T::AUV_INFO("Waiting for %.1f seconds", box().wait_time_);
  }
  inline void init(double wait_time, Macho::Alias alias) {
    box().wait_time_ = wait_time;
    box().start_time_ = ros::Time::now().toSec();
    box().alias_ = alias;
    box().is_alias_set_ = true;
    T::AUV_INFO("Switched to Timer State");
    T::AUV_INFO("Waiting for %.1f seconds", box().wait_time_);
  }
};

#endif