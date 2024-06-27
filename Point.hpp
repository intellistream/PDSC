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

#ifndef PDSC_POINT_H
#define PDSC_POINT_H

#include <vector>

struct Point {
    std::vector<double> coordinates;
    double timestamp;

    Point(int dimensions) : coordinates(dimensions, 0.0), timestamp(0.0) {}
};

std::vector<Point> generateDataPoints(int numPoints, int dimensions);

#endif //PDSC_POINT_H
