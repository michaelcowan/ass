cmake_minimum_required(VERSION 3.11)
project(ass)

set(CMAKE_CXX_STANDARD 14)

add_executable(ass ass.hpp tests/catch/catch.hpp tests/unit_tests.cpp)

enable_testing()
add_custom_target(check COMMAND ${CMAKE_CTEST_COMMAND} --verbose)
add_test(NAME unit COMMAND ass "-s" "-r" "console" "--use-colour" "yes")