cmake_minimum_required(VERSION 3.0)
project(untitled1)

set(CMAKE_CXX_STANDARD 26)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -mavx")

add_executable(untitled1 main.cpp
        dataset.cpp
        dataset.h
        analysis.cpp
        analysis.h
        vector.cpp
        vector.h)
