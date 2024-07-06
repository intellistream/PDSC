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

#ifndef PDSC_COMMON_HPP
#define PDSC_COMMON_HPP

#include <cmath>
#include <memory>
#include <vector>

#ifndef NDEBUG
#define NDEBUG
#endif

using u32 = uint32_t;
using u64 = uint64_t;
using f32 = float;
using f64 = double;

inline u32 DIMENSIONS = 10;    // Number of dimensions in the data
inline u64 NUM_POINTS = 50000; // Total number of data points
const u64 BATCH_SIZE = 1000;   // Tunable batch size

// Algorithm specific parameters.
const double TIME_WINDOW = 1000.0; // used by CluStream and EDMStream
const int K = 5;                   // used by SLKMeans.

#endif // PDSC_COMMON_HPP
