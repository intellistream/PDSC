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

// ostream operator for Point
std::ostream &operator<<(std::ostream &os, const Point &point) {
  os << "Point[";
  if (point.features.size() <= 5) {
    for (int i = 0; i < point.features.size(); i++) {
      os << point.features[i];
      if (i < point.features.size() - 1) {
        os << ", ";
      }
    }
  } else {
    for (int i = 0; i < 3; i++) {
      os << point.features[i] << ", ";
    }
    os << "..., ";
    for (int i = point.features.size() - 3; i < point.features.size(); i++) {
      os << point.features[i];
      if (i < point.features.size() - 1) {
        os << ", ";
      }
    }
  }
  os << "]";
  return os;
}

// ostream operator for Dataset
std::ostream &operator<<(std::ostream &os, const Dataset &dataset) {
  os << dataset.name << "{\n";
  os << "\t# Points: " << dataset.num_points << "\n";
  os << "\t# Dimensions: " << dataset.dim << "\n";
  os << "\t# True Clusters: " << dataset.num_true_clusters << "\n";
  if (dataset.points.size() <= 10) {
    for (int i = 0; i < dataset.points.size(); i++) {
      os << '\t' << dataset.points[i];
      if (i < dataset.points.size() - 1) {
        os << ", \n";
      }
    }
  } else {
    for (int i = 0; i < 5; i++) {
      os << '\t' << dataset.points[i] << ", \n";
    }
    os << "\t..., \n";
    for (int i = dataset.points.size() - 5; i < dataset.points.size(); i++) {
      os << '\t' << dataset.points[i];
      if (i < dataset.points.size() - 1) {
        os << ", \n";
      }
    }
  }
  os << "\n}";
  return os;
}

#endif // POINTGENERATION_HPP
