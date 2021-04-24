cmake_minimum_required(VERSION 3.10)
project(sudoku LANGUAGES CXX)

SET(CMAKE_CXX_FLAGS "-std=c++17")

add_library(sudoku_lib STATIC
# Source and private headers
    src/sudoku/Grid.cpp
    src/sudoku/Solver.cpp
    src/sudoku/CombinationsGen.cpp
    src/sudoku/CombinationsGen.h

# Public headers
    include/sudoku/Cell.h
    include/sudoku/Grid.h
    include/sudoku/Solver.h
)

target_include_directories(sudoku_lib
    PRIVATE include/sudoku
    PUBLIC include
)