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

#ifndef PDSC_CLUSTREAM_HPP
#define PDSC_CLUSTREAM_HPP

#include "common.hpp"

const int MAX_MICRO_CLUSTERS = 100;

struct MicroCluster {
  std::vector<double> linear_sum;
  std::vector<double> squared_sum;
  int n; // Number of points
  double last_update_time;

  MicroCluster(int dimensions)
      : linear_sum(dimensions, 0.0), squared_sum(dimensions, 0.0), n(0),
        last_update_time(0.0) {}

  void addPoint(const Point &point) {
    n++;
    for (int i = 0; i < point.features.size(); i++) {
      linear_sum[i] += point.features[i];
      squared_sum[i] += point.features[i] * point.features[i];
    }
    last_update_time = point.timestamp;
  }

  double calcDistance(const Point &point) const {
    double dist = 0.0;
    for (int i = 0; i < point.features.size(); i++) {
      double mean = linear_sum[i] / n;
      dist += (point.features[i] - mean) * (point.features[i] - mean);
    }
    return sqrt(dist);
  }

  double calcRadius() const {
    double radius = 0.0;
    for (int i = 0; i < linear_sum.size(); i++) {
      double mean = linear_sum[i] / n;
      double variance = (squared_sum[i] / n) - (mean * mean);
      radius += variance;
    }
    return sqrt(radius);
  }

  bool isWithinTimeWindow(double current_time) const {
    return (current_time - last_update_time) <= TIME_WINDOW;
  }
};

class CluStream {
public:
  CluStream(int dimensions) : dimensions(dimensions) {}

  void insert(const Point &point) {
    if (micro_clusters.empty()) {
      // Create the first micro-cluster
      MicroCluster newMC(dimensions);
      newMC.addPoint(point);
      micro_clusters.push_back(newMC);
      return;
    }

    // Find the closest micro-cluster
    int closestIndex = -1;
    double closestDist = std::numeric_limits<double>::max();
    for (int i = 0; i < micro_clusters.size(); i++) {
      if (micro_clusters[i].isWithinTimeWindow(point.timestamp)) {
        double dist = micro_clusters[i].calcDistance(point);
        if (dist < closestDist) {
          closestDist = dist;
          closestIndex = i;
        }
      }
    }

    // Add the point to the closest micro-cluster
    if (closestDist < threshold) {
      micro_clusters[closestIndex].addPoint(point);
    } else {
      // Create a new micro-cluster
      MicroCluster newMC(dimensions);
      newMC.addPoint(point);
      micro_clusters.push_back(newMC);

      // Remove the oldest micro-cluster if necessary
      if (micro_clusters.size() > MAX_MICRO_CLUSTERS) {
        removeOldestMicroCluster(point.timestamp);
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
  std::vector<MicroCluster> micro_clusters;
  const double threshold = 1.0; // Threshold for micro-cluster distance

  void removeOldestMicroCluster(double current_time) {
    int oldestIndex = -1;
    double oldestTime = std::numeric_limits<double>::max();
    for (int i = 0; i < micro_clusters.size(); i++) {
      if (micro_clusters[i].last_update_time < oldestTime) {
        oldestTime = micro_clusters[i].last_update_time;
        oldestIndex = i;
      }
    }
    if (oldestIndex != -1) {
      micro_clusters.erase(micro_clusters.begin() + oldestIndex);
    }
  }
};

#endif // PDSC_CLUSTREAM_HPP
