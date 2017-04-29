#include <auv_mission_control/TemplateStates.hpp>
#include <auv_mission_control/mission/Test.hpp>

namespace Mission {

int Test::execute() {
  pm()->ensureDepth();
  pm()->ensureYaw();

  if (checkEventQueue()) {
    sm_->run();
  }
}

void Test::Init::run() {
  // setState<Timer::Timer>(3.0, Buoy::alias);
  // AUV_DEBUG("Test::Init::run: ID %d", Macho::State<Top>()._KeyData)
  // Macho::IEvent<Test::Top> *event =
  //     Macho::Event(&Test::Top::whatever);
  // setState<Timer::Timer>
  // setState<TimerOld<Init>>(3.0);
  // setState<MoveOld<Init>>({INPUT::IMU_POS}, {10.0}, 3.0);

  // setState<MoveOld<Test>>(std::initializer_list<INPUT>{INPUT::IMU_POS},
  //  std::initializer_list<double>{10.0}, 3.0);
  // setState<MoveOld<Test>>(AxisVec{AXIS::YAW, AXIS::HEAVE, AXIS::ROLL},
  //                      ValuesVec{10.0, 5.0, 45.0}, 3.0);
  // AUV_INFO("%s", ph().pm_->getAxisName(0).c_str());
  self()->queueEnable();

  AUV_INFO("Starting movement...");

  // Top::box().self_->queueState<TimerOld<Nowhere>>(10.0);
  //
  // Top::box().self_->queueState<MoveOld<Nowhere>>(
  //     {AXIS::YAW, AXIS::HEAVE, AXIS::ROLL}, {10.0, 5.0, 45.0}, 3.0);

  // Top::box().self_->queueState<MoveOld<Test>>({AXIS::YAW, AXIS::SURGE},
  //                                          {6.0, 10.0}, 400.0);

  QUEUE_ACTION(Move, {AXIS::SURGE}, {26.0}, 10.0)
  // std::vector<INPUT> in1 = {INPUT::IMU_POS};
  // std::vector<double> in2 = {10.0};
  // foo<char, int, float> f1;
  // foo<char, int> f2;
  // bar(f1, f2, 9);

  QUEUE_STATE(Tested);

  // setState<MoveOld<Init>>(INPUTS{INPUT::IMU_POS}, 3.0);
}

void Test::Tested::run() { test()->execute(); }

void Test::Example::run() { example()->execute(); }

void Test::Gate::run() { gate()->execute(); }

void Test::Buoy::run() { buoy()->execute(); }

void Test::Marker::run() { marker()->execute(); }

void Test::Kill::run() {
  // AUV_ERROR("Kill::run");
  // setState<Init>();
}
}
