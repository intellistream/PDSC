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

#ifndef PDSC_BIRCH_HPP
#define PDSC_BIRCH_HPP

#include "common.hpp"
#include "point.hpp"

#include <limits>

const int BRANCHING_FACTOR = 50;
const int MAX_ENTRIES = 100;

struct ClusteringFeature {
  std::vector<double> linear_sum;
  std::vector<double> squared_sum;
  int n; // Number of points

  ClusteringFeature(int dimensions)
      : linear_sum(dimensions, 0.0), squared_sum(dimensions, 0.0), n(0) {}

  void addPoint(const Point &point) {
    n++;
    for (int i = 0; i < point.coordinates.size(); i++) {
      linear_sum[i] += point.coordinates[i];
      squared_sum[i] += point.coordinates[i] * point.coordinates[i];
    }
  }

  void addCF(const ClusteringFeature &cf) {
    n += cf.n;
    for (int i = 0; i < linear_sum.size(); i++) {
      linear_sum[i] += cf.linear_sum[i];
      squared_sum[i] += cf.squared_sum[i];
    }
  }

  double calcDistance(const Point &point) const {
    double dist = 0.0;
    for (int i = 0; i < point.coordinates.size(); i++) {
      double mean = linear_sum[i] / n;
      dist += (point.coordinates[i] - mean) * (point.coordinates[i] - mean);
    }
    return sqrt(dist);
  }
};

struct CFNode;
using CFNodePtr = std::shared_ptr<CFNode>;

struct CFNode {
  bool isLeaf;
  std::vector<ClusteringFeature> entries;
  std::vector<CFNodePtr> children;

  CFNode(bool leaf) : isLeaf(leaf) {
    entries.reserve(MAX_ENTRIES);
    children.reserve(BRANCHING_FACTOR);
  }
};

class BIRCH {
public:
  BIRCH(int dimensions) : dimensions(dimensions), root(new CFNode(true)) {}

  void insert(const Point &point) {
    ClusteringFeature cf(dimensions);
    insertCF(root, cf, point);
  }

  void cluster(const std::vector<Point> &points) {
    for (const auto &point : points) {
      insert(point);
    }
  }

private:
  int dimensions;
  CFNodePtr root;

  void insertCF(CFNodePtr &node, ClusteringFeature &cf, const Point &point) {
    if (node->isLeaf) {
      // Find the closest CF entry
      int closestIndex = -1;
      double closestDist = std::numeric_limits<double>::max();
      for (int i = 0; i < node->entries.size(); i++) {
        double dist = node->entries[i].calcDistance(point);
        if (dist < closestDist) {
          closestDist = dist;
          closestIndex = i;
        }
      }

      // Add the point to the closest CF entry
      if (closestDist < threshold) {
        node->entries[closestIndex].addPoint(point);
      } else {
        // Create a new CF entry
        ClusteringFeature newCF(dimensions);
        newCF.addPoint(point);
        node->entries.push_back(newCF);

        // Split the node if necessary
        if (node->entries.size() > MAX_ENTRIES) {
          splitNode(node);
        }
      }
    } else {
      // Find the closest child node
      int closestIndex = -1;
      double closestDist = std::numeric_limits<double>::max();
      for (int i = 0; i < node->children.size(); i++) {
        double dist = node->children[i]->entries[0].calcDistance(point);
        if (dist < closestDist) {
          closestDist = dist;
          closestIndex = i;
        }
      }

      // Recursively insert the CF into the child node
      insertCF(node->children[closestIndex], node->entries[closestIndex],
               point);
    }
  }

  void splitNode(CFNodePtr &node) {
    // Split the node into two nodes
    CFNodePtr newNode(new CFNode(node->isLeaf));
    for (int i = 0; i < node->entries.size() / 2; i++) {
      newNode->entries.push_back(node->entries.back());
      node->entries.pop_back();
    }

    if (!node->isLeaf) {
      for (int i = 0; i < node->children.size() / 2; i++) {
        newNode->children.push_back(node->children.back());
        node->children.pop_back();
      }
    }

    // Add the new node to the parent
    if (node == root) {
      CFNodePtr newRoot(new CFNode(false));
      newRoot->entries.push_back(ClusteringFeature(dimensions));
      newRoot->children.push_back(root);
      newRoot->children.push_back(newNode);
      root = newRoot;
    } else {
      // Add the new node to the parent's children
      CFNodePtr parent = findParent(root, node);
      parent->children.push_back(newNode);
      parent->entries.push_back(ClusteringFeature(dimensions));
    }
  }

  CFNodePtr findParent(CFNodePtr node, CFNodePtr target) {
    if (node->isLeaf) {
      return nullptr;
    }

    for (auto &child : node->children) {
      if (child == target) {
        return node;
      } else {
        CFNodePtr parent = findParent(child, target);
        if (parent != nullptr) {
          return parent;
        }
      }
    }
    return nullptr;
  }

  const double threshold = 1.0; // Threshold for CF entry distance
};

#endif // PDSC_BIRCH_HPP
