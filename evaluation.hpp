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

#ifndef PDSC_EVALUATION_HPP
#define PDSC_EVALUATION_HPP

#include "point.hpp"

#include <map>

std::vector<int> group_by_centers(const std::vector<Point> &points,
                                  const std::vector<Point> &centers) {
  std::vector<int> predicts(points.size());
  for (int i = 0; i < points.size(); i++) {
    double min_dist = std::numeric_limits<double>::max();
    for (int j = 0; j < centers.size(); j++) {
      double dist = points[i].l2_dist(centers[j]);
      if (dist < min_dist) {
        min_dist = dist;
        predicts[i] = j;
      }
    }
  }
  return predicts;
}

double evaluate_purity(const std::vector<Point> &points,
                       const std::vector<int> &predicts, u32 num_true_clusters,
                       u32 num_pred_clusters) {
  std::vector<std::vector<int>> cluster_to_label(
      std::max(num_true_clusters, num_pred_clusters));

  for (int i = 0; i < points.size(); i++) {
    cluster_to_label[predicts[i]].push_back(points[i].true_clu_id);
  }

  double purity = 0.0;
  for (int i = 0; i < num_true_clusters; i++) {
    std::map<int, int> label_count;
    for (int label : cluster_to_label[i]) {
      label_count[label]++;
    }
    int max_count = 0;
    for (const auto &pair : label_count) {
      max_count = std::max(max_count, pair.second);
    }
    purity += max_count;
  }
  return purity / points.size();
}

#endif