cmake_minimum_required(VERSION 3.10)

project(sudoku_solver LANGUAGES CXX)
enable_testing()

SET(CMAKE_CXX_FLAGS "-std=c++17")
SET(CMAKE_CXX_FLAGS_DEBUG "-g -O0")
SET(CMAKE_CXX_FLAGS_RELEASE "-O3 -s")

add_library(sudoku_solver_lib STATIC
    cell.cpp 
    grid.cpp 
    solverbruteforce.cpp
    solverlogic.cpp
    combination.cpp
    abstractsolver.cpp
    
    cell.h
    grid.h
    solverbruteforce.h
    solverlogic.h
    combination.h
    abstractsolver.h
)

add_executable(sudoku_solver_app
    main.cpp
)

target_link_libraries(sudoku_solver_app
	PUBLIC
    sudoku_solver_lib
)


