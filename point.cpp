/*
 * Copyright 2024 IntelliStream team (https://github.com/intellistream)
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef POINTGENERATION_HPP
#define POINTGENERATION_HPP

#include "point.hpp"

#include <cstdlib>
#include <ctime>
#include <vector>

std::vector<Point> generateDataPoints(int numPoints, int dimensions) {
  std::vector<Point> dataPoints(numPoints, Point(dimensions));
  srand(static_cast<unsigned>(time(0)));
  for (int i = 0; i < numPoints; ++i) {
    for (int j = 0; j < dimensions; ++j) {
      dataPoints[i].coordinates[j] =
          static_cast<double>(rand()) / RAND_MAX * 100;
    }
    dataPoints[i].timestamp = i; // Example timestamp, could be any sequence
  }
  return dataPoints;
}

// ostream operator for Point
std::ostream &operator<<(std::ostream &os, const Point &point) {
  os << "Point(";
  for (int i = 0; i < point.coordinates.size(); i++) {
    os << point.coordinates[i];
    if (i < point.coordinates.size() - 1) {
      os << ", ";
    }
  }
  os << ")";
  return os;
}

#endif // POINTGENERATION_HPP
