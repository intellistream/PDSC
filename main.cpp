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

#include "birch.hpp"
#include "clustream.hpp"
#include "common.hpp"
#include "denstream.hpp"
#include "dstream.hpp"
#include "edmstream.hpp"
#include "point.hpp"
#include "slkmeans.hpp"

#include <chrono>
#include <cstdlib>
#include <iostream>
#include <vector>

// Similar placeholder classes for SLKMeans, EDMStream

int main() {
  std::cout << "Starting benchmarks..." << std::endl;

  // Generate random data points
  std::vector<Point> dataPoints = generateDataPoints(NUM_POINTS, DIMENSIONS);

  // Benchmark BIRCH
  std::cout << "Running BIRCH ..." << std::endl;
  BIRCH birch(DIMENSIONS);
  auto start = std::chrono::high_resolution_clock::now();
  for (int i = 0; i < NUM_POINTS; i += BATCH_SIZE) {
    int end = std::min(i + BATCH_SIZE, NUM_POINTS);
    std::vector<Point> batch(dataPoints.begin() + i, dataPoints.begin() + end);
    birch.cluster(batch);
  }
  auto end = std::chrono::high_resolution_clock::now();
  std::chrono::duration<double> elapsed = end - start;
  std::cout << "BIRCH execution time with batch size " << BATCH_SIZE << ": "
            << elapsed.count() << " seconds" << std::endl;

  // Benchmark CluStream
  std::cout << "Running CluStream ..." << std::endl;
  // Create CluStream instance
  CluStream cluStream(DIMENSIONS);
  start = std::chrono::high_resolution_clock::now();
  for (int i = 0; i < NUM_POINTS; i += BATCH_SIZE) {
    int end = std::min(i + BATCH_SIZE, NUM_POINTS);
    std::vector<Point> batch(dataPoints.begin() + i, dataPoints.begin() + end);
    cluStream.cluster(batch);
  }
  end = std::chrono::high_resolution_clock::now();
  elapsed = end - start;
  std::cout << "CluStream execution time with batch size " << BATCH_SIZE << ": "
            << elapsed.count() << " seconds" << std::endl;

  // Benchmark EDMStream
  std::cout << "Running EDMStream ..." << std::endl;
  // Create EDMStream instance
  EDMStream edmStream(DIMENSIONS);
  start = std::chrono::high_resolution_clock::now();
  for (int i = 0; i < NUM_POINTS; i += BATCH_SIZE) {
    int end = std::min(i + BATCH_SIZE, NUM_POINTS);
    std::vector<Point> batch(dataPoints.begin() + i, dataPoints.begin() + end);
    edmStream.cluster(batch);
  }
  end = std::chrono::high_resolution_clock::now();
  elapsed = end - start;
  std::cout << "EDMStream execution time with batch size " << BATCH_SIZE << ": "
            << elapsed.count() << " seconds" << std::endl;

  // Benchmark DStream
  std::cout << "Running DStream ..." << std::endl;
  // Create DStream instance
  DStream dStream(DIMENSIONS);

  start = std::chrono::high_resolution_clock::now();
  for (int i = 0; i < NUM_POINTS; i += BATCH_SIZE) {
    int end = std::min(i + BATCH_SIZE, NUM_POINTS);
    std::vector<Point> batch(dataPoints.begin() + i, dataPoints.begin() + end);
    dStream.cluster(batch);
  }
  end = std::chrono::high_resolution_clock::now();
  elapsed = end - start;
  std::cout << "DStream execution time with batch size " << BATCH_SIZE << ": "
            << elapsed.count() << " seconds" << std::endl;

  // Benchmark DenStream
  std::cout << "Running DenStream ..." << std::endl;
  // Create DStream instance
  DenStream denStreamm(DIMENSIONS);

  start = std::chrono::high_resolution_clock::now();
  for (int i = 0; i < NUM_POINTS; i += BATCH_SIZE) {
    int end = std::min(i + BATCH_SIZE, NUM_POINTS);
    std::vector<Point> batch(dataPoints.begin() + i, dataPoints.begin() + end);
    denStreamm.cluster(batch);
  }
  end = std::chrono::high_resolution_clock::now();
  elapsed = end - start;
  std::cout << "DenStream execution time with batch size " << BATCH_SIZE << ": "
            << elapsed.count() << " seconds" << std::endl;

  // Benchmark SLKMeans
  std::cout << "Running SLKMeans ..." << std::endl;
  // Create SLKMeans instance
  SLKMeans slkmeans(DIMENSIONS, K);

  start = std::chrono::high_resolution_clock::now();
  for (int i = 0; i < NUM_POINTS; i += BATCH_SIZE) {
    int end = std::min(i + BATCH_SIZE, NUM_POINTS);
    std::vector<Point> batch(dataPoints.begin() + i, dataPoints.begin() + end);
    slkmeans.cluster(batch);
  }
  end = std::chrono::high_resolution_clock::now();
  elapsed = end - start;
  std::cout << "SLKMeans execution time with batch size " << BATCH_SIZE << ": "
            << elapsed.count() << " seconds" << std::endl;

  return 0;
}
