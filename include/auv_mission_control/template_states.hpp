#ifndef TIMER_H
#define TIMER_H

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
        : wait_time_(0), start_time_(0), event_(0), isEventSet(false),
          alias_(TOP::alias()), isAliasSet(false) {}

    double wait_time_;
    double start_time_;

    Macho::IEvent<typename T::Top> *event_;
    bool isEventSet;

    Macho::Alias alias_;
    bool isAliasSet;
  };
  AUV_TSTATE(Timer);
  inline void run() {
    if ((ros::Time::now().toSec() - box().start_time_) > box().wait_time_) {
      if (box().isEventSet) {
        LINK::dispatch(box().event_);
      }
      printstate();
      if (box().isAliasSet) {
        T::ph().mission()->queueEnable();
        T::ph().mission()->template queueStateAlias(box().alias_);
        TOP::setState(T::alias());
      } else {
        T::self().queueEnable();
        TOP::setState(T::alias());
      }
    }
    return;
  }
  inline bool isAliasNamed(std::string state) {
    return !state.compare(T::alias().name());
  }
  inline void printstate() {
    if (box().isAliasSet) {
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
    box().isEventSet = true;
    box().event_ = event;
    T::AUV_INFO("Switched to Timer State");
    T::AUV_INFO("Waiting for %.1f seconds", box().wait_time_);
  }
  // void init(double waitTime, int i);
  inline void init(double wait_time, Macho::Alias alias) {
    box().wait_time_ = wait_time;
    box().start_time_ = ros::Time::now().toSec();
    box().alias_ = alias;
    box().isAliasSet = true;
    T::AUV_INFO("Switched to Timer State");
    T::AUV_INFO("Waiting for %.1f seconds", box().wait_time_);
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
  AUV_TSTATE(Move);
  inline void run() {
    if (!box().is_first_run_) {
      box().is_first_run_ = true;

      if (box().axis_.size() != box().values_.size()) {
        T::AUV_ERROR("[Template State] State received mismatched parameters");
        printstate();
        if (box().isAliasSet) {
          T::ph().mission()->queueEnable();
          T::ph().mission()->template queueStateAlias(box().alias_);
        } else {
          T::self().queueEnable();
          T::self().template queueStateAlias(T::alias());
        }
        return;
      }
      for (unsigned i : util::lang::indices(box().axis_)) {
        INPUT input;
        switch (box().axis_.at(i)) {
        case AXIS::SURGE:
        case AXIS::SWAY:
          T::ph().pm_->setEnabled(box().axis_.at(i), false);
          T::ph().pm_->setControlEffort(box().axis_.at(i), box().values_.at(i));
          continue;

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
        T::ph().pm_->setEnabled(box().axis_.at(i), true);
        T::ph().pm_->setSetpoint(box().axis_.at(i), input, box().values_.at(i));
      }
    }

    for (auto axis : box().axis_) {
      T::ph().pm_->updatePlantState(axis);
    }

    if ((ros::Time::now().toSec() - box().start_time_) > box().wait_time_) {
      printstate();
      if (box().isAliasSet) {
        T::ph().mission()->queueEnable();
        T::ph().mission()->template queueStateAlias(box().alias_);
      } else {
        T::self().queueEnable();
        T::setState(T::alias());
      }
      return;
    }
  }
  inline bool isAliasNamed(std::string state) {
    return !state.compare(T::alias().name());
  }
  inline void printstate() {
    if (box().isAliasSet) {
      T::AUV_INFO("Switching to Task %s", box().alias_.name());
    } else {
      if ((isAliasNamed("Nowhere") || isAliasNamed("Test")) ||
          (isAliasNamed("Whatever") || isAliasNamed("Init")))
        return;
      T::AUV_INFO("Switching to State %s", T::alias().name());
    }
  }
  inline void printaxis() {
    for (unsigned i : util::lang::indices(box().axis_)) {
      T::AUV_INFO("Axis %s: %.1f",
                  T::ph().pm_->getAxisName(box().axis_.at(i)).c_str(),
                  box().values_.at(i));
    }
  }

private:
  inline void init(std::vector<AXIS> axis, std::vector<double> values,
                   double wait_time) {

    box().axis_ = axis;
    box().values_ = values;

    box().wait_time_ = wait_time;
    box().start_time_ = ros::Time::now().toSec();
    T::AUV_INFO("Switched to Move State");
    T::AUV_INFO("Waiting for %.1f seconds", box().wait_time_);
    printaxis();
  }

  inline void init(std::vector<AXIS> axis, std::vector<double> values,
                   double wait_time, Macho::Alias alias) {
    box().axis_ = axis;
    box().values_ = values;

    box().wait_time_ = wait_time;
    box().start_time_ = ros::Time::now().toSec();
    box().alias_ = alias;
    box().isAliasSet = true;
    T::AUV_INFO("Switched to Move State");
    T::AUV_INFO("Waiting for %.1f seconds", box().wait_time_);
    printaxis();
  }
};

#endif
