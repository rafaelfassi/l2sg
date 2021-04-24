cmake_minimum_required(VERSION 3.10)
project(sudoku LANGUAGES CXX)

SET(CMAKE_CXX_FLAGS "-std=c++17")

add_executable(sudoku_app
    main.cpp
)

target_link_libraries(sudoku_app PRIVATE
    sudoku_lib
)
