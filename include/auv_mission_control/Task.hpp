#ifndef TASK_H
#define TASK_H

#include <auv_mission_control/Logger.hpp>
#include <auv_mission_control/Macho.hpp>
#include <boost/type_index.hpp>

class StateMachine;
class PointerHandler;

enum class MISSION { TEST, FULL, GATE_ONLY };
enum class TASK { TEST, EXAMPLE, GATE, BUOY, MARKER };
enum class AXIS { SURGE, SWAY, HEAVE, ROLL, PITCH, YAW };
enum class INPUT { CAM_FRONT, CAM_BOTTOM, IMU_POS, IMU_ACCEL, DEPTH };

#define SUB_SUCCEEDED 0
#define SUB_TIMEOUT 1
#define SUB_KILL 2

#define MAXWIDTH 7

#define QUEUE_ACTION(Action, ...)                                              \
  self()->queueState<Action<Nowhere>>(__VA_ARGS__);

#define QUEUE_STATE(State, ...) self()->queueState<State>(__VA_ARGS__);

#define TEST(TT, SS, ...)                                                      \
  eventqueue_.push_back(                                                       \
      Macho::Event(&TT::Top::setMachineState<SS>, __VA_ARGS__));

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
                                                                               \
  AUV_CREATE_QUEUE(T, sm_);

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
    AUV_MACHINE_FUNCTIONS();                                                   \
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

#define AUV_MACHINE_FUNCTIONS()                                                \
                                                                               \
  void setMachineStateAlias(Macho::Alias alias) { setState(alias); }           \
                                                                               \
  template <class S, class... P> void setMachineState(P... p) {                \
    setState<S>(p...);                                                         \
  }                                                                            \
                                                                               \
  template <class S, class P1> void setMachineState(P1 p1) {                   \
    setState<S>(p1);                                                           \
  }                                                                            \
                                                                               \
  template <class S, class P1, class P2> void setMachineState(P1 p1, P2 p2) {  \
    setState<S>(p1, p2);                                                       \
  }                                                                            \
                                                                               \
  template <class S>                                                           \
  void setMachineState(std::vector<AXIS> axis, std::vector<double> values,     \
                       double wait_time) {                                     \
    setState<S>(axis, values, wait_time);                                      \
  }                                                                            \
                                                                               \
  template <class S>                                                           \
  void setMachineState(std::vector<AXIS> axis, std::vector<double> values,     \
                       double wait_time, Macho::Alias alias) {                 \
    setState<S>(axis, values, wait_time, alias);                               \
  }

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
  void queueStateAlias(Macho::Alias alias) {                                   \
    eventqueue_.push_back(Macho::Event(&T::Top::setMachineStateAlias, alias)); \
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
