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

#ifndef DENSTREAM_HPP
#define DENSTREAM_HPP

#include "algorithm.hpp"

#include <cmath>
#include <deque>
#include <limits>
#include <vector>

const double EPSILON = 1.0;
const int MIN_POINTS = 5;

struct DenStreamMicroCluster {
  std::vector<double> linear_sum;
  std::vector<double> squared_sum;
  int n;
  double weight;
  double creation_time;

  DenStreamMicroCluster(int dimensions)
      : linear_sum(dimensions, 0.0), squared_sum(dimensions, 0.0), n(0),
        weight(0.0), creation_time(0.0) {}

  void addPoint(const Point &point, double timestamp) {
    n++;
    weight += 1;
    for (int i = 0; i < point.features.size(); i++) {
      linear_sum[i] += point.features[i];
      squared_sum[i] += point.features[i] * point.features[i];
    }
    creation_time = timestamp;
  }

  double calcDistance(const Point &point) const {
    double dist = 0.0;
    for (int i = 0; i < point.features.size(); i++) {
      double mean = linear_sum[i] / n;
      dist += (point.features[i] - mean) * (point.features[i] - mean);
    }
    return sqrt(dist);
  }
};

class DenStream : public Algorithm {
public:
  DenStream(int dimensions) : dimensions(dimensions) {}

  void insert(const Point &point) {
    double timestamp = point.timestamp;

    // Remove outdated micro-clusters
    for (auto it = clusters.begin(); it != clusters.end();) {
      if (timestamp - it->creation_time > TIME_WINDOW) {
        it = clusters.erase(it);
      } else {
        ++it;
      }
    }

    // Find the closest micro-cluster
    int closestIndex = -1;
    double closestDist = std::numeric_limits<double>::max();
    for (int i = 0; i < clusters.size(); i++) {
      double dist = clusters[i].calcDistance(point);
      if (dist < closestDist) {
        closestDist = dist;
        closestIndex = i;
      }
    }

    // Add the point to the closest micro-cluster
    if (closestDist < EPSILON) {
      clusters[closestIndex].addPoint(point, timestamp);
    } else {
      // Create a new micro-cluster
      DenStreamMicroCluster newCluster(dimensions);
      newCluster.addPoint(point, timestamp);
      clusters.push_back(newCluster);
    }
  }

  void cluster(const std::vector<Point> &points) {
    for (const auto &point : points) {
      insert(point);
    }
  }

  std::vector<Point> output_centers() {
    std::vector<Point> centers;
    for (const auto &cluster : clusters) {
      if (cluster.weight >= MIN_POINTS) {
        Point center(cluster.linear_sum);
        center /= cluster.n;
        centers.push_back(center);
      }
    }
    return centers;
  }

private:
  int dimensions;
  std::deque<DenStreamMicroCluster> clusters;
  const double TIME_WINDOW = 1000.0;
};

#endif // DENSTREAM_HPP
