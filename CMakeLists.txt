# cmake_minimum_required(VERSION <specify CMake version here>)
project(PDSC)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(PDSC
        main.cpp
        main.h
        BIRCH.hpp
        common.hpp
        common.cpp
        CluStream.hpp
        Point.hpp
        Point.cpp
        EDMStream.hpp
        SLKMeans.hpp
        DenStream.hpp
        DStream.hpp)
