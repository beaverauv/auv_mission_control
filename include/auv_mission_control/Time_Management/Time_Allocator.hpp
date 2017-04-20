#ifndef TIME_ALLOCATOR
#define TIME_ALLOCATOR

double stubbornness = 0; //Global stoubbornness modifier
double idealism = 0; //Global idealism modifier
double minimumTimePercent;

class Time_Allocator{

public:

  Time_Allocator();
  ~Time_Allocator();

  std::string getTag(){
          return std::string("[Time Allocator]");
  }
  int execute();


private:

}
