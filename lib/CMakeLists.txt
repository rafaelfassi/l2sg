cmake_minimum_required(VERSION 3.10)
project(sudoku LANGUAGES CXX)

SET(CMAKE_CXX_FLAGS "-std=c++17")

add_library(sudoku_lib STATIC
# Source
    src/sudoku/Grid.cpp
    src/sudoku/Solver.cpp
    src/sudoku/techniques/NakedSingles.cpp
    src/sudoku/techniques/HiddenSingles.cpp
    src/sudoku/techniques/LockedCandidates.cpp
    src/sudoku/techniques/XWings.cpp
    src/sudoku/techniques/Swordfish.cpp
    src/sudoku/techniques/HiddenMulti.cpp
    src/sudoku/techniques/NakedMulti.cpp

# Private headers
    src/sudoku/Utils.h

# Public headers
    include/sudoku/Cell.h
    include/sudoku/Grid.h
    include/sudoku/Solver.h
)

target_include_directories(sudoku_lib
    PRIVATE
        src/sudoku
        include/sudoku
    PUBLIC
        include
)
