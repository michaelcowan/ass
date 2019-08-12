cmake_minimum_required(VERSION 3.14)
project(ass)

set(CMAKE_CXX_STANDARD 14)

add_executable(ass ass.hpp tests/catch/catch.hpp tests/unit_tests.cpp)