#ifndef TIMER_H
#define TIMER_H

#include <auv_mission_control/Macho.hpp>
#include <auv_mission_control/StateMachine.hpp>
#include <auv_mission_control/range.hpp>
#include <memory>

// EXAMPLES

// Wait a specified amount of time and call an event while switching to any
// state
// Event must be virtual function in the Top state, with the same event
// implemented in the state being called
// It will call the version in the state you are switching too
// setState<Timer::Timer<Whatever> >(3.0, Macho::Event(&Top::here));
// setState<Timer::Timer<StateToSwitchTo> >(TimeToWait,
// Macho::Event(&Top::eventToCall));

// Wait a specified amount of time and switch to any state
// setState<Timer::Timer<Init> >(3.0);
// setState<Timer::Timer<StateToSwitchTo> >(TimeToWait);

// Queue state switch in main state machine
// setState<Timer::Timer<Init> >(3.0, StateMachine::Gate::alias());
// setState<Timer::Timer<CurrentState> >(3.0, StateToSwitchTo::alias());

template <class T> TSUBSTATE(Timer, T) {
  struct Box {
    Box()
        : wait_time_(0), start_time_(0), event_(0), isEventSet(false),
          alias_(TOP::alias()), isAliasSet(false) {}
    double wait_time_;
    double start_time_;

    Macho::IEvent<typename T::Top> *event_;
    bool isEventSet;

    Macho::Alias alias_;
    bool isAliasSet;
  };
  TSTATE(Timer)
  inline void run() {
    if ((ros::Time::now().toSec() - box().start_time_) > box().wait_time_) {
      if (box().isEventSet) {
        LINK::dispatch(box().event_);
      }
      if (box().isAliasSet) {
        SUPER::TOP::box().statemachine_->queueEnable();
        SUPER::TOP::box().statemachine_->template queueStateAlias(box().alias_);
        TOP::setState(SUPER::alias());
      } else {
        TOP::box().self_->queueEnable();
        TOP::setState(SUPER::alias());
      }
    }
    return;
  }

private:
  inline void init(double waitTime) {
    box().wait_time_ = waitTime;
    box().start_time_ = ros::Time::now().toSec();
  }
  inline void init(double waitTime, Macho::IEvent<typename T::Top> *event) {
    box().wait_time_ = waitTime;
    box().start_time_ = ros::Time::now().toSec();
    box().isEventSet = true;
    box().event_ = event;
  }
  // void init(double waitTime, int i);
  inline void init(double waitTime, Macho::Alias alias) {
    box().wait_time_ = waitTime;
    box().start_time_ = ros::Time::now().toSec();
    box().alias_ = alias;
    box().isAliasSet = true;
  }
};

template <class T> TSUBSTATE(Move, T) {
  struct Box {
    Box()
        : is_first_run_(false), wait_time_(0), start_time_(0),
          alias_(TOP::alias()), isAliasSet(false) {}

    bool is_first_run_;

    double wait_time_;
    double start_time_;

    Macho::Alias alias_;
    bool isAliasSet;

    std::vector<AXIS> axis_;
    std::vector<double> values_;
  };
  TSTATE(Move);
  inline void run() {
    if (!box().is_first_run_) {
      box().is_first_run_ = true;

      if (box().axis_.size() != box().values_.size()) {
        ROS_ERROR("[Template State] State received mismatched parameters");

        if (box().isAliasSet) {
          SUPER::TOP::box().statemachine_->queueEnable();
          SUPER::TOP::box().statemachine_->template queueStateAlias(
              box().alias_);
          // TOP::box().self_->template queueStateAlias(TOP::alias());
        } else {
          TOP::box().self_->queueEnable();
          TOP::box().self_->template queueStateAlias(SUPER::alias());
        }
        return;
      }

      for (unsigned i : util::lang::indices(box().axis_)) {
        INPUT input;
        switch (box().axis_.at(i)) {
        case AXIS::SURGE:
        case AXIS::SWAY:
          input = INPUT::IMU_VEL;
          break;
        case AXIS::ROLL:
        case AXIS::PITCH:
        case AXIS::YAW:
          input = INPUT::IMU_POS;
          break;
        case AXIS::HEAVE:
          input = INPUT::DEPTH;
          break;
        }
        SUPER::TOP::box().pm_->setEnabled(box().axis_.at(i), true);
        SUPER::TOP::box().pm_->setSetpoint(box().axis_.at(i), input,
                                           box().values_.at(i));
      }
    }

    for (auto axis : box().axis_) {
      SUPER::TOP::box().pm_->updatePlantState(axis);
    }

    if ((ros::Time::now().toSec() - box().start_time_) > box().wait_time_) {

      if (box().isAliasSet) {
        SUPER::TOP::box().statemachine_->queueEnable();
        SUPER::TOP::box().statemachine_->template queueStateAlias(box().alias_);
        // TOP::box().self_->template queueStateAlias(TOP::alias());
      } else {
        TOP::box().self_->queueEnable();
        TOP::setState(SUPER::alias());
      }
      return;
    }
  }

private:
  inline void init(std::vector<AXIS> axis, std::vector<double> values,
                   double wait_time) {

    box().axis_ = axis;
    box().values_ = values;

    box().wait_time_ = wait_time;
    box().start_time_ = ros::Time::now().toSec();
    ROS_INFO("Move State");
    for (unsigned i : util::lang::indices(box().axis_)) {
      ROS_INFO("Axis %d: %f", (int)box().axis_.at(i), box().values_.at(i));
    }
  }

  inline void init(std::vector<AXIS> axis, std::vector<double> values,
                   double wait_time, Macho::Alias alias) {
    box().axis_ = axis;
    box().values_ = values;

    box().wait_time_ = wait_time;
    box().start_time_ = ros::Time::now().toSec();
    box().alias_ = alias;
    box().isAliasSet = true;

    ROS_INFO("Move State");
    for (unsigned i : util::lang::indices(box().axis_)) {
      ROS_INFO("Axis %d: %f", (int)box().axis_.at(i), box().values_.at(i));
    }
  }
};

#endif
