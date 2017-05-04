#ifndef TASK_H
#define TASK_H

#include <auv_mission_control/Logger.hpp>
#include <auv_mission_control/Macho.hpp>
#include <auv_mission_control/enum.hpp>
#include <boost/type_index.hpp>

class StateMachine;
class PointerHandler;

enum class MISSION { Base, Test, Full, GateOnly };
enum class TASK { Test, Example, Gate, Buoy, Marker };
enum class AXIS { SURGE, SWAY, HEAVE, ROLL, PITCH, YAW };
enum class INPUT { CAM_FRONT, CAM_BOTTOM, IMU_POS, IMU_ACCEL, DEPTH };

#define SUB_SUCCEEDED 0
#define SUB_TIMEOUT 1
#define SUB_KILL 2

#define MAXWIDTH 7

#define QUEUE_ACTION(Action, ...) self()->queueState<Action<Idle>>(__VA_ARGS__);

#define QUEUE_STATE(State, ...) self()->queueState<State>(__VA_ARGS__);

#define AUV_TOPSTATE(Top) TOPSTATE(Top), Logger

#define AUV_SUBSTATE(STATE, SUPERSTATE) SUBSTATE(STATE, SUPERSTATE)

#define AUV_STATE(S)                                                           \
  STATE(S);                                                                    \
  virtual std::string getStateTag() {                                          \
    return std::string("[State ") + std::string(#S) + std::string("]");        \
  }                                                                            \
                                                                               \
  std::string getTag() { return TOP::box().self_->getTag(); }

#define AUV_TSTATE(S)                                                          \
  TSTATE(S);                                                                   \
  virtual std::string getStateTag() {                                          \
    return std::string("[State ") + std::string(#S) + std::string("]");        \
  }                                                                            \
                                                                               \
  std::string getTag() { return TOP::box().self_->getTag(getStateTag()); }

#define AUV_LOG_TAG(Class)                                                     \
  std::string getTag() {                                                       \
    std::stringstream ss;                                                      \
    std::string task = std::string(#Class) + std::string("]");                 \
                                                                               \
    ss << std::string("[") << std::right << std::setw(MAXWIDTH)                \
       << std::setfill(' ') << task;                                           \
    std::string str = ss.str();                                                \
    std::transform(str.begin(), str.end(), str.begin(), ::toupper);            \
    return str + sm_->getStateTag();                                           \
  }                                                                            \
  std::string getTag(std::string state_tag) {                                  \
    std::stringstream ss;                                                      \
    std::string task = std::string(#Class) + std::string("]");                 \
                                                                               \
    ss << std::string("[") << std::right << std::setw(MAXWIDTH)                \
       << std::setfill(' ') << task;                                           \
    std::string str = ss.str();                                                \
    std::transform(str.begin(), str.end(), str.begin(), ::toupper);            \
    return str + state_tag;                                                    \
  }

#define AUV_LOG_TAG_NO_SM(Class)                                               \
  std::string getTag() {                                                       \
    std::stringstream ss;                                                      \
    std::string task = std::string(#Class) + std::string("]");                 \
                                                                               \
    ss << std::string("[") << std::right << std::setw(MAXWIDTH)                \
       << std::setfill(' ') << task;                                           \
    std::string str = ss.str();                                                \
    std::transform(str.begin(), str.end(), str.begin(), ::toupper);            \
    return str;                                                                \
  }

#define AUV_CREATE_FUNCTIONS(T)                                                \
                                                                               \
  Macho::Machine<T::Top> sm_;                                                  \
  ros::NodeHandle nh_;                                                         \
  typedef Logger INHERITED;                                                    \
  std::string getParamPath();                                                  \
  AUV_CREATE_QUEUE(T, sm_);

#define AUV_CREATE_MISSION_FUNCTIONS(T)                                        \
  Macho::Machine<T::Top> sm_;                                                  \
  ros::NodeHandle nh_;                                                         \
  typedef Base INHERITED;                                                      \
  AUV_CREATE_QUEUE(T, sm_);

#define AUV_CREATE_PARAM_LOADER(T)                                             \
  std::string T::getParamPath() {                                              \
    return std::string(ph()->mission_str() + "/" +                             \
                       Enum<TASK>::Parse(TASK::T));                            \
  }

#define AUV_CREATE_TOP_STATE(Class)                                            \
                                                                               \
  int execute();                                                               \
                                                                               \
  auto self() { return this; }                                                 \
                                                                               \
  AUV_TOPSTATE(Top) {                                                          \
                                                                               \
    struct Box {                                                               \
      Box() {}                                                                 \
      std::shared_ptr<Class> self_;                                            \
    };                                                                         \
                                                                               \
    AUV_STATE(Top);                                                            \
                                                                               \
    virtual void run() { setState<Init>(); }                                   \
                                                                               \
    auto self() { return box().self_; }                                        \
    auto ph() { return self()->ph_; }                                          \
    auto pm() { return self()->pm(); }                                         \
    auto sm() { return self()->sm(); }                                         \
    auto mission() { return self()->mission(); }                               \
    auto cam() { return self()->cam(); }                                       \
    auto vision() { return self()->vision(); }                                 \
    auto test() { return self()->test(); }                                     \
    auto example() { return self()->example(); }                               \
    auto gate() { return self()->gate(); }                                     \
    auto buoy() { return self()->buoy(); }                                     \
    auto marker() { return self()->marker(); }                                 \
    auto getParamPath() { return self()->getParamPath(); }                     \
                                                                               \
  private:                                                                     \
    void init(Class *self) { box().self_ = std::shared_ptr<Class>(self); }     \
  };

#define AUV_CREATE_MISSION_TOP_STATE(Class)                                    \
                                                                               \
  int execute();                                                               \
                                                                               \
  auto self() { return this; }                                                 \
                                                                               \
  AUV_TOPSTATE(Top) {                                                          \
                                                                               \
    struct Box {                                                               \
      Box() {}                                                                 \
      std::shared_ptr<Class> self_;                                            \
    };                                                                         \
                                                                               \
    AUV_STATE(Top);                                                            \
                                                                               \
    virtual void run() { setState<Init>(); }                                   \
                                                                               \
    auto self() { return box().self_; }                                        \
    auto ph() { return self()->ph_; }                                          \
    auto pm() { return self()->pm(); }                                         \
    auto sm() { return self()->sm(); }                                         \
    auto mission() { return self()->mission(); }                               \
    auto cam() { return self()->cam(); }                                       \
    auto vision() { return self()->vision(); }                                 \
    auto test() { return self()->test(); }                                     \
    auto example() { return self()->example(); }                               \
    auto gate() { return self()->gate(); }                                     \
    auto buoy() { return self()->buoy(); }                                     \
    auto marker() { return self()->marker(); }                                 \
                                                                               \
  private:                                                                     \
    void init(Class *self) { box().self_ = std::shared_ptr<Class>(self); }     \
  };

#define AUV_CREATE_STATE(State)                                                \
  AUV_SUBSTATE(State, Top) {                                                   \
                                                                               \
    AUV_STATE(State);                                                          \
                                                                               \
    void run();                                                                \
  };

#define AUV_CREATE_EMPTY_STATE(State)                                          \
  AUV_SUBSTATE(State, Top) {                                                   \
                                                                               \
    AUV_STATE(State);                                                          \
                                                                               \
    void run() {}                                                              \
  };

#define AUV_CREATE_QUEUE(T, state)                                             \
public:                                                                        \
  typedef std::vector<Macho::IEvent<typename T::Top> *> EventQueue;            \
  EventQueue eventqueue_;                                                      \
                                                                               \
  bool eventqueue_should_switch_ = false;                                      \
  bool eventqueue_last_empty_ = false;                                         \
                                                                               \
  void queueEnable() { eventqueue_should_switch_ = true; }                     \
                                                                               \
  void queueDisable() { eventqueue_should_switch_ = false; }                   \
                                                                               \
  void queueState(Macho::Alias alias) {                                        \
    eventqueue_.push_back(Macho::StateEvent<T::Top>(alias));                   \
  }                                                                            \
                                                                               \
  template <class S, class... P> void queueState(P... p) {                     \
    eventqueue_.push_back(Macho::StateEvent<T::Top, S>(p...));                 \
  }                                                                            \
  template <class S, class... P>                                               \
  void queueState(std::vector<AXIS> axis, std::vector<double> values,          \
                  P... p) {                                                    \
    eventqueue_.push_back(Macho::StateEvent<T::Top, S>(axis, values, p...));   \
  }                                                                            \
                                                                               \
  int checkEventQueue() {                                                      \
    if (eventqueue_.empty()) {                                                 \
      eventqueue_last_empty_ = true;                                           \
      eventqueue_should_switch_ = false;                                       \
      return 1;                                                                \
    }                                                                          \
                                                                               \
    if (eventqueue_last_empty_ || eventqueue_should_switch_) {                 \
      state.dispatch(eventqueue_.at(0));                                       \
                                                                               \
      eventqueue_.erase(eventqueue_.begin());                                  \
      if (eventqueue_last_empty_)                                              \
        eventqueue_should_switch_ = false;                                     \
      eventqueue_last_empty_ = false;                                          \
      return 0;                                                                \
    }                                                                          \
                                                                               \
    return 1;                                                                  \
  }

#endif
