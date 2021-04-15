cmake_minimum_required(VERSION 3.10)

project(sudoku_solver LANGUAGES CXX)

SET(CMAKE_CXX_FLAGS "-std=c++17")
SET(CMAKE_CXX_FLAGS_DEBUG "-g -O0")
SET(CMAKE_CXX_FLAGS_RELEASE "-O3 -s")

add_executable(sudoku_solver
    main.cpp
    cell.cpp 
    grid.cpp 
    solverbruteforce.cpp
    visualgrid.cpp
    solverlogic.cpp
    combination.cpp
    abstractsolver.cpp
    testsolverlogic.cpp
    
    cell.h
    grid.h
    solverbruteforce.h
    visualgrid.h
    solverlogic.h
    combination.h
    abstractsolver.h
    testsolverlogic.h
)

