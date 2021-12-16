cmake_minimum_required(VERSION 3.16)
project(tailsort)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_BUILD_TYPE "Release")

add_executable(tailsort main.cpp tailsort.h timsort.h)