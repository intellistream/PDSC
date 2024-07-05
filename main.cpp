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
#include "evaluation.hpp"
#include "point.hpp"
#include "slkmeans.hpp"

#include <chrono>
#include <cstdlib>
#include <iostream>
#include <vector>

// Similar placeholder classes for SLKMeans, EDMStream

using namespace std;

int main(int argc, char *argv[]) {
  cout << "Starting benchmarks..." << endl;

  Dataset dataset;
  if (argc < 2) {
    cout << "Usage: " << argv[0] << " <dataset>" << endl;
    cout << "Using random generated dataset, results may vary." << endl;
    dataset.gen(NUM_POINTS, DIMENSIONS);
  } else {
    dataset.load(argv[1]);
  }

  cout << dataset << endl;

  // Benchmark BIRCH
  cout << "Running BIRCH ..." << endl;
  BIRCH birch(dataset.dim);
  auto start = chrono::high_resolution_clock::now();
  for (int i = 0; i < NUM_POINTS; i += BATCH_SIZE) {
    int end = min(i + BATCH_SIZE, NUM_POINTS);
    vector<Point> batch(dataset.points.begin() + i,
                        dataset.points.begin() + end);
    birch.cluster(batch);
  }
  auto end = chrono::high_resolution_clock::now();
  chrono::duration<double> elapsed = end - start;
  cout << "BIRCH execution time with batch size " << BATCH_SIZE << ": "
       << elapsed.count() << " seconds" << endl;

  auto centers = birch.output_centers();
  cout << "Number of clusters: " << centers.size() << endl;
  auto purity =
      evaluate_purity(dataset.points, group_by_centers(dataset.points, centers),
                      dataset.num_true_clusters);
  cout << "Purity: " << purity << endl;

  // Benchmark CluStream
  cout << "Running CluStream ..." << endl;
  // Create CluStream instance
  CluStream cluStream(dataset.dim);
  start = chrono::high_resolution_clock::now();
  for (int i = 0; i < NUM_POINTS; i += BATCH_SIZE) {
    int end = min(i + BATCH_SIZE, NUM_POINTS);
    vector<Point> batch(dataset.points.begin() + i,
                        dataset.points.begin() + end);
    cluStream.cluster(batch);
  }
  end = chrono::high_resolution_clock::now();
  elapsed = end - start;
  cout << "CluStream execution time with batch size " << BATCH_SIZE << ": "
       << elapsed.count() << " seconds" << endl;

  // Benchmark EDMStream
  cout << "Running EDMStream ..." << endl;
  // Create EDMStream instance
  EDMStream edmStream(dataset.dim);
  start = chrono::high_resolution_clock::now();
  for (int i = 0; i < NUM_POINTS; i += BATCH_SIZE) {
    int end = min(i + BATCH_SIZE, NUM_POINTS);
    vector<Point> batch(dataset.points.begin() + i,
                        dataset.points.begin() + end);
    edmStream.cluster(batch);
  }
  end = chrono::high_resolution_clock::now();
  elapsed = end - start;
  cout << "EDMStream execution time with batch size " << BATCH_SIZE << ": "
       << elapsed.count() << " seconds" << endl;

  // Benchmark DStream
  cout << "Running DStream ..." << endl;
  // Create DStream instance
  DStream dStream(dataset.dim);

  start = chrono::high_resolution_clock::now();
  for (int i = 0; i < NUM_POINTS; i += BATCH_SIZE) {
    int end = min(i + BATCH_SIZE, NUM_POINTS);
    vector<Point> batch(dataset.points.begin() + i,
                        dataset.points.begin() + end);
    dStream.cluster(batch);
  }
  end = chrono::high_resolution_clock::now();
  elapsed = end - start;
  cout << "DStream execution time with batch size " << BATCH_SIZE << ": "
       << elapsed.count() << " seconds" << endl;

  // Benchmark DenStream
  cout << "Running DenStream ..." << endl;
  // Create DStream instance
  DenStream denStreamm(dataset.dim);

  start = chrono::high_resolution_clock::now();
  for (int i = 0; i < NUM_POINTS; i += BATCH_SIZE) {
    int end = min(i + BATCH_SIZE, NUM_POINTS);
    vector<Point> batch(dataset.points.begin() + i,
                        dataset.points.begin() + end);
    denStreamm.cluster(batch);
  }
  end = chrono::high_resolution_clock::now();
  elapsed = end - start;
  cout << "DenStream execution time with batch size " << BATCH_SIZE << ": "
       << elapsed.count() << " seconds" << endl;

  // Benchmark SLKMeans
  cout << "Running SLKMeans ..." << endl;
  // Create SLKMeans instance
  SLKMeans slkmeans(dataset.dim, K);

  start = chrono::high_resolution_clock::now();
  for (int i = 0; i < NUM_POINTS; i += BATCH_SIZE) {
    int end = min(i + BATCH_SIZE, NUM_POINTS);
    vector<Point> batch(dataset.points.begin() + i,
                        dataset.points.begin() + end);
    slkmeans.cluster(batch);
  }
  end = chrono::high_resolution_clock::now();
  elapsed = end - start;
  cout << "SLKMeans execution time with batch size " << BATCH_SIZE << ": "
       << elapsed.count() << " seconds" << endl;

  return 0;
}
