#ifndef MULTILATERATION_H
#define MULTILATERATION_H

#include <array>
#include <functional>
#include <math.h>
#include <ros/ros.h>

typedef std::function<double(double, int)> Equation;
typedef std::function<double(Equation, Equation, double, int)> CombinedEquation;

class Multilateration {
public:
  Multilateration();
  ~Multilateration();

  double findFunctionIntersection(Equation f00, Equation f01, Equation f10,
                                  Equation f11, int sign);
  void findAllIntersections();
  void printAllIntersections() {}
  auto sq(auto a) { return a * a; }

  // double allIntersections[12][2];
  std::array<std::array<double, 2>, 12> allIntersections;

  double xCurrent;
  double xInitial = 10;
  double xChange = 0;
  double NewtonRaphsonXThresh = .0000000000001;
  int iteratorNewton;
  int maxIterations = 10000;
  int arrayCounter = 0;

private:
  float xDist = 1;
  // the horizontal PHYSICAL mounting distance of the hydrophones
  float yDist = 1;
  // the vertical PHYSICAL mounting distance of the hydrophones

  double a = 0.115784275175;

  double b = 0.935821596706;

  double c = 0.0886131503014;

  double d = 0.855231827134;

  Equation f1 = [&](double x, int sign) {
    return 1 + (sign * sqrt((1 - a) - ((sq(x) / a) - xDist)));
  };

  Equation f2 = [&](double x, int sign) {
    return sign * sqrt(b * (1 + sq(x + 1) / (1 - b)));
  };

  Equation f3 = [&](double x, int sign) {
    return -1 + (sign * sqrt((1 - c) * ((sq(x) / c) - 1)));
  };

  Equation f4 = [&](double x, int sign) {
    return sign * sqrt(d * (1 + sq(x - 1) / (1 - d)));
  };

  Equation fd1 = [&](double x, int sign) {
    return ((sign > 0) ? (x - a * x) : (a * x - x)) /
           (a * sqrt(((a - 1) * (a - sq(x))) / a));
  };

  Equation fd2 = [&](double x, int sign) {
    return (b * x + b) / (((sign > 0) ? (1 - b) : (b - 1)) *
                          sqrt(b * (1 - (sq(x + 1) / (b - 1)))));
  };

  Equation fd3 = [&](double x, int sign) {
    return ((sign > 0) ? (x - c * x) : (c * x - x)) /
           (c * sqrt(((c - 1) * (c - sq(x))) / c));
  };

  Equation fd4 = [&](double x, int sign) {
    return (d * x - d) / (((sign > 0) ? (1 - d) : (d - 1)) *
                          sqrt(d * (1 - sq(x - 1) / (d - 1))));
  };

  CombinedEquation functionCombined = [](Equation Eq1, Equation Eq2, double x,
                                         int sign) {
    return Eq1(x, sign) - Eq2(x, sign);
  };
};

#endif
