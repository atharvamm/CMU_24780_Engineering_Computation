#ifndef TRIANGULATE_H
#define TRIANGULATE_H

#include <vector>
#include <cmath>
#include "primitives.h"

bool isEar(const Point& a, const Point& b, const Point& c, const std::vector<Point>& polygon);

std::vector<std::vector<Point>> triangulateConcavePolygon(const std::vector<Point>& polygon);

#endif  // TRIANGULATE_H
