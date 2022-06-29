cmake_minimum_required(VERSION 3.16)
project(tailsort)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_BUILD_TYPE "Release")

add_executable(tailsort main.cpp tailsort.h timsort.h)

add_executable(test test.cpp)

add_executable(test2 test2.cpp)

add_executable(test3 test3.cpp)

add_executable(test4 test4.cpp)

add_executable(test_pod test_pod.cc)

add_executable(test_send test_send.cpp)
