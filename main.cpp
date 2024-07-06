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

#include <cassert>
#include <chrono>
#include <cstdlib>
#include <iostream>
#include <vector>

using namespace std;
using namespace std::chrono_literals;

void run(const string &name, const Dataset &dataset, Algorithm &algo) {
  auto start = chrono::high_resolution_clock::now();
  for (int i = 0; i < dataset.num_points; i += BATCH_SIZE) {
    int end = min(i + BATCH_SIZE, dataset.num_points);
    algo.cluster(vector<Point>(dataset.points.begin() + i,
                               dataset.points.begin() + end));
    cout << "Progress: [" << (i + BATCH_SIZE) << " / " << dataset.num_points
         << "]\r";
  }
  cout << endl;
  auto end = chrono::high_resolution_clock::now();
  auto elapsed = chrono::duration_cast<chrono::milliseconds>(end - start);
  cout << "Execution time: " << elapsed.count() << " ms" << endl;
  auto centers = algo.output_centers();
  cout << "Number of clusters: " << centers.size() << endl;
  // save centers to file
  ofstream out(name + ".centers");
  for (const auto &center : centers) {
    for (int i = 0; i < center.features.size(); i++) {
      out << center.features[i] << " ";
    }
    out << endl;
  }
  out.close();
  if (centers.size() > 0 && centers.size() <= 1000) {
    auto purity = evaluate_purity(dataset.points,
                                  group_by_centers(dataset.points, centers),
                                  dataset.num_true_clusters, centers.size());
    cout << "Purity: " << purity << endl;
  } else {
    cout << "Purity: N/A, please check code correctness" << endl;
  }
}

int main(int argc, char *argv[]) {
  cout << "Starting benchmarks..." << endl;

  Dataset dataset;
  if (argc < 2) {
    cout << "Usage: " << argv[0] << " <dataset>" << endl;
    cout << "Using random generated dataset, results may vary." << endl;
    dataset.gen(NUM_POINTS, DIMENSIONS);
  } else {
    dataset.load(argv[1]);
    // dataset.limit(2000);
  }

  cout << dataset << endl;

  // Benchmark BIRCH
  cout << "==============================" << endl;
  cout << "Running BIRCH ..." << endl;
  BIRCH birch(dataset.dim);
  run("birch", dataset, birch);

  // Benchmark CluStream
  cout << "==============================" << endl;
  cout << "Running CluStream ..." << endl;
  CluStream clustream(dataset.dim);
  run("clustream", dataset, clustream);

  // Benchmark EDMStream
  // cout << "==============================" << endl;
  // cout << "Running EDMStream ..." << endl;
  // EDMStream edm(dataset.dim);
  // run("edmstream", dataset, edm);

  // Benchmark DStream
  cout << "==============================" << endl;
  cout << "Running DStream ..." << endl;
  DStream dstream(dataset.dim);
  run("dstream", dataset, dstream);

  // Benchmark DenStream
  cout << "==============================" << endl;
  cout << "Running DenStream ..." << endl;
  DenStream denstream(dataset.dim);
  run("denstream", dataset, denstream);

  // Benchmark SLKMeans
  cout << "==============================" << endl;
  cout << "Running SLKMeans ..." << endl;
  SLKMeans slkmeans(dataset.dim, dataset.num_true_clusters);
  run("slkmeans", dataset, slkmeans);

  return 0;
}
