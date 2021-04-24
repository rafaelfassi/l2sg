cmake_minimum_required(VERSION 3.10)
project(sudoku LANGUAGES CXX)

SET(CMAKE_CXX_FLAGS "-std=c++17")

add_executable(all_tests
    TestSolveLevel.cpp
)

target_link_libraries(all_tests PRIVATE
    sudoku_lib
)

add_test(NAME all_tests COMMAND all_tests)

