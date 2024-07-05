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

#ifndef DSTREAM_HPP
#define DSTREAM_HPP

#include "point.hpp"

#include <cmath>
#include <limits>
#include <string>
#include <unordered_map>
#include <vector>

const int CELL_SIZE = 1;

struct Cell {
  std::vector<double> coordinates;
  double density;
  double timestamp;

  Cell(int dimensions)
      : coordinates(dimensions, 0.0), density(0.0), timestamp(0.0) {}

  Cell(const std::vector<double> &coords, double density = 0.0,
       double timestamp = 0.0)
      : coordinates(coords), density(density), timestamp(timestamp) {}

  void addPoint(const Point &point) {
    density += 1;
    timestamp = point.timestamp;
  }

  double calcDistance(const Cell &cell) const {
    double dist = 0.0;
    for (int i = 0; i < coordinates.size(); i++) {
      dist += (coordinates[i] - cell.coordinates[i]) *
              (coordinates[i] - cell.coordinates[i]);
    }
    return sqrt(dist);
  }
};

class DStream {
public:
  DStream(int dimensions) : dimensions(dimensions) {}

  void insert(const Point &point) {
    // Create cell coordinates for the point
    std::vector<double> cellCoordinates(dimensions);
    for (int i = 0; i < dimensions; ++i) {
      cellCoordinates[i] = std::floor(point.coordinates[i] / CELL_SIZE);
    }

    // Generate the cell key
    std::string cellKey = createCellKey(cellCoordinates);

    // Insert the cell or update the existing cell
    auto it = grid.find(cellKey);
    if (it != grid.end()) {
      it->second.addPoint(point);
    } else {
      Cell cell(cellCoordinates);
      cell.addPoint(point); // Initialize the cell density and timestamp
      grid.emplace(cellKey, std::move(cell));
    }

    // Remove outdated cells
    for (auto it = grid.begin(); it != grid.end();) {
      if (point.timestamp - it->second.timestamp > TIME_WINDOW) {
        it = grid.erase(it);
      } else {
        ++it;
      }
    }
  }

  void cluster(const std::vector<Point> &points) {
    for (const auto &point : points) {
      insert(point);
    }
  }

private:
  int dimensions;
  std::unordered_map<std::string, Cell> grid;
  const double TIME_WINDOW = 1000.0;

  std::string createCellKey(const std::vector<double> &coordinates) const {
    std::string key;
    for (const auto &coord : coordinates) {
      key += std::to_string(static_cast<int>(coord)) + "_";
    }
    return key;
  }
};

#endif // DSTREAM_HPP
