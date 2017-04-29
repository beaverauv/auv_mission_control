#ifndef MOVE_H_
#define MOVE_H_

#include <auv_mission_control/Macho.hpp>
#include <auv_mission_control/StateMachine.hpp>
#include <auv_mission_control/range.hpp>
#include <memory>

template <class T> TSUBSTATE(Move, T) {
  struct Box {
    Box()
        : is_first_run_(false), wait_time_(0), start_time_(0),
          alias_(TOP::alias()), is_alias_set_(false) {}

    bool is_first_run_;

    double wait_time_;
    double start_time_;

    Macho::Alias alias_;
    bool is_alias_set_;

    std::vector<AXIS> axis_;
    std::vector<double> values_;
  };
  AUV_TSTATE(Move);
  inline void run() {
    if (!box().is_first_run_) {
      box().is_first_run_ = true;

      if (!T::pm())
        T::AUV_ERROR("[Template State] pm pointer is null");

      if (box().axis_.size() != box().values_.size()) {
        T::AUV_ERROR("[Template State] State received mismatched parameters");
        // printState();
        if (box().is_alias_set_) {
          T::mission()->queueEnable();
          T::mission()->template queueStateAlias(box().alias_);
        } else {
          T::self()->queueEnable();
          T::self()->template queueStateAlias(T::alias());
        }
        return;
      }
    }
    for (unsigned i : util::lang::indices(box().axis_)) {
      if (hasPidFeedback(box().axis_.at(i))) {
        T::pm()->setEnabled(box().axis_.at(i), true);
        T::pm()->setSetpoint(box().axis_.at(i), getAxisInput(box().axis_.at(i)),
                             box().values_.at(i));
      } else {
        T::pm()->setEnabled(box().axis_.at(i), false);
        T::pm()->setControlEffort(box().axis_.at(i), box().values_.at(i));
      }

      T::pm()->updatePlantState(box().axis_.at(i));
    }

    if ((ros::Time::now().toSec() - box().start_time_) > box().wait_time_) {
      printState();
      if (box().is_alias_set_) {
        T::mission()->queueEnable();
        T::mission()->template queueStateAlias(box().alias_);
      } else {
        T::self()->queueEnable();
        T::setState(T::alias());
      }
      return;
    }
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
  inline void printAxis() {
    for (unsigned i : util::lang::indices(box().axis_)) {
      T::AUV_INFO("Axis %s: %.1f",
                  T::pm()->getAxisName(box().axis_.at(i)).c_str(),
                  box().values_.at(i));
    }
  }

  inline bool hasPidFeedback(AXIS axis) {
    if (axis == AXIS::YAW || axis == AXIS::HEAVE) {
      return true;
    } else {
      return false;
    }
  }

  inline INPUT getAxisInput(AXIS axis) {
    if (axis == AXIS::YAW)
      return INPUT::IMU_POS;
    else if (axis == AXIS::HEAVE)
      return INPUT::DEPTH;
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
    printAxis();
  }

  inline void init(std::vector<AXIS> axis, std::vector<double> values,
                   double wait_time, Macho::Alias alias) {
    box().axis_ = axis;
    box().values_ = values;

    box().wait_time_ = wait_time;
    box().start_time_ = ros::Time::now().toSec();
    box().alias_ = alias;
    box().is_alias_set_ = true;
    T::AUV_INFO("Switched to Move State");
    T::AUV_INFO("Waiting for %.1f seconds", box().wait_time_);
    printAxis();
  }
};

#endif
