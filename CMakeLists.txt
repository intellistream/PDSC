cmake_minimum_required(VERSION 3.17)
project(PDSC)

set(CMAKE_CXX_STANDARD 17)

include_directories(.)

add_executable(pdsc
        main.cpp
        birch.hpp
        common.hpp
        evaluation.hpp
        clustream.hpp
        point.hpp
        point.cpp
        edmstream.hpp
        slkmeans.hpp
        denstream.hpp
        dstream.hpp)
