cmake_minimum_required(VERSION 3.17)
project(PDSC)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(pdsc
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
