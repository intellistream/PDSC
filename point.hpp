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

#ifndef PDSC_POINT_HPP
#define PDSC_POINT_HPP

#include "common.hpp"

#include <cstdlib>
#include <ctime>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

struct Point {
  std::vector<f64> features;
  u64 timestamp, true_clu_id;

  Point(int dim = 0) : features(dim, 0.0), timestamp(0) {}
  Point(std::vector<double> coords, double timestamp = 0.0)
      : features(coords), timestamp(timestamp) {}
  Point &operator/=(double scalar) {
    for (int i = 0; i < features.size(); i++) {
      features[i] /= scalar;
    }
    return *this;
  }
  double l2_dist(const Point &other) const {
    double dist = 0.0;
    for (int i = 0; i < features.size(); i++) {
      dist +=
          (features[i] - other.features[i]) * (features[i] - other.features[i]);
    }
    return sqrt(dist);
  }
};

struct Dataset {
  std::string name;
  u64 num_points = 0;
  u32 dim = 0, num_true_clusters = 0;
  std::vector<Point> points;
  void gen(u64 num_points, u32 dim) {
    this->num_points = num_points;
    this->dim = dim;
    name = "random";
    points.resize(num_points, Point(dim));
    srand(static_cast<unsigned>(time(0)));
    for (u64 i = 0; i < num_points; ++i) {
      for (u32 j = 0; j < dim; ++j) {
        points[i].features[j] = static_cast<double>(rand()) / RAND_MAX * 100;
      }
      points[i].timestamp = i; // Example timestamp, could be any sequence
    }
  }
  void load(const std::string &filename) {
    std::ifstream file(filename);
    if (file.is_open()) {
      {
        // read header line: "# dataset_name num_points dim num_true_clusters"
        std::string header;
        std::getline(file, header);
        std::stringstream ss(header);
        std::string token;
        ss >> token >> name >> num_points >> dim >> num_true_clusters;
      }
      for (int i = 0; i < num_points; ++i) {
        Point point(dim);
        std::string line;
        std::getline(file, line);
        std::stringstream ss(line);
        std::string token;
        for (int j = 0; j < dim; ++j) {
          std::getline(ss, token, ',');
          point.features[j] = std::stod(token);
        }
        std::getline(ss, token, ',');
        point.true_clu_id = std::stoull(token);
        point.timestamp = i + 1; // Example timestamp, could be any sequence
        points.push_back(point);
      }
      file.close();
    }
  }
  void limit(u64 num_points) {
    if (num_points && num_points < points.size()) {
      points.resize((size_t)num_points);
      this->num_points = num_points;
    }
  }
};

std::ostream &operator<<(std::ostream &os, const Point &point);
std::ostream &operator<<(std::ostream &os, const Dataset &dataset);

#endif // PDSC_POINT_HPP
