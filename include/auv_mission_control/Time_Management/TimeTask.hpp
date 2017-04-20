#ifndef TIME_TASK
#define TIME_TASK

double stubbornness = 0; //Global stoubbornness modifier
double idealism = 0; //Global idealism modifier
double minimumTimePercent;

class TimeTask{

public:

  TimeTask();
  ~TimeTask();

  std::string getTag(){
          return std::string("[Time Allocator]");
  }
  int execute();


private:

    double stubbornness;
    double idealism;
    double minimumTimePercent;
    int priority;
    int orderSlot;
    double taskTime;
    double taskMinimumTime;
    double taskMaximumTime;
    double maxPoints;
    double minPoints;

}
