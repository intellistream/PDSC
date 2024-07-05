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

#ifndef PDSC_SLKMEANS_HPP
#define PDSC_SLKMEANS_HPP

#include "point.hpp"
#include "common.hpp"
#include <vector>
#include <cmath>
#include <limits>
#include <algorithm>
#include <random>
#include <deque>

const int WINDOW_SIZE = 1000;

class SLKMeans {
public:
    SLKMeans(int dimensions, int k) : dimensions(dimensions), k(k) {
        centroids.resize(k, Point(dimensions));
    }

    void insert(const Point& point) {
        window.push_back(point);
        if (window.size() > WINDOW_SIZE) {
            window.pop_front();
        }
        if (window.size() >= k) {
            runKMeans();
        }
    }

    void cluster(const std::vector<Point>& points) {
        for (const auto& point : points) {
            insert(point);
        }
    }

private:
    int dimensions;
    int k;
    std::vector<Point> centroids;
    std::deque<Point> window;

    void initializeCentroids() {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(0, window.size() - 1);
        for (int i = 0; i < k; ++i) {
            centroids[i] = window[dis(gen)];
        }
    }

    void runKMeans() {
        // Initialize centroids if the window has enough points
        if (window.size() < k) return;
        initializeCentroids();

        bool converged = false;
        std::vector<int> assignments(window.size());

        while (!converged) {
            // Step 1: Assign points to the nearest centroid
            converged = true;
            for (size_t i = 0; i < window.size(); ++i) {
                double minDist = std::numeric_limits<double>::max();
                int bestCluster = -1;
                for (int j = 0; j < k; ++j) {
                    double dist = calcDistance(window[i], centroids[j]);
                    if (dist < minDist) {
                        minDist = dist;
                        bestCluster = j;
                    }
                }
                if (assignments[i] != bestCluster) {
                    assignments[i] = bestCluster;
                    converged = false;
                }
            }

            // Step 2: Update centroids
            std::vector<Point> newCentroids(k, Point(dimensions));
            std::vector<int> counts(k, 0);
            for (size_t i = 0; i < window.size(); ++i) {
                int cluster = assignments[i];
                for (int d = 0; d < dimensions; ++d) {
                    newCentroids[cluster].features[d] += window[i].features[d];
                }
                counts[cluster]++;
            }
            for (int j = 0; j < k; ++j) {
                if (counts[j] > 0) {
                    for (int d = 0; d < dimensions; ++d) {
                        newCentroids[j].features[d] /= counts[j];
                    }
                }
            }
            centroids = newCentroids;
        }
    }

    double calcDistance(const Point& a, const Point& b) const {
        double dist = 0.0;
        for (int i = 0; i < dimensions; ++i) {
            dist += (a.features[i] - b.features[i]) * (a.features[i] - b.features[i]);
        }
        return sqrt(dist);
    }
};
#endif //PDSC_SLKMEANS_HPP
