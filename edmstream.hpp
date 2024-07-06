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

#ifndef PDSC_EDMSTREAM_HPP
#define PDSC_EDMSTREAM_HPP

#include "algorithm.hpp"
#include "common.hpp"

const int MAX_CLUSTERS = 100;
const double DECAY_RATE = 0.01;
const double DENSITY_THRESHOLD = 0.1;

struct ClusterCell {
  Point seed;
  double density = 0.0;
  const double dependent_distance = 5000.0;
  double creation_time = 0.0;

  ClusterCell(int dimensions) : seed(dimensions) {}

  void addPoint(const Point &point) {
    for (int i = 0; i < point.features.size(); i++) {
      seed.features[i] += point.features[i];
    }
    density += 1.0; // Simplified for illustration
    seed.timestamp = point.timestamp;
  }

  void decayDensity(double current_time) {
    density *= exp(-DECAY_RATE * (current_time - seed.timestamp));
  }

  double calcDistance(const Point &point) const {
    double dist = 0.0;
    for (int i = 0; i < point.features.size(); i++) {
      dist += (point.features[i] - seed.features[i]) *
              (point.features[i] - seed.features[i]);
    }
    return sqrt(dist);
  }
};

class DPNode {
public:
  ClusterCell cell;
  std::vector<DPNode *> children;

  DPNode(const ClusterCell &c) : cell(c) {}
};

class DPTree {
public:
  DPNode *root;

  DPTree() : root(nullptr) {}

  void addClusterCell(const ClusterCell &cell) {
    if (!root) {
      root = new DPNode(cell);
    } else {
      addClusterCellRecursive(root, cell);
    }
  }

  void decayClusters(double current_time) {
    decayClustersRecursive(root, current_time);
  }

private:
  void addClusterCellRecursive(DPNode *node, const ClusterCell &cell) {
    double dist = node->cell.calcDistance(cell.seed);
    if (dist < node->cell.dependent_distance) {
      DPNode *child = new DPNode(cell);
      node->children.push_back(child);
    } else {
      for (auto &child : node->children) {
        addClusterCellRecursive(child, cell);
      }
    }
  }

  void decayClustersRecursive(DPNode *node, double current_time) {
    if (!node)
      return;
    node->cell.decayDensity(current_time);
    for (auto &child : node->children) {
      decayClustersRecursive(child, current_time);
    }
  }
};

class EDMStream : public Algorithm {
public:
  EDMStream(int dimensions) : dimensions(dimensions), dp_tree(new DPTree()) {}

  void insert(const Point &point) {
    // Decay existing clusters
    dp_tree->decayClusters(point.timestamp);

    // Create a new cluster cell
    ClusterCell newCell(dimensions);
    newCell.addPoint(point);
    newCell.creation_time = point.timestamp;

    // Insert into DP-Tree
    dp_tree->addClusterCell(newCell);
  }

  void cluster(const std::vector<Point> &points) {
    for (const auto &point : points) {
      insert(point);
    }
  }

  std::vector<Point> output_centers() {
    std::vector<Point> centers;
    output_centers_recursive(dp_tree->root, centers);
    return centers;
  }

  void output_centers_recursive(DPNode *node, std::vector<Point> &centers) {
    if (!node)
      return;
    if (node->cell.density > DENSITY_THRESHOLD) {
      // std::cout << "density: " << node->cell.density << std::endl;
      Point center(node->cell.seed.features);
      center /= node->cell.density;
      centers.push_back(center);
    }
    // std::cout << "children size: " << node->children.size() << std::endl;
    for (auto &child : node->children) {
      output_centers_recursive(child, centers);
    }
  }

private:
  int dimensions;
  DPTree *dp_tree;
};
#endif // PDSC_EDMSTREAM_HPP
