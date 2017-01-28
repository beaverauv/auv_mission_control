#include <auv_mission_control/Multilateration.hpp>

Multilateration::Multilateration() {}

Multilateration::~Multilateration() {}

double Multilateration::findFunctionIntersection(Equation f00, Equation f01,
                                                 Equation f10, Equation f11,
                                                 int sign) {
  xCurrent = xInitial;
  xChange = 1;
  int numberOfIterations = 0;
  while (fabs(xChange) > NewtonRaphsonXThresh) {
    xChange = functionCombined(f00, f01, xCurrent, sign) /
              functionCombined(f10, f11, xCurrent, sign);
    xCurrent -= xChange;
    numberOfIterations++;
    if (numberOfIterations > maxIterations) {
      printf("PANIC\n");
      break;
    }
  }
  return xCurrent;
}

void Multilateration::findAllIntersections() {
  allIntersections.at(0).at(0) = findFunctionIntersection(f1, f2, fd1, fd2, 1);
  allIntersections.at(0).at(1) =
      functionCombined(f1, f2, allIntersections.at(0).at(0), 1);
  printf("%.18f, %.18f\n", allIntersections.at(0).at(0),
         allIntersections.at(0).at(1));

  allIntersections.at(1).at(0) = findFunctionIntersection(f1, f3, fd1, fd3, 1);
  allIntersections.at(1).at(1) =
      functionCombined(f1, f3, allIntersections.at(1).at(0), 1);
  printf("%.18f, %.18f\n", allIntersections.at(1).at(1),
         allIntersections.at(1).at(1));

  allIntersections.at(2).at(0) = findFunctionIntersection(f1, f4, fd1, fd4, 1);
  allIntersections.at(2).at(1) =
      functionCombined(f1, f4, allIntersections.at(2).at(0), 1);
  printf("%.18f, %.18f\n", allIntersections.at(2).at(0),
         allIntersections.at(2).at(1));

  allIntersections.at(3).at(0) = findFunctionIntersection(f2, f4, fd2, fd4, 1);
  allIntersections.at(3).at(1) =
      functionCombined(f2, f4, allIntersections.at(3).at(0), 1);
  printf("%.18f, %.18f\n", allIntersections.at(3).at(0),
         allIntersections.at(3).at(1));

  allIntersections.at(4).at(0) = findFunctionIntersection(f3, f4, fd3, fd4, 1);
  allIntersections.at(4).at(1) =
      functionCombined(f3, f4, allIntersections.at(4).at(0), 1);
  printf("%.18f, %.18f\n", allIntersections.at(4).at(0),
         allIntersections.at(4).at(1));
}
