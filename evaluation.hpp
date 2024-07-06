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
  // for (int i = 0; i < points.size(); i++) {
  //   std::cout << predicts[i] << " ";
  // }
  // std::cout << std::endl;
  std::vector<std::vector<int>> confusion_matrix(
      num_true_clusters + 1, std::vector<int>(num_pred_clusters));
  for (int i = 0; i < points.size(); i++) {
    // std::cout << predicts[i] << " ";
    confusion_matrix[points[i].true_clu_id][predicts[i]]++;
  }
  // std::cout << "Confusion Matrix:" << std::endl;
  // for (int i = 1; i <= num_true_clusters; i++) {
  //   for (int j = 0; j < num_pred_clusters; j++) {
  //     std::cout << confusion_matrix[i][j] << " ";
  //   }
  //   std::cout << std::endl;
  // }
  double total = 0.0;
  for (int i = 1; i <= num_true_clusters; i++) {
    double max = 0.0;
    for (int j = 0; j < num_pred_clusters; j++) {
      if (confusion_matrix[i][j] > max) {
        max = confusion_matrix[i][j];
      }
    }
    total += max;
  }
  // std::cout << "Total Purity: " << total << std::endl;
  return total / (double)points.size();
}

#endif